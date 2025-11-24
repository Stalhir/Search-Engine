#include "Indexer.h"
#include <string>
#include <vector>
#include <iostream>
#include  "locale.h"
#include "pugixml.hpp"

indexer::indexer() {

}


std::vector<std::string> indexer::GetHrefs(std::string response)
{
    std::vector<std::string> test;

    pugi::xml_document doc;
    doc.load_string(response.c_str());

    auto hrefs = doc.select_nodes("//a/@href");
    std::cout << "Found " << hrefs.size() << " links:" << std::endl;
    for (auto attr : hrefs) {
        std::string url = attr.attribute().value();

        std::cout << "URL: " << url << std::endl;

        test.push_back(url);
    }
    return test;
}

void indexer::DelHTML(std::string response)
{

}

void indexer::RefactorText(std::string response)
{

}

std::vector<std::string> indexer::SeparateWorlds(std::string response)
{
    std::vector<std::string> words;


    return words;
}

void indexer::AddToDB(std::vector<std::string> words)
{

}
