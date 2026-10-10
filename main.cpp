#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <iostream>
#include <string>

// test function 

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::cout << curl_version() << "\n";

    std::string url  = "https://example.com";
    std::string html = "<html><body><a href=\"/a\">A</a><a href=\"/b\">B</a></body></html>";

    htmlDocPtr doc = htmlReadMemory(html.c_str(), (int)html.size(), url.c_str(), nullptr,
                                    HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (!doc) {
        std::cerr << "parse failed\n";
        curl_global_cleanup();
        return 1;
    }

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    xmlXPathObjectPtr res = xmlXPathEvalExpression((const xmlChar*)"//a/@href", ctx);

    if (res && res->nodesetval) {
        for (int i = 0; i < res->nodesetval->nodeNr; ++i) {
            xmlChar* href = xmlNodeGetContent(res->nodesetval->nodeTab[i]);
            std::cout << "link: " << (const char*)href << "\n";
            xmlFree(href);
        }
    }
    std::cout<<"check done"<<std::endl;

    xmlXPathFreeObject(res);
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    curl_global_cleanup();
}
