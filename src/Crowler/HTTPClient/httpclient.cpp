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

httpclient::httpclient()
{
    asio::ssl::context ctx(asio::ssl::context::tls_client);
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(asio::ssl::verify_peer);
}

std::string httpclient::download_http(std::string host, std::string target)
{
    return target;
}

std::string httpclient::download_https(std::string host, std::string target)
{
return target;
}

std::string httpclient::download(std::string host,std::string port, std::string target)
{
    //Настроить отлов ошибок надо
    //Разбить на функции
    //ДОбавить чтоб при HTTP он не делал шифрование
    try {
        beast::error_code ec;
        asio::io_context ioc;
        asio::ip::tcp::resolver resolver_(ioc);//днс ресольвер
        //std::lock_guard<std::mutex> lock(mutex);
        /* Ошибка «handshake: длина пакета слишком длинная (рутины SSL)» [asio.ssl:167772358] может возникать,
         * когда сервер настроен для обработки HTTP-запросов через порт 443, а не протокола HTTPS*/

        static thread_local asio::ssl::context tls_ctx(asio::ssl::context::tls_client);

            tls_ctx.set_default_verify_paths();
            tls_ctx.set_verify_mode(asio::ssl::verify_none);
            SSL_CTX_set_max_cert_list(tls_ctx.native_handle(), 100 * 1024);

            // Инициализация OpenSSL для этого потока
            SSL_library_init();
            OpenSSL_add_all_algorithms();
            SSL_load_error_strings();



        asio::ssl::stream<beast::tcp_stream> stream(ioc,tls_ctx);
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

        std::cout<<"All okey" << std::endl;
        return resp.body();
    }
    catch (beast::error_code& ec) {
        std::cerr << ec.message() << std::endl;
        return "";
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


httpclient::~httpclient()
{
    
}