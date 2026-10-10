#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>

// test function 

std::string get_request(std::string url)
{
	CURL *curl = curl_easy_init();
	std::string result {};
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void *contents, size_t size, size_t nmemb, std::string *response)
				{
				((std::string *) response)->append((char *) contents, size *nmemb);
				return size * nmemb;
				});
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
		curl_easy_perform(curl);
	}
	return result;
}

int main() 
{
	curl_global_init(CURL_GLOBAL_ALL);
	std::string html_document = get_request("https://www.scrapingcourse.com/ecommerce/");
	std::cout<<"test"<<std::endl;
	std::cout<<html_document;
	std::cout<<"test"<<std::endl;


	curl_global_cleanup();
	return 0;
}
