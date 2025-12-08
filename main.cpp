#include <iostream>
#include "Crowler.h"
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include "initializer.h"
#include  <thread>

int main()
{ //Добавить обработку ошибок http download переписать под async boost beast скорее всего ошибка так исчезнет
try
{
    //HMODULE ssl = LoadLibraryA("libssl-3-x64.dll");
    //HMODULE crypto = LoadLibraryA("libcrypto-3-x64.dll");

    /*
    if(!ssl || !crypto) {
        std::cout << "OpenSSL DLL not found!" << std::endl;
        return 1;
    }
    std::cout << "OpenSSL DLL loaded successfully!" << std::endl;
*/

    std::cout<< "Main Thread id: "<< std::this_thread::get_id() << std::endl;

    InitSearchEngine init("C:\\Users\\askoy\\Desktop\\search engine\\setting.ini");

    //init.SetForDB();
    //Проблема 100 процентов в pqxx
    httpclient httpclient;
    indexer indexer_(init.SetForDB());

    indexer_.database.InitDB();

    ThreadPool pool(1);

    Crowler TEST(httpclient, indexer_, pool,3);



    ParsedUrl testurl;
    testurl.host = "www.rfc-editor.org";
    testurl.port = "443";
    testurl.target = "/rfc/rfc2606.html";

    httpclient.download(testurl.host, testurl.port, testurl.target);
    //OpenSSL не потокобезопасен
    //TEST.AddInitialUrl(testurl);

    TEST.Work(testurl, 1);

    testurl.host = "www.wikipedia.org";
    testurl.port = "443";
    testurl.target = "/";

    //TEST.Work(testurl, 1);



    std::this_thread::sleep_for(std::chrono::seconds(20));

}
catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
}
catch(...) {
    std::cerr << "Exception of unknown exception" << std::endl;
}
}
