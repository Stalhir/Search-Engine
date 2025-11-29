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
    boost::asio::ssl::context ctx{asio::ssl::context::tlsv12_client};
    asio::io_context& ioc;

public:
    httpclient(asio::io_context& ioc);

    std::string download(std::string host,std::string port, std::string target);

    ~httpclient();
private:


};


