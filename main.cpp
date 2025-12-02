#include <iostream>
#include "Crowler.h"
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include  <thread>

int main()
{ //Добавить проверку visited URLs

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
    ThreadPool pool;

    Crowler TEST(httpclient, indexer_, pool,2);

    ParsedUrl testurl;
    testurl.host = "www.rfc-editor.org";
    testurl.port = "443";
    testurl.target = "/rfc/rfc2606.html";

    TEST.Work(testurl, 1);


    //std::string test = httpclient.download("www.iana.org", "443", "/help/example-domains");

    //test = indexer_.DelHTML(test);
    //test = indexer_.RefactorText(test);
    //indexer_.SeparateWords(test);

    //std::cout<< test << std::endl;

    //indexer_.GetHrefs(test);

}
