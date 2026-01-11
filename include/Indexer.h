#pragma once
#include <string>
#include  "DataBase.h"
#include <vector>
#include <unordered_map>
#include <mutex>


struct ParsedUrl
{
std::string host;
std::string port;
std::string target;
};




class indexer
{
public:
    DataBase database;

    std::mutex mutDB;
public:
    indexer(DataBase&& database);

    void Index(std::string response, ParsedUrl url);

public:

    ParsedUrl ParsingURL(std::string url);

    ParsedUrl FixURL(ParsedUrl BasicUrl , ParsedUrl SeparateURL);

    bool CheckUrl(ParsedUrl url);

    std::string DelHead(std::string response);

    std::vector<std::string> GetHrefs(std::string response);

    std::string DelHTML(std::string response);

    std::string RefactorText(std::string response);

    std::unordered_map<std::string, int> SeparateWords(std::string response);

    void AddToDB(const std::unordered_map<std::string, int> &words, ParsedUrl url);
};

