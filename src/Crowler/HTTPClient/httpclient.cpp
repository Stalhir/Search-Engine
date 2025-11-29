#include "httpclient.h"
#include <string>
#include <chrono>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;

httpclient::httpclient(asio::io_context& ioc) : ioc(ioc)
{

}



std::string httpclient::download(std::string host,std::string port, std::string target)
{
    //Настроить отлов ошибок надо
    //Разбить на функции
    //ДОбавить чтоб при HTTP он не делал шифрование
    try {
        beast::error_code ec;
        asio::ip::tcp::resolver resolver_(ioc);//днс ресольвер
        asio::ssl::stream<beast::tcp_stream> stream(ioc,ctx);
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::response<http::string_body> resp;

        beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));//timeout

        auto result = resolver_.resolve(host, port);


        req.method(http::verb::get);
        req.version(11);
        req.set(http::field::host, host);
        req.target(target);
        req.set(http::field::user_agent, "HTTPCLIENT_CROWLER");

        beast::get_lowest_layer(stream).connect(result);// тут подключение типо
        std::cout<<"connect okey" << std::endl;

        if(!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            ec = beast::error_code(static_cast<int>(::ERR_get_error()),
                                 asio::error::get_ssl_category());
            std::cerr << "SNI error: " << ec.message() << std::endl;
        }
        std::cout<<"SNI okey" << std::endl;

        stream.handshake(asio::ssl::stream_base::client);
        std::cout<<"handshake okey" << std::endl;

        http::write(stream, req);


        http::read(stream, buffer, resp);

        beast::get_lowest_layer(stream).close();

        return resp.body();
    }
    catch (beast::error_code& ec) {
        std::cerr << ec.message() << std::endl;
    }
}

httpclient::~httpclient()
{
    
}