#pragma once
#include <pqxx/pqxx>
#include <mutex>

//pqxx::work tx(Con);
// примеры tx.exec("DROP TABLE IF EXISTS Phone;"); && tx.exec("create table if not exists User_ ("
//          "id SERIAL PRIMARY KEY, first_name VARCHAR(60), last_name VARCHAR(60), email VARCHAR(60)); ");
// экранизация tx.esc(last_name)
// tx.commit(); сохраняем транзакцию

class DataBase
{
public:
    pqxx::connection connection_;
    std::mutex connection_mutex_;

    DataBase(pqxx::connection&& connection) noexcept;

    DataBase() = delete;

    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    DataBase(DataBase&& other) noexcept;
    DataBase& operator=(DataBase&& other) noexcept;


    void InitDB();

    std::string InsertWord(std::string word, pqxx::work& tx);
    std::string InsertPage(std::string url, pqxx::work& tx);

    void InsertPageWord(std::string pageId, std::string wordId, std::string count, pqxx::work& tx);

    std::string SearchWord(std::string word, pqxx::nontransaction& tx);
    std::string SearchPage(std::string url, pqxx::nontransaction& tx);

    std::vector<std::string> SearchPages(const std::string& query);
};
