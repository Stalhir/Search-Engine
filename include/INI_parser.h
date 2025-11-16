#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <string>

using namespace std;

bool boolFind(string::iterator begin, string::iterator end, char sym);


void DeleteAllSym(string& Str , string::iterator begin, string::iterator end, char sym);

struct Section
{
    string name;
    map<string, string> var;

    Section(string name);
};

class ini_parser
{
private:
	vector<Section> Sections;

	fstream file;
public:

ini_parser(string fileadress);


bool AddSection(string& Str);


bool AddVar(string& Str, int IndexSection);


void readFile();



string getvalue(string SectionName, string ValueName);


~ini_parser();

};


