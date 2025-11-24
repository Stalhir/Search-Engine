#pragma once
#include <string>
#include <vector>

// План:
/*
1. Достаём ссылки
2. Убираем теги html
3. Убираем знаки припинания опускаем всё в нижний регистр
4. убираем все слова Не соответсвующих параметрам(НАписаны в  ТЗ)
5. делим на отдельные слова .. передаём бд
*/

class indexer
{
public:
    indexer();

    void Index(std::string response); // результ не нужен сделаем просто функцию типо которая обьеденяет все

public:
    std::vector<std::string> GetHrefs(std::string response);

    void DelHTML(std::string response);

    void RefactorText(std::string response);

    std::vector<std::string> SeparateWorlds(std::string response);

    void AddToDB(std::vector<std::string> words);
};

