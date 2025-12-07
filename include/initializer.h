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

    SearchEngine SetForSE();

    Crowler&& SetForCrowler();
public:

    InitSearchEngine(std::string path);
};