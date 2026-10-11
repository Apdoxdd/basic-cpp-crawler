#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>

// test function 

int main() 
{
	curl_global_init();

	curl_global_cleanup();
	return 0;
}
