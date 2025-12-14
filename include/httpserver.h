#pragma once
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "DataBase.h"
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

// httpserver принимает запросы и создаёт обьекты сессии. сессии читают запросы отправляют ответы и тд



class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, ssl::context& ctx, std::shared_ptr<DataBase> db);

    void run();

private:
    ssl::stream<tcp::socket> stream_;
    beast::flat_buffer buffer_{8192};
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
    std::shared_ptr<DataBase> db;

    void read_request();

    void handle_request();

    void send_search_form();

    void handle_search();

    void send_not_found();

    void send_response();

    //
    std::string parse_query(const std::string& body);

    std::string perform_search(const std::string& query);

};

class http_server
{
public:
    http_server(net::io_context& ioc, unsigned short port,
                const std::string& cert_file, const std::string& key_file, std::shared_ptr<DataBase> db);

private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    ssl::context ctx_;
    std::shared_ptr<DataBase> db;

    void do_accept();

};