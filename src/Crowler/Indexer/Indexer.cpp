#include "Indexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include  <boost/locale.hpp>
#include "pugixml.hpp"
#include "SYMBOLS.h"

indexer::indexer(DataBase&& database) : database(std::move(database)) {

}


ParsedUrl indexer::FixURL(ParsedUrl BasicUrl , ParsedUrl SeparateURL)
{

// BasicUrl это изначальная ссылка. Приминяется например когда у нас относительная ссылка
    if (SeparateURL.port.empty() || SeparateURL.host.empty())
    {
        SeparateURL.port = BasicUrl.port;
        SeparateURL.host = BasicUrl.host;
    }

    std::string target = SeparateURL.target;

    if (!target.empty()) {

        if (target.size() >= 2 && target[0] == '.' && target[1] == '/') {
            target = target.substr(2);
        }

        if (!target.empty() && target[0] != '/' && !BasicUrl.target.empty()) {

            size_t last_slash = BasicUrl.target.find_last_of('/');
            if (last_slash != std::string::npos) {

                std::string base_dir = BasicUrl.target.substr(0, last_slash + 1);
                target = base_dir + target;
            } else {
                target = "/" + target;
            }
        }

        while (target.size() >= 2 && target[0] == '.' && target[1] == '/') {
            target = target.substr(2);
        }

        if (!target.empty() && target[0] != '/') {
            target = "/" + target;
        }
    }

    SeparateURL.target = target;
    return SeparateURL;
}

bool indexer::CheckUrl(ParsedUrl url)
{

    std::string target = url.target;
    int last_dot = target.find_last_of('.');
    int last_slash = target.find_last_of('/');

    if (!url.target.empty() && !url.host.empty() && !url.port.empty()) {
        return true;
    }

    if(target.find("#") != std::string::npos) {
        return false;
    }
    std::transform(target.begin(), target.end(), target.begin(), ::tolower);


    if(last_dot == std::string::npos ||
     (last_slash != std::string::npos && last_dot < last_slash)) {
        return true;
     }

    if(target.find("mailto:") == 0 ||
       target.find("javascript:") == 0 ||
       target.find("tel:") == 0)
    {
        return false;
    }

    std::string extension = target.substr(last_dot);

    for(const auto& ext : SPECIAL_CHAR_STRING::badExtension) {
        if(extension.find(ext) != std::string::npos) {
            return false;
        }
    }

return true;
}

ParsedUrl indexer::ParsingURL(std::string url)
{

ParsedUrl parsed_url;

bool ThisOnlyTarget{false};

    if (url.find("http://") == 0){
        parsed_url.port = "http://";
        url.erase(0, 7);
    }
    else if (url.find("https://") == 0) {
        parsed_url.port = "https://";// вместо чисел сделал так
        url.erase(0, 8);
    }
    else
    {

    }


    int slash_pos = url.find('/');

    if (slash_pos != std::string::npos) {
        if (slash_pos > 0) {
            parsed_url.host = url.substr(0, slash_pos);
            parsed_url.target = url.substr(slash_pos);
        }
        else
        {
            parsed_url.host = "";
            parsed_url.target = url;
        }
    }
    else
    {
        if (url.empty() || url[0] == '#' || url[0] == '?') {
            parsed_url.host = "";
            parsed_url.target = url;
        }
        else
        {
            parsed_url.host = url;
            parsed_url.target = "/";
        }
    }

return parsed_url;
}

std::string indexer::DelHead(std::string response)
{


if(response.find("<head>") != std::string::npos
   && response.find("</head>") != std::string::npos)
{
response.erase(response.find("<head>"), response.find("</head>"));
}

return response;
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
{//настроить чтоб HEAD вырезался полностью
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
    std::string result;

    bool IsSpec{false};

    for (char c : response)
    {
        IsSpec = false;
        for (char s : SPECIAL_CHAR_STRING::special_chars)
        {
            if (c == s)
            {
                IsSpec = true;
                break;
            }
        }


        if (!IsSpec && std::isprint(static_cast<unsigned char>(c)))
        {
        result += c;
        }
    }

std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
{
    return std::tolower(c);
});

return result;
}

std::unordered_map<std::string, int> indexer::SeparateWords(std::string response)
{
    std::unordered_map<std::string, int> words;
    std::stringstream ss(response);
    std::string word;


    const size_t MIN_LENGTH = 3;
    const size_t MAX_LENGTH = 32;

    while (ss >> word)
    {
        size_t len = word.length();


        if (len > MIN_LENGTH && len < MAX_LENGTH)
        {
            words[word]++;
        }

    }

    return words;
}

void indexer::AddToDB(const std::unordered_map<std::string, int>& words, ParsedUrl url)
{
    std::cout<< "Add BD start" << std::endl;
    std::lock_guard<std::mutex> lock(mutDB);
    pqxx::work tx(database.connection_);


    std::string full_url = url.host + url.target;
    std::string pageid = database.InsertPage(full_url,tx);
    std::cout<< "InsertPage okey" << std::endl;
    for (auto word : words) {
    std::cout<<"WORD: " << word.first << std::endl; // WORD: ░╬k∙╒hоу▲К░hїlд√╢q►vШn
        std::string wordid = database.InsertWord(word.first,tx);
        std::cout<< "InsertWord okey" << std::endl;

        database.InsertPageWord(pageid, wordid, std::to_string(word.second),tx);
    }
    std::cout<< "Add BD okey";
    tx.commit();
}


void indexer::Index(std::string response, ParsedUrl url)
{
    //std::vector<std::string> hrefs = GetHrefs(response);


    std::string result = DelHead(response);
    std::cout<< "DelHead okey" << std::endl;
    result = DelHTML(result);
    std::cout<< "DelHTML okey" << std::endl;
    result = RefactorText(result);
    std::cout<< "RefactorText okey" << std::endl;
    //SeparateWords(result);
    AddToDB(SeparateWords(result),url);
    std::cout<< "Index okey" << std::endl;
}