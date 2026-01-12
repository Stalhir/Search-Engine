#include "SearchEngine.h"

SearchEngine::SearchEngine(net::io_context &ioc, unsigned short port, const std::string &cert_file,
    const std::string &key_file, std::shared_ptr<DataBase> db) : server_(ioc, port, cert_file, key_file,db)
{

}
