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
    std::call_once(open_ssl_init_flag_, InitializeOpenSSL);


    ctx.set_default_verify_paths();
    ctx.set_verify_mode(asio::ssl::verify_none);
}

std::once_flag httpclient::open_ssl_init_flag_;

void httpclient::InitializeOpenSSL() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

std::string httpclient::download_http(http::request<http::string_body> req,
    http::response<http::string_body> resp, beast::flat_buffer buffer,asio::ip::basic_resolver_results<asio::ip::tcp> resolver)
{
    asio::io_context ioc;
    beast::tcp_stream stream(ioc);

    stream.expires_after(std::chrono::seconds(30));

    stream.connect(resolver);

    http::write(stream, req);
    http::read(stream, buffer, resp);

    stream.close();

    return resp.body();
}

std::string httpclient::download_https( http::request<http::string_body> req,
    http::response<http::string_body> resp, beast::flat_buffer buffer,asio::ip::basic_resolver_results<asio::ip::tcp> resolver, std::string host )
{
    asio::io_context ioc;
    asio::ssl::stream<beast::tcp_stream> stream(ioc,ctx);

    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));

    beast::get_lowest_layer(stream).connect(resolver);

    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
        throw std::exception("SSL_set_tlsext_host_name error");
    }

    stream.handshake(asio::ssl::stream_base::client);

    http::write(stream, req);
    http::read(stream, buffer, resp);

    beast::get_lowest_layer(stream).close();

return resp.body();
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

        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::response<http::string_body> resp;

        req.method(http::verb::get);
        req.version(11);
        req.set(http::field::host, host);
        req.target(target);
        req.set(http::field::user_agent, "HTTPCLIENT_CROWLER");

        boost::asio::ip::basic_resolver_results<asio::ip::tcp> resolver = resolver_.resolve(host, port);

        if (port == "80") {
            return download_http(req,resp, buffer,resolver);
        }
        else if(port == "443")
        {
            return download_https(req, resp, buffer,resolver, host);
        }
        else {
            throw std::exception("invalid port");
        }

    }
    catch (beast::error_code& ec) {
        std::cerr << ec.message() << std::endl;
        return "";
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}


httpclient::~httpclient()
{
    
}