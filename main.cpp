#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>

// test function 

int main() 
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, "https://www.scrapingcourse.com/ecommerce/");
	curl_easy_perform(handle);
	/*curl_version_info_data *vers = curl_version_info(CURLVERSION_NOW);
	std::cout<<vers->version<<std::endl;*/

	curl_global_cleanup();
	return 0;
}
