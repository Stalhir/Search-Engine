#pragma once
#include <string>
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;



class httpclient : public std::enable_shared_from_this<httpclient>// помогает shader_ptr вернуть указатель на самого себя
{
private:
    boost::asio::ssl::context ctx{asio::ssl::context::tls_client};
    //asio::io_context ioc;

    static std::once_flag open_ssl_init_flag_;
    static void InitializeOpenSSL();

public:
    httpclient();

    std::string download(std::string host,std::string port, std::string target);

    std::string download_http( http::request<http::string_body> req, http::response<http::string_body> resp,
        beast::flat_buffer buffer, asio::ip::basic_resolver_results<asio::ip::tcp> resolver);

    std::string download_https( http::request<http::string_body> req, http::response<http::string_body> resp,
        beast::flat_buffer buffer, asio::ip::basic_resolver_results<asio::ip::tcp> resolver, std::string host);

    ~httpclient();
private:


};


