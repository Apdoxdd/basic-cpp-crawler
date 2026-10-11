#pragma once
#include <curl/curl.h>
#include <fstream>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <set>
#include <iostream>

class crawler 
{
private:
	CURL *handle;
	std::fstream data_base;
	std::string response;	
public:
	crawler();
	~crawler();
	std::string get_request(std::string& url);
	std::set<std::string>extract_links(const std::string& base_url);
	void crawl();


};

