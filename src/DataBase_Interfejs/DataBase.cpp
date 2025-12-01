#include "DataBase.h"

DataBase::DataBase(pqxx::connection &connection) : connection_(connection)
{

}
