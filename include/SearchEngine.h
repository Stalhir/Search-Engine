#pragma once
#include "httpserver.h"
#include "DataBase.h"

class SearchEngine
{
private:
    net::io_context ioc;
    unsigned short port = 10322;
    std::string cert_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.crt";
    std::string key_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.key";
    std::shared_ptr<DataBase> db;
    http_server server_{ioc, port, cert_file, key_file,db};

};

