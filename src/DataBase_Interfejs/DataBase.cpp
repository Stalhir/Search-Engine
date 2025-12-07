#include "DataBase.h"


DataBase::DataBase(pqxx::connection&& connection) noexcept
    : connection_(std::move(connection))
{
}

void DataBase::InitDB() {
    pqxx::work tx(connection_);
    tx.exec("create table if not exists Pages (id SERIAL PRIMARY KEY,url VARCHAR(512) UNIQUE); ");
    tx.exec("create table if not exists Words (id SERIAL PRIMARY KEY, word VARCHAR(35) UNIQUE); ");
    tx.exec("create table if not exists Page_Word (page_id INTEGER REFERENCES Pages(id),word_id INTEGER REFERENCES Words(id),count INTEGER,CONSTRAINT pg PRIMARY KEY (page_id, word_id)); ");
    tx.commit();
}

std::string DataBase::InsertPage(std::string url, pqxx::work& tx) {
    std::string safe_url = tx.quote(url);

    std::string query = "INSERT INTO Pages(url) VALUES (" + safe_url + ") "
                        "ON CONFLICT (url)DO UPDATE SET url = EXCLUDED.url "
                        "RETURNING id;";
    pqxx::result res = tx.exec(query);

    if (res.empty())
    {
        throw std::runtime_error("InsertPage failed to return ID.");
    }
    return res[0][0].as<std::string>();

}

std::string DataBase::InsertWord(std::string word, pqxx::work& tx) {
    std::string safe_word = tx.quote(word);

    std::string query = "INSERT INTO Words(word) VALUES (" + safe_word + ") "
                             "ON CONFLICT (word) DO UPDATE SET word = EXCLUDED.word "
                             "RETURNING id;";

    pqxx::result res = tx.exec(query);

    if (res.empty()) {
        throw std::runtime_error("InsertWord failed to return ID.");
    }
    return res[0][0].as<std::string>();

}

void DataBase::InsertPageWord(std::string pageId, std::string wordId, std::string count, pqxx::work& tx) {
    std::string query = "INSERT INTO Page_Word(page_id, word_id, count) "
                        "VALUES(" + pageId + ", " + wordId + ", " + count + ") "
                        "ON CONFLICT (page_id, word_id) DO UPDATE "
                        "SET count = " + count + ";";
    tx.exec(query);

};

std::string DataBase::SearchWord(std::string word) {
    pqxx::work tx(connection_);

    std::string safe_word = tx.quote(word);

    std::string sql = "SELECT id FROM Words WHERE word = " + safe_word + ";";

    pqxx::result res = tx.exec(sql);

    if (res.empty()) return "";
    return res[0][0].as<std::string>();
};
std::string DataBase::SearchPage(std::string url) {
    pqxx::work tx(connection_);

    std::string safe_url = tx.quote(url);

    std::string sql = "SELECT id FROM Pages WHERE url = " + safe_url + ";";

    pqxx::result res = tx.exec(sql);

    if (res.empty()) return "";
    return res[0][0].as<std::string>();
};

DataBase::DataBase(DataBase&& other) noexcept
    : connection_(std::move(other.connection_))
{
}


DataBase& DataBase::operator=(DataBase&& other) noexcept
{
    if (this != &other) {
        connection_ = std::move(other.connection_);
    }
    return *this;
}