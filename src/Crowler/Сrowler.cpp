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
    std::vector<std::string> urls = indexer_.GetHrefs(page);

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

            threadPool.submit(std::bind(&Crowler::Work, this, parsed_url, deep+1));
            std::cout<< "Addwork push okey" << std::endl;
        }
        std::cout<< "Addwork okey" << std::endl;
}


void Crowler::Work(ParsedUrl url, int deep)
{

    std::string key = MakeUrlKey(url);
    if (!TryAddUrl(key) && deep != 1) {
        std::cout << "Already visited: " << url.host << url.target << std::endl;
        return;
    }
    else if (deep == 1)
    {
    AddInitialUrl(url);
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (deep > maxDeep) {
        std::cout << "Max deep reached: " << deep << std::endl;
        return;
    }
std::cout<< "download start" << deep << std::endl;
    std::string responce = connects.download(url.host, url.port, url.target);
    std::cout<< "download work?" << deep << std::endl;
    if (!responce.empty()) {
        std::cout << "URL: " << url.target << std::endl;
        indexer_.Index(responce, url);
std::cout<< "Index Yes?" << deep << std::endl;

        if (deep < maxDeep) {
            AddWork(responce, url, deep);
        }
    }
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