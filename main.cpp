#include <iostream>
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include  <thread>

int main()
{

    HMODULE ssl = LoadLibraryA("libssl-3-x64.dll");
    HMODULE crypto = LoadLibraryA("libcrypto-3-x64.dll");

    if(!ssl || !crypto) {
        std::cout << "OpenSSL DLL not found!" << std::endl;
        return 1;
    }
    std::cout << "OpenSSL DLL loaded successfully!" << std::endl;

    boost::asio::io_context ioc;

    httpclient httpclient(ioc);
    indexer indexer_;
    /*
    std::thread t3([&httpclient]() {
    httpclient.download("github.com", "443", "/olipramarcin/taskflow/blob/main/LICENSE");
    });
    t3.join();
    */

    std::string test = httpclient.download("en.wikipedia.org", "443", "/wiki/Main_Page");

    std::cout<< test << std::endl;

    indexer_.GetHrefs(test);

}
