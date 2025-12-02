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
// Добавить проверку visited URLs
int maxDeep; // Work выполняет работу. в нём для ссылок
public: //FOR TEST THIS PRIVATE
void Work(ParsedUrl url, int deep); // прокидывается в пулл потоков. Скачивает и Индеексирует страницу


void AddWork(std::string page, ParsedUrl BasicUrl, int deep);

    public:

    Crowler(httpclient& client, indexer& idx, ThreadPool& pool, int maxDeep);


};

