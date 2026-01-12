#include "initializer.h"

#include <iostream>
#include <iostream>





InitSearchEngine::InitSearchEngine(std::string path) : parser(path)
{
GetSettings();
}

void InitSearchEngine::GetSettings()
{
    settings.push_back(parser.getvalue("database","host"));
    settings.push_back(parser.getvalue("database","port"));
    settings.push_back(parser.getvalue("database","name"));
    settings.push_back(parser.getvalue("database","login"));
    settings.push_back(parser.getvalue("database","password"));

    settings.push_back(parser.getvalue("crowler","start_page"));
    settings.push_back(parser.getvalue("crowler","recursion_depth"));

    settings.push_back(parser.getvalue("search_engine","port"));
}


Crowler InitSearchEngine::SetForCrowler(httpclient& client, indexer& idx, ThreadPool& pool)
{
return Crowler(client, idx, pool, settings[5], std::stoi(settings[6]));
}


SearchEngine InitSearchEngine::SetForSE(net::io_context &ioc, const std::string &cert_file,
    const std::string &key_file, std::shared_ptr<DataBase> db)
{
return SearchEngine(ioc, std::stoi(settings[7]) ,cert_file,key_file,db);
}

DataBase InitSearchEngine::SetForDB() {
    std::string init = "host=" + settings[0] + " "
    "port=" + settings[1] + " "
    "dbname=" + settings[2] + " "
    "user=" + settings[3] + " "
    "password=" + settings[4] + " ";

    pqxx::connection c(init);

return DataBase(std::move(c));
}