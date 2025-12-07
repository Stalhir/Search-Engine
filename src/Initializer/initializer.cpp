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

/*
Crowler&& InitSearchEngine::SetForCrowler() // rvalue чтоб сразу принималось.
{
return Crowler();
}
*/

SearchEngine InitSearchEngine::SetForSE()
{
return SearchEngine();
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