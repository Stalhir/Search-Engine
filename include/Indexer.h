#pragma once
#include <string>
#include <vector>

// План:
/*
4. убираем все слова Не соответсвующих параметрам(НАписаны в  ТЗ) |
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

    std::vector<std::pair<std::string, int>> SeparateWorlds(std::string response);

    void AddToDB(std::vector<std::string> words);
};

