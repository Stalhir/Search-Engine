#pragma once
#include <pqxx/pqxx>

//pqxx::work tx(Con);
// примеры tx.exec("DROP TABLE IF EXISTS Phone;"); && tx.exec("create table if not exists User_ ("
//          "id SERIAL PRIMARY KEY, first_name VARCHAR(60), last_name VARCHAR(60), email VARCHAR(60)); ");
// экранизация tx.esc(last_name)
// tx.commit();

class DataBase
{
private:
    pqxx::connection& connection_;

public:
    DataBase(pqxx::connection& connection);
};
