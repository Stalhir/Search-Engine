#pragma once
#include "httpclient.h"
#include "Indexer.h"
#include "threadpool.h"
#include <string>
#include <vector>

class Crowler
{
private:
httpclient& connects;
indexer& indexer_;
ThreadPool& threadPool;

void Work(std::vector<std::string> url, int deep); // прокидывается в пулл потоков. Скачивает и Индеексирует страницу

    public:

    Crowler(httpclient& client, indexer& idx, ThreadPool& pool);

};