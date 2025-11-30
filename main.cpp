#include <iostream>
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
#include "Indexer.h"
#include  <thread>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
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


    std::string test = httpclient.download("www.iana.org", "443", "/help/example-domains");

    test = indexer_.DelHTML(test);
    test = indexer_.RefactorText(test);

    std::cout<< test << std::endl;

    //indexer_.GetHrefs(test);

}
