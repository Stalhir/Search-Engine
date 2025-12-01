#pragma once
#include <string>
#include <vector>
#include <unordered_map>
// План:
/*
5. делим на отдельные слова .. передаём бд |
*/

class indexer
{
public:
    indexer();

    void Index(std::string response, std::string url); // результ не нужен сделаем просто функцию типо которая обьеденяет все

public:

    std::vector<std::string> ParsingURL(std::string url);

    std::vector<std::string> GetHrefs(std::string response);

    std::string DelHTML(std::string response);

    std::string RefactorText(std::string response);

    std::unordered_map<std::string, int> SeparateWords(std::string response);

    void AddToDB(std::vector<std::string> words);
};

