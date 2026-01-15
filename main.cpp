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

    InitSearchEngine init(R"(C:\Users\askoy\Documents\PROJECT\Search-Engine\Search-Engine\setting.ini)");// path to setting.ini

      httpclient httpclient;
    indexer indexer_(init.SetForDB());

    indexer_.database.InitDB();

    ThreadPool pool(16);

    Crowler TEST(init.SetForCrowler(httpclient, indexer_, pool ));

    TEST.StartCrowler();


   
//http://httpbin.org/relative-redirect/3
    



    std::shared_ptr<DataBase> db_ptr =
        std::make_shared<DataBase>(init.SetForDB());

    auto port = static_cast<unsigned short>(443);
    std::string cert_file = R"(C:\Users\askoy\Documents\PROJECT\Search-Engine\Search-Engine\opensslsertificate\server.crt)";
    std::string key_file = R"(C:\Users\askoy\Documents\PROJECT\Search-Engine\Search-Engine\opensslsertificate\server.key)";

    net::io_context ioc;
    //http_server server(ioc, port, cert_file, key_file, db_ptr);
    SearchEngine server_(init.SetForSE(ioc, cert_file, key_file, db_ptr));
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
