#include "httpserver.h"
#include "DataBase.h"

http_server::http_server(net::io_context& ioc, unsigned short port,
            const std::string& cert_file, const std::string& key_file)
    : ioc_(ioc)
    , acceptor_(ioc, tcp::endpoint(tcp::v4(), port))
    , ctx_(ssl::context::tlsv12)
{
    ctx_.set_options(
        ssl::context::default_workarounds |
        ssl::context::no_sslv2 |
        ssl::context::single_dh_use);

    ctx_.use_certificate_chain_file(cert_file);
    ctx_.use_private_key_file(key_file, ssl::context::pem);

    do_accept();
}

void http_server::do_accept()
{
    acceptor_.async_accept(
        [this](beast::error_code ec, tcp::socket socket)
        {
            if(!ec)
            {
                // Создаем сессию для нового соединения
                std::make_shared<session>(std::move(socket), ctx_)->run();
            }

            // Принимаем следующее соединение
            do_accept();
        });
}

session::session(tcp::socket socket, ssl::context& ctx)
    : stream_(std::move(socket), ctx)
{
}

void session::run()
{
    stream_.async_handshake(
        ssl::stream_base::server,
        [self = shared_from_this()](beast::error_code ec)
        {
            if(!ec)
                self->read_request();
        });
}

   void session::read_request()
    {
        request_ = {};
        buffer_.consume(buffer_.size());

        http::async_read(
            stream_,
            buffer_,
            request_,
            [self = shared_from_this()](beast::error_code ec, std::size_t)
            {
                if(!ec)
                    self->handle_request();
            });
    }

    void session::handle_request()
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        auto target = request_.target();
        auto method = request_.method();

        if(method == http::verb::get && target == "/")
        {
            send_search_form();
        }
        // POST запрос - обработка поиска
        else if(method == http::verb::post && target == "/search")
        {
            handle_search();
        }
        else
        {
            send_not_found();
        }
    }

    void session::send_search_form()
    {
        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Search</title>
    <meta charset="utf-8">
</head>
<body>
    <h1>Search</h1>
    <form method="POST" action="/search">
        <input type="text" name="query" placeholder="Write request...">
        <button type="submit">your request</button>
    </form>
</body>
</html>
)";

        response_.result(http::status::ok);
        response_.set(http::field::server, "Search Engine Server");
        response_.set(http::field::content_type, "text/html; charset=utf-8");
       beast::ostream(response_.body()) << html;
        response_.prepare_payload();

        send_response();
    }

    void session::handle_search()
    {
        // Извлекаем тело POST запроса
        std::string body = beast::buffers_to_string(request_.body().data());

        // Парсим параметр query (упрощенно)
        std::string query = parse_query(body);

        // Выполняем поиск (ваша логика здесь)
        std::string results = perform_search(query);

        // Формируем HTML с результатами
        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Result</title>
    <meta charset="utf-8">
</head>
<body>
    <h1>Result for: )" + query + R"(</h1>
    <ul>
)" + results + R"(
    </ul>
    <a href="/">New search</a>
</body>
</html>
)";

        response_.result(http::status::ok);
        response_.set(http::field::content_type, "text/html; charset=utf-8");
        beast::ostream(response_.body()) << html;
        response_.prepare_payload();

        send_response();
    }

    void session::send_not_found()
    {
        response_.result(http::status::not_found);
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body()) << "404 Not Found";
        response_.prepare_payload();

        send_response();
    }

    void session::send_response()
    {
        auto self = shared_from_this();
        http::async_write(
            stream_,
            response_,
            [self](beast::error_code ec, std::size_t)
            {
                // После отправки закрываем SSL соединение
                self->stream_.async_shutdown(
                    [self](beast::error_code ec)
                    {

                    });
            });
    }



    std::string session::parse_query(const std::string& body)
    {
        size_t pos = body.find("query=");
        if(pos != std::string::npos)
        {
            std::string query = body.substr(pos + 6);
            // Обрезаем до следующего &
            size_t end = query.find('&');
            if(end != std::string::npos)
                query = query.substr(0, end);

            // Заменяем + на пробелы (базовое декодирование)
            std::replace(query.begin(), query.end(), '+', ' ');
            return query;
        }
        return "";
    }

std::string session::perform_search(const std::string& query)
{

    if(query.empty())
        return "<li>Write request</li>";

    // Пример статических результатов
    return "<li>Result 1: " + query + "</li>\n" +
           "<li>Result 2: " + query + "</li>\n" +
           "<li>Result 3: " + query + "</li>";
}