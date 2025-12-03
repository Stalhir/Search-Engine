#include <iostream>
#include "Crowler.h"
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include  <thread>

int main()
{ //Добавить обработку ошибок http download переписать под async boost beast скорее всего ошибка так исчезнет
try
{
    HMODULE ssl = LoadLibraryA("libssl-3-x64.dll");
    HMODULE crypto = LoadLibraryA("libcrypto-3-x64.dll");

    if(!ssl || !crypto) {
        std::cout << "OpenSSL DLL not found!" << std::endl;
        return 1;
    }
    std::cout << "OpenSSL DLL loaded successfully!" << std::endl;


    std::cout<< "Main Thread id: "<< std::this_thread::get_id() << std::endl;

    httpclient httpclient;
    indexer indexer_;
    ThreadPool pool(16);

    Crowler TEST(httpclient, indexer_, pool,2);

    ParsedUrl testurl;
    testurl.host = "www.rfc-editor.org";
    testurl.port = "443";
    testurl.target = "/rfc/rfc2606.html";
    //Проблема: OpenSSL не потокобезопасен по умолчанию!
    TEST.AddInitialUrl(testurl);

    TEST.Work(testurl, 1);
    /*
        1. DelHead() - опасное удаление по шаблону
    cpp
    response.erase(response.find("<head>"), response.find("</head>"));
        Если <head> или </head> не найдены, find() вернет string::npos


        Это приводит к переполнению буфера или неопределенному поведению
    */ //Якоря не принимать
    //httpclient.download("www.rfc-editor.org", "443", "/rfc/rfc2606.html");
    // ПРОВЕРЯТЬ HTML ли это также когда корневая ссылка таргету ставить значение "/"

    std::this_thread::sleep_for(std::chrono::seconds(50));
}
catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
}
catch(...) {
    std::cerr << "Exception of unknown exception" << std::endl;
}
}
