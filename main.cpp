#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>

// test function 
//

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
	std::string webDoc {};
	curl_easy_setopt(handle, CURLOPT_URL, "https://www.scrapingcourse.com/ecommerce/");
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, +chunk_inc); // + to make the vardiac argument (lamda function) convert to func ptr
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &webDoc);
	code = curl_easy_perform(handle);
	if (code != CURLE_OK)
	{
		std::cout<<"failed to access website"<<std::endl;
		std::cout<<curl_easy_strerror(code)<<std::endl;
	}
	/*curl_version_info_data *vers = curl_version_info(CURLVERSION_NOW);
	std::cout<<vers->version<<std::endl;*/
	std::cout<<webDoc<<std::endl;

	curl_global_cleanup();
	return 0;
}
