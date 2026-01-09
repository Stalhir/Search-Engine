#include  "Crowler.h"
#include  <iostream>
#include <future>

Crowler::Crowler(httpclient& client, indexer& idx, ThreadPool& pool, int maxDeep = 1)
: connects(client), indexer_(idx), threadPool(pool), maxDeep(maxDeep)
{
    visited_urls.reserve(10000);
}

void Crowler::AddWork(std::string page, ParsedUrl BasicUrl, int deep)
{
    std::vector<std::string> urls;
    {
        std::lock_guard<std::mutex> lock(indexer_mutex);
        urls = indexer_.GetHrefs(page);
    }
    if (deep >= maxDeep) {
        return;
    }

        for (std::string url : urls)
        {
            ParsedUrl parsed_url = indexer_.ParsingURL(url);
            parsed_url = indexer_.FixURL(BasicUrl, parsed_url);

            if (!indexer_.CheckUrl(parsed_url)) {
                continue;
            }

            std::string key = MakeUrlKey(parsed_url);
            if (!TryAddUrl(key)) {
                std::cout << "Already visited: " << parsed_url.host << parsed_url.target << std::endl;
                continue;
            }

            tasks_count++;

            threadPool.submit(std::bind(&Crowler::Work, this, parsed_url, deep+1));
            std::cout<< "URL push: " <<parsed_url.port + parsed_url.host + parsed_url.target << std::endl;
        }
        std::cout<< "Addwork okey" << std::endl;
}


void Crowler::Work(ParsedUrl url, int deep)
{

    std::string key = MakeUrlKey(url);

    if (deep == 1)
    {
    AddInitialUrl(url);
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (deep > maxDeep) {
        std::cout << "Max deep reached: " << deep << std::endl;
        return;
    }

    auto responce = connects.download(url.host, url.port, url.target);

    if (connects.checkRedirect(responce)) {//проверка на редирект
        auto it = responce.find(http::field::location);
        if (it != responce.end()) {
            std::string redirect(it->value());
            ParsedUrl parsed_url = indexer_.ParsingURL(redirect);
            parsed_url = indexer_.FixURL(url, parsed_url);

            if (!indexer_.CheckUrl(parsed_url)) {
                tasks_count--;
                return;
            }

            std::string key = MakeUrlKey(parsed_url);
            if (!TryAddUrl(key))
            {
                tasks_count--;
                return;
            }
            Work(parsed_url, deep);
            std::cout << "REDIRECT WORK" << std::endl;
            return;
        }
    }


    //responce. тут берём сыллку и парсим её
    //ParsingURL()
    //FixURL()
    //CheckUrl()
    if (!responce.body().empty()) {
        indexer_.Index(responce.body(), url);

        if (deep < maxDeep) {
            AddWork(responce.body(), url, deep);
        }
    }
    tasks_count--;
}

std::string Crowler::MakeUrlKey(ParsedUrl url)
{
    return url.host + ":" + url.port + url.target;
}

bool  Crowler::TryAddUrl(std::string url_key)
{
    std::lock_guard<std::mutex> lock(visited_mutex);

    auto it = visited_urls.find(url_key);
    if (it != visited_urls.end()) {
        return false;
    }


    visited_urls.insert(url_key);
    return true;
}

void Crowler::AddInitialUrl(const ParsedUrl& url) {
    std::string key = MakeUrlKey(url);
    TryAddUrl(key);
}

void Crowler::WaitUntilDone() {
    std::cout << "Waiting for crawler to finish..." << std::endl;


    while (tasks_count > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Crawler finished work!" << std::endl;
}