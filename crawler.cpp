#include "crawler.hpp"
#include "curl/curl.h"

crawler::crawler(): response("")
{
	curl_global_init(CURL_GLOBAL_ALL);

	handle = curl_easy_init();
	if (!handle)
	{
		throw std::runtime_error{"Failed to create handle"};
	}
	curl_easy_setopt(handle, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
}
std::string crawler::get_request(std::string& url)
{
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
	return response;
}
std::set crawler::get_links(const std::string& base_url)
{

}
