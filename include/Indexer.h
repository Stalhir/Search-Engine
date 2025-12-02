#pragma once
#include <string>
#include <vector>
#include <unordered_map>
// План:
/*
5. делим на отдельные слова .. передаём бд |
*/


struct ParsedUrl // заменить вектора ссылки на это |
{
std::string host;
std::string port;
std::string target;
};




class indexer
{
public:
    indexer();

    void Index(std::string response, ParsedUrl url); // результ не нужен сделаем просто функцию типо которая обьеденяет все

public:

    ParsedUrl ParsingURL(std::string url);

    ParsedUrl FixAndCheckURL(ParsedUrl BasicUrl , ParsedUrl SeparateURL);

    std::string DelHead(std::string response);

    std::vector<std::string> GetHrefs(std::string response);

    std::string DelHTML(std::string response);

    std::string RefactorText(std::string response);

    std::unordered_map<std::string, int> SeparateWords(std::string response);

    void AddToDB(std::unordered_map<std::string, int> words);
};

