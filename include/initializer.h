#pragma once
#include "INI_parser.h"
#include <vector>
#include <string>
#include "Crowler.h"
#include "DataBase.h"
#include "SearchEngine.h"

class InitSearchEngine
{
public:
    std::vector<std::string> settings;

    ini_parser parser;

    void GetSettings();

    DataBase SetForDB();

    SearchEngine SetForSE(net::io_context &ioc, const std::string &cert_file,
    const std::string &key_file, std::shared_ptr<DataBase> db);

    Crowler SetForCrowler(httpclient& client, indexer& idx, ThreadPool& pool);
public:

    InitSearchEngine(std::string path);
};