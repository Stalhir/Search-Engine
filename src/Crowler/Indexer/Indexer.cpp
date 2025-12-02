#include "Indexer.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include  <boost/locale.hpp>
#include "pugixml.hpp"
#include "SYMBOLS.h"

indexer::indexer() {

}


ParsedUrl indexer::FixAndCheckURL(ParsedUrl BasicUrl , ParsedUrl SeparateURL) // берёт ссылку и если она относительная например то поправляет на основе той что вошла в индексер
{

// BasicUrl это изначальная ссылка. Приминяется например когда у нас относительная ссылка
    if (SeparateURL.port == "" || SeparateURL.host == "")
    {
        SeparateURL.port = BasicUrl.port;
        SeparateURL.host = BasicUrl.host;
    }

    return SeparateURL;
}

ParsedUrl indexer::ParsingURL(std::string url)
{

ParsedUrl parsed_url;

bool ThisOnlyTarget{false};

    if (url.find("http://") != std::string::npos){
        parsed_url.port = "80";
        url.erase(url.find("http://"), 7);
    }
    else if (url.find("https://") != std::string::npos) {
        parsed_url.port = "443";
        url.erase(url.find("https://"), 8);
    }
    else
    {
        ThisOnlyTarget = true;
    }

    if(!ThisOnlyTarget)
    {
        parsed_url.host = url.substr(0,url.find("/"));
        url.erase(0, url.find("/"));
    }
    else
    {
        parsed_url.host = "";
        parsed_url.port = "";
        parsed_url.target = url;
    }


return parsed_url;
}

std::string indexer::DelHead(std::string response)
{

response.erase(response.find("<head>"), response.find("</head>"));


return response;
}

std::vector<std::string> indexer::GetHrefs(std::string response)// бывает короче так.. Что нам нужно заходить в глубь тоесть без адреса и тд.
//Нужно короче просто сохранять гдет адрес и текущий таргет сайта где хрaнить
// ЗАМЕНИ HTML ENTITIES перед парсингом
// УБЕРИ НЕВАЛИДНЫЕ СИМВОЛЫ . Хотя от пк много ресурсов будет жрать. лучше сразу находить ссылки и в поток их
{
    std::vector<std::string> test;

    pugi::xml_document doc;
    doc.load_string(response.c_str());

    auto hrefs = doc.select_nodes("//a/@href");
    std::cout << "Found " << hrefs.size() << " links:" << std::endl;
    for (auto     attr : hrefs) {
        std::string url = attr.attribute().value();

        //std::cout << "URL: " << url << std::endl;

        test.push_back(url);
    }
    return test;
}

std::string indexer::DelHTML(std::string response)
{//настроить чтоб HEAD вырезался полностью
    bool in_tag{};
    std::string result;

    for (char c : response) {
        if (c == '<') {
            in_tag = true;
        } else if (c == '>') {
            in_tag = false;
        } else if (!in_tag) {
            result += c;
        }
    }
    return result;
}

std::string indexer::RefactorText(std::string response)
{
    std::string result;

    bool IsSpec{false};

    for (char c : response)
    {
        IsSpec = false;
        for (char s : spec_symbols::special_chars)
        {
            if (c == s)
            {
                IsSpec = true;
                break;
            }
        }
        if (!IsSpec)
        {
        result += c;
        }
    }

std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
{
    return std::tolower(c);
});

return result;
}

std::unordered_map<std::string, int> indexer::SeparateWords(std::string response)
{
    std::unordered_map<std::string, int> words;

    std::string word;

    // создаём строку А. цикл проходящий по всей строке респонс. Далее игнорируем каждый пробел и не записываем их. Видим что то кроме пробела смотрим растояния до следующего.
    // если оно соответствует тз проверяем нету ли этой строки в массиве и либо добовляем либо увеличиваем счётчик в уже существующем элементе. Если не соответствует скипаем. А также чистим строку

    int step = 0;
    int skipCount = 0;

    for (char c : response)
    {
       if (c != ' ' && skipCount <= 0)
       {
           auto it = std::find(response.begin()+step, response.end(), ' ');
           int distance = std::distance(response.begin()+step, it);
           if (distance < 32 && distance > 3)
           {
               word = std::string(response.begin()+step, it);//скип слов дописать надо
               words[word]++;
           }
           skipCount = distance;
       }

    skipCount--;
    step++;
    }

    return words;
}

void indexer::AddToDB(std::unordered_map<std::string, int> words)
{
std::cout << words.size() << std::endl;
}


void indexer::Index(std::string response, ParsedUrl url)
{
    //std::vector<std::string> hrefs = GetHrefs(response);


    std::string result = DelHead(response);

    result = DelHTML(result);
    result = RefactorText(result);

    AddToDB(SeparateWords(result));
}