#include "Indexer.h"
#include <string>
#include <regex>
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

    if (SeparateURL.port.empty() || SeparateURL.host.empty())
    {
        SeparateURL.port = BasicUrl.port;
        SeparateURL.host = BasicUrl.host;
    }

    std::string target = SeparateURL.target;

    if (target.empty()) {
        SeparateURL.target = BasicUrl.target;
        return SeparateURL;
    }

    if (target[0] == '/') {
        SeparateURL.target = target;
        return SeparateURL;
    }


    std::string base_path = BasicUrl.target;
    size_t last_slash = base_path.find_last_of('/');

    if (last_slash != std::string::npos) {
        base_path = base_path.substr(0, last_slash + 1); // Включаем завершающий слэш
    } else {
        base_path = "/";
    }

    std::string current_path = base_path + target;


    std::vector<std::string> segments;
    std::stringstream ss(current_path);
    std::string segment;


    while (std::getline(ss, segment, '/')) {
        if (segment.empty() || segment == ".") {
            continue;
        } else if (segment == "..") {

            if (!segments.empty()) {
                segments.pop_back();
            }
        } else {
            segments.push_back(segment); // Обычный сегмент
        }
    }


    bool ends_with_slash = (!SeparateURL.target.empty() && SeparateURL.target.back() == '/');

    target = "";
    for (const auto& s : segments) {
        target += "/";
        target += s;
    }

    if (segments.empty()) {
        target = "/";
    }

    else if (ends_with_slash) {
        target += "/";
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
        parsed_url.port = "80";
        url.erase(0, 7);
    }
    else if (url.find("https://") == 0) {
        parsed_url.port = "443";// вместо чисел сделал так
        url.erase(0, 8);
    }
    else if (url[0] == '/' && url[1] == '/') {
        url.erase(0, 2);
    }
    else
    {

    }

    size_t hash_pos = url.find('#');

    if (hash_pos != std::string::npos) {
        url.erase(hash_pos);
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
response.erase(response.find("<head>"), response.find("</head>")+7);
}

return response;
}

std::vector<std::string> indexer::GetHrefs(std::string response)
{
    const std::regex href_regex("<a\\s+(?:[^>]*?\\s+)?href=\"([^\"]*)\"", std::regex::icase); 

    std::vector<std::string> hrefs;
    std::smatch match;
    std::string::const_iterator search_start(response.cbegin());

    int count = 0;
    while (std::regex_search(search_start, response.cend(), match, href_regex))
    {
        std::string url = match[1].str();
        hrefs.push_back(url);
        search_start = match.suffix().first;
        count++;
    }

    return hrefs;
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

    std::lock_guard<std::mutex> lock(database.connection_mutex_);
    pqxx::work tx(database.connection_);

    std::string protocol;
    if (url.port == "443") {
        protocol = "https://";
    } else if (url.port == "80") {
        protocol = "http://";
    } else {
        protocol = "http://";
    }


    std::string full_url = protocol + url.host + url.target;
    std::string pageid = database.InsertPage(full_url,tx);
    for (auto word : words) {
   // WORD: ░╬k∙╒hоу▲К░hїlд√╢q►vШn
        std::string wordid = database.InsertWord(word.first,tx);


        database.InsertPageWord(pageid, wordid, std::to_string(word.second),tx);
    }
    tx.commit();
}


void indexer::Index(std::string response, ParsedUrl url)
{
    //std::vector<std::string> hrefs = GetHrefs(response);


    //std::string result = DelHead(response);

    response = DelHTML(response);

    response = RefactorText(response);

    AddToDB(SeparateWords(response),url); //not problem

}