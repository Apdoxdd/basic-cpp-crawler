#include "libxml/parser.h"
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>
#include <set>
#include <fstream>



int main() 
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL *handle = curl_easy_init();
	CURLcode code {};
	if (!handle)
	{
		std::cout<<"failed to create easy handle"<<std::endl;
		return -1;
	}
	auto chunk_inc = [](char *chunk, size_t size, size_t nmemb, void *userp)
	{
		size_t bytes = size * nmemb;
		std::string *response = static_cast<std::string *>(userp);
		response->append(chunk, bytes);
		return bytes;
	};
	std::string base_url {"https://www.scrapingcourse.com/ecommerce/"};
	std::string response {};
	curl_easy_setopt(handle, CURLOPT_URL,base_url.c_str()); 
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, +chunk_inc); // + to make the vardiac argument (lamda function) convert to func ptr
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
	code = curl_easy_perform(handle);
	if (code != CURLE_OK)
	{
		std::cout<<"failed to access website"<<std::endl;
		std::cout<<curl_easy_strerror(code)<<std::endl;
	}
	/*curl_version_info_data *vers = curl_version_info(CURLVERSION_NOW);
	std::cout<<vers->version<<std::endl;
	std::cout<<response<<std::endl;*/

	 std::fstream data_base{"../data_base.txt", std::ios::out | std::ios::app | std::ios::in};
	 //start-parse logic
	 htmlDocPtr doc = htmlReadMemory(response.c_str(), response.length(), base_url.c_str(), nullptr, HTML_PARSE_NOERROR);
	 if (!doc)
	 {
		 std::cout<<"failed to parse html"<<std::endl;
		 return -1;
	 }
	 xmlXPathContextPtr context = xmlXPathNewContext(doc);
	 xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar *)"//a/@href", context);
	 
	 std::set<std::string> links;
	 if (result && result->nodesetval)
	 {
	 	 for (auto i {0uz}; i < result->nodesetval->nodeNr; ++i)
	 	 {
	 	 	 xmlChar *href = xmlNodeGetContent(result->nodesetval->nodeTab[i]);
	 	 	 if (href)
	 	 	 {
	 	 	 	 links.insert((const char*)href);
	 	 	 	 xmlFree(href);
	 	 	 }
	 	 }
	 }
	 for (auto link : links)
	 {
	 	 std::cout<<link<<std::endl;
	 	 data_base<<link<<"\n";
	 }



	 //end-parse logic

	 



	curl_global_cleanup();
	return 0;
}
