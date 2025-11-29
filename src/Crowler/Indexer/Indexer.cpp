#include "Indexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include  <boost/locale.hpp>
#include "pugixml.hpp"
#include "SYMBOLS.h"

indexer::indexer() {

}

std::vector<std::string> indexer::ParsingURL(std::string url)
{
std::vector<std::string> parsed_url;
std::string host, port, target;

    if (url.find("http://") != std::string::npos){
        port = "80";
        url.erase(url.find("http://"), 7);
    }
    else if (url.find("https://") != std::string::npos) {
        port = "443";
        url.erase(url.find("https://"), 8);
    }
    else
    {
    // эт может быть типо таргет для директории надо придумать чё с таким делать
    }


    host = url.substr(0,url.find("/"));
    url.erase(0, url.find("/"));

    target = url;

    parsed_url.push_back(host);
    parsed_url.push_back(port);
    parsed_url.push_back(target);
return parsed_url;
}


std::vector<std::string> indexer::GetHrefs(std::string response)// бывает короче так.. Что нам нужно заходить в глубь тоесть без адреса и тд.
//Нужно короче просто сохранять гдет адрес и текущий таргет сайта где хрaнить
// ЗАМЕНИ HTML ENTITIES перед парсингом
// УБЕРИ НЕВАЛИДНЫЕ СИМВОЛЫ . Хотя от пк много ресурсов будет жрать. лучше сразу находить ссылки и в поток их
{
    std::vector<std::string> test;

    pugi::xml_document doc;
    doc.load_string(response.c_str());

    auto hrefs = doc.select_nodes("//a/@href");
    std::cout << "Found " << hrefs.size() << " links:" << std::endl;
    for (auto     attr : hrefs) {
        std::string url = attr.attribute().value();

        std::cout << "URL: " << url << std::endl;

        test.push_back(url);
    }
    return test;
}

std::string indexer::DelHTML(std::string response)
{
    bool in_tag{};
    std::string result;

    for (char c : response) {
        if (c == '<') {
            in_tag = true;
        } else if (c == '>') {
            in_tag = false;
        } else if (!in_tag) {
            result += c;
        }
    }
    return result;
}

std::string indexer::RefactorText(std::string response)
{
    boost::locale::to_lower(response);



    return response;
}

std::vector<std::pair<std::string, int>> indexer::SeparateWorlds(std::string response)
{
    std::vector<std::pair<std::string, int>> words;


    return words;
}

void indexer::AddToDB(std::vector<std::string> words)
{

}
