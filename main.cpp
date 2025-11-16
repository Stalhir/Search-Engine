#include <iostream>
#include <pqxx/pqxx>
#include <boost/beast.hpp>
#include "include/httpclient.h"
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
    //httpclient.download("123","","");
    std::thread t1([&httpclient]() {
        httpclient.download("habr.com", "443", "/ru/articles/227575/");
    });

    std::thread t2([&httpclient]() {
    httpclient.download("portal.librus.pl", "443", "/rodzina");
    });
    std::thread t3([&httpclient]() {
    httpclient.download("github.com", "443", "/olipramarcin/taskflow/blob/main/LICENSE");
    });


    t1.join();
    t2.join();
    t3.join();
}
