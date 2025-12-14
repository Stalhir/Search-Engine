#include <iostream>
#include "Crowler.h"
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include "initializer.h"
#include "httpserver.h"
#include  <thread>

int main()
{ //ƒобавить обработку ошибок http download переписать под async boost beast скорее всего ошибка так исчезнет
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
    //ѕроблема 100 процентов в pqxx
    httpclient httpclient;
    indexer indexer_(init.SetForDB());

    indexer_.database.InitDB();

    ThreadPool pool(16);

    Crowler TEST(httpclient, indexer_, pool,3);



    ParsedUrl testurl;
    testurl.host = "www.rfc-editor.org";
    testurl.port = "443";
    testurl.target = "/rfc/rfc2606.html";

    //httpclient.download(testurl.host, testurl.port, testurl.target);
    //OpenSSL не потокобезопасен
    //TEST.AddInitialUrl(testurl);

    TEST.Work(testurl, 1);

    testurl.host = "www.wikipedia.org";
    testurl.port = "443";
    testurl.target = "/";

    //TEST.Work(testurl, 1);

    DataBase  test(init.SetForDB());
    std::shared_ptr<DataBase> db_ptr =
        std::make_shared<DataBase>(init.SetForDB());

    auto port = static_cast<unsigned short>(10322);
    std::string cert_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.crt";
    std::string key_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.key";

    net::io_context ioc;
    http_server server(ioc, port, cert_file, key_file, db_ptr);

    std::cout << "HTTPS сервер запущен на порту " << port << std::endl;
    ioc.run();


    std::this_thread::sleep_for(std::chrono::seconds(20));

}
catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
}
catch(...) {
    std::cerr << "Exception of unknown exception" << std::endl;
}
}
