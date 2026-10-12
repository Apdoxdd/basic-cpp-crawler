#include "crawler.hpp"
#include "curl/curl.h"

crawler::crawler(): response(""), data_base("../data_base.txt", std::ios::in | std::ios::out | std::ios::app)
{
	curl_global_init(CURL_GLOBAL_ALL);

	handle = curl_easy_init();
	if (!handle)
	{
		throw std::runtime_error{"Failed to create handle"};
	}
	curl_easy_setopt(handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
}
void crawler::set_request(std::string& url)
{
	response.clear();
	CURLcode code{};
	auto chunk_inc = [](char *chunk, size_t size, size_t nmemb, void *userp)
	{
		size_t bytes = size * nmemb;
		std::string *full_chunk = static_cast<std::string *>(userp);
		full_chunk->append(chunk, bytes);
		return bytes;
	};
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, +chunk_inc); 
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response ); // data member
	
	code = curl_easy_perform(handle);

	if (code != CURLE_OK)
	{
		std::cout<<"failed to access website"<<std::endl;
		std::cout<<curl_easy_strerror(code)<<std::endl;
	}
}
std::set<std::string> crawler::extract_links(const std::string& base_url)
{
	htmlDocPtr doc = htmlReadMemory(response.c_str(), response.length(), base_url.c_str(), NULL, HTML_PARSE_NOERROR); std::set<std::string> links;
	if(!doc)
	{
		std::cout<<"failed to parse html"<<std::endl;
		return links;
	}
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	const char *query = "//a[@href"
	" and not(starts-with(normalize-space(@href), '#'))"
	" and not(starts-with(normalize-space(@href), '?'))"
	" and not(starts-with(normalize-space(@href), 'mailto:'))"
	" and not(starts-with(normalize-space(@href), 'tel:'))"
	" and not(starts-with(normalize-space(@href), 'javascript:'))"
	" and not(contains(@href, 'add-to-cart'))"
	"]/@href";
	xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar *)query, context);

	if (result && result->nodesetval)
	{
		xmlNodeSetPtr nodes = result->nodesetval;
		for (auto i {0}; i < nodes->nodeNr; ++i)
		{
			xmlChar *href = xmlNodeGetContent(nodes->nodeTab[i]);
			if (href)
			{
				links.insert((const char*)href);
				xmlFree(href);
			}
		}
	}
	xmlXPathFreeObject(result);
	xmlXPathFreeContext(context);
	xmlFreeDoc(doc);
	return links;
}
void crawler::crawl()
{
	std::streampos pos {0};
	while(true)
	{
		data_base.clear();
		data_base.seekg(pos);
		std::string base_url{};
		if(!(data_base>>base_url)) break;
		pos = data_base.tellg();

		this->set_request(base_url);
		std::set<std::string> links;
		links = this->extract_links(base_url);

		for(auto link : links)
		{
			data_base<<link<<"\n";
		}
		data_base.flush();

	}
}

crawler::~crawler()
{
	curl_easy_cleanup(handle);
	curl_global_cleanup();
}
