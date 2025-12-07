#pragma once
#include <pqxx/pqxx>


//pqxx::work tx(Con);
// примеры tx.exec("DROP TABLE IF EXISTS Phone;"); && tx.exec("create table if not exists User_ ("
//          "id SERIAL PRIMARY KEY, first_name VARCHAR(60), last_name VARCHAR(60), email VARCHAR(60)); ");
// экранизация tx.esc(last_name)
// tx.commit(); сохраняем транзакцию

class DataBase
{
private:
    pqxx::connection connection_;

public:
    DataBase(pqxx::connection&& connection) noexcept;

    DataBase() = delete;

    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    DataBase(DataBase&& other) noexcept;
    DataBase& operator=(DataBase&& other) noexcept;


    void InitDB();

    void InsertWord(std::string word);
    void InsertPage(std::string url);

    void InsertPageWord(std::string pageId, std::string wordId, std::string count);

    std::string SearchWord(std::string word);
    std::string SearchPage(std::string url);
};
