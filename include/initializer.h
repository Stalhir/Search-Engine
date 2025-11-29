#pragma once
#include "INI_parser.h"
#include <vector>
#include <string>
#include "Crowler.h"
#include "DataBase.h"
#include "SearchEngine.h"

class InitSearchEngine
{
private:
    std::vector<std::string> settings;

    ini_parser parser{"setting.ini"};

    void GetSettings();

    DataBase SetForDB();

    SearchEngine SetForSE();

    Crowler&& SetForCrowler();
public:

    InitSearchEngine();
};