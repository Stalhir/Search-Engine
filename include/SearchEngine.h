#pragma once
#include "httpserver.h"
#include "DataBase.h"

class SearchEngine
{
private:
    net::io_context ioc;
    unsigned short port = 10322;
    std::string cert_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.crt";
    std::string key_file = "C:\\Users\\askoy\\Desktop\\search engine\\opensslsertificate\\server.key";
    std::shared_ptr<DataBase> db;
    http_server server_{ioc, port, cert_file, key_file,db};
    // добавить инитиализацию из класса initializer. добавить запрос из базы данных
    /*
    SELECT P.url, SUM(PW.count) AS total_word_count
    FROM Pages P
    JOIN Page_Word PW ON P.id = PW.page_id
    JOIN Words W ON W.id = PW.word_id
    WHERE W.word IN ('база', 'данных', 'запрос') -- <--- Замените на список ваших слов
    GROUP BY P.id, P.url
    HAVING COUNT(DISTINCT W.word) = 3 -- <--- Замените на количество слов в списке
    ORDER BY total_word_count DESC;
    */

};

