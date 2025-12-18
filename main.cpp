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
{
try
{
    

    std::cout<< "Main Thread id: "<< std::this_thread::get_id() << std::endl;

    InitSearchEngine init("C:\\Users\\askoy\\Desktop\\search engine\\setting.ini");

      httpclient httpclient;
    indexer indexer_(init.SetForDB());

    indexer_.database.InitDB();

    ThreadPool pool(16);

    Crowler TEST(httpclient, indexer_, pool,3);


    ParsedUrl testurl;
    testurl.host = "www.rfc-editor.org";
    testurl.port = "443";
    testurl.target = "/rfc/rfc2606.html";

   

    
    testurl.host = "www.wikipedia.org";
    testurl.port = "443";
    testurl.target = "/";
    //TEST.Work(testurl, 1);
    TEST.Work(testurl, 1);

    std::shared_ptr<DataBase> db_ptr =
        std::make_shared<DataBase>(init.SetForDB());

    auto port = static_cast<unsigned short>(10322);
    std::string cert_file = R"(C:\Users\askoy\Desktop\search engine\opensslsertificate\server.crt)";
    std::string key_file = R"(C:\Users\askoy\Desktop\search engine\opensslsertificate\server.key)";

    net::io_context ioc;
    http_server server(ioc, port, cert_file, key_file, db_ptr);
    //TEST.WaitUntilDone();
    std::cout << "HTTPS server work on port: " << port << std::endl;
    ioc.run();


    //std::this_thread::sleep_for(std::chrono::seconds(20));

}
catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
}
catch(...) {
    std::cerr << "Exception of unknown exception" << std::endl;
}
}
