#pragma once
#include "httpclient.h"
#include "Indexer.h"
#include "threadpool.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <atomic>

class Crowler
{
private:
httpclient& connects;
indexer& indexer_;
ThreadPool& threadPool;
std::unordered_set<std::string> visited_urls;
std::mutex visited_mutex;
std::mutex indexer_mutex;
std::atomic<int> tasks_count{ 0 };
std::string StartPage{};

int maxDeep;


public: //FOR TEST THIS PRIVATE

void Work(ParsedUrl url, int deep); // прокидывается в пулл потоков. Скачивает и Индеексирует страницу


void AddWork(std::string page, ParsedUrl BasicUrl, int deep);

std::string Crowler::MakeUrlKey( ParsedUrl url);

bool TryAddUrl(std::string url_key);

void StartCrowler();

    public:

    void AddInitialUrl(const ParsedUrl& url);

    Crowler(httpclient& client, indexer& idx, ThreadPool& pool, std::string StartPage, int maxDeep);

    void WaitUntilDone();
};

