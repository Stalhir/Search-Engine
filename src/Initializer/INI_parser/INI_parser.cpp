#include <iostream>
#include "INI_parser.h"
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

bool boolFind(string::iterator begin, string::iterator end, char sym)
{
	if(find(begin, end, sym) == end)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void DeleteAllSym(string& Str , string::iterator begin, string::iterator end, char sym)
{
	if (begin >= end || begin < Str.begin() || end > Str.end())
	{return;}

	auto new_end = remove(begin, end, sym);
	Str.erase(new_end, end);
}




Section::Section(string name) : name(name) {}



ini_parser::ini_parser(string fileadress) : file(fileadress)
{
	if (!file.is_open())
	{
		throw "the file did not open";
	}
	else
	{
		readFile();
	}
}

bool ini_parser::AddSection(string& Str)
{
	auto itBegin = find(Str.begin(), Str.end(), '[');
	auto itEnd = find(Str.begin(), Str.end(), ']');

	if (itBegin == Str.end() || itEnd == Str.end())
	{ return false; }
	else if (itBegin >= itEnd)
	{ return false; }
	else
	{
		Sections.push_back(Section(string(itBegin + 1, itEnd)));
		return true;
	}
};

bool ini_parser::AddVar(string& Str, int IndexSection)
{
	if (!boolFind(Str.begin(), Str.end(), '='))
	{ return false; }
	else if ((Str.front() == '=' || Str.back() == '='))
	{ return false; }
	else 
	{
		DeleteAllSym(Str, Str.begin(), find(Str.begin(), Str.end(), '='), ' ');
		auto itEqSign = find(Str.begin(), Str.end(), '=');

		if (Str.front() != '=')
		{
			string valName, valData;
			valName = string(Str.begin(), itEqSign);
			valData = string(++itEqSign, Str.end());
			cout << valName << endl;
			cout << valData << endl;
			Sections[IndexSection].var.insert(std::pair<string, string>(valName, valData));
		}
		else { return false; }

		return true;
	}
};

void ini_parser::readFile()
{
	
	string input;
	int IndSect = Sections.size()-1;
	int i = 0;

	while (getline(file, input))
	{
		
		auto it = std::find(input.begin(), input.end(), ';');
		if (it == input.begin()) { continue; }
		input.erase(it, input.end());
		
		
		if(AddSection(input))
		{
			IndSect++;	
		}
		else if(AddVar(input, IndSect))
		{
			
		}
		else { continue; }
	}
}


string ini_parser::getvalue(string SectionName, string ValueName)
{
	auto sectionit = std::find_if(Sections.begin(), Sections.end(),
		[&SectionName](const Section& sect) { return sect.name == SectionName; });

	if (sectionit == Sections.end()) {
		throw "No section with that name was found";
	}

	auto varit = sectionit->var.find(ValueName);
	if (varit == sectionit->var.end()) {
		throw "No vareable with that name was found";
	}
	const string& result = varit->second;
	

	return result;
}

ini_parser::~ini_parser()
{
	file.close();
}



