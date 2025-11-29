#pragma once
#include "httpclient.h"
#include "Indexer.h"
#include "threadpool.h"
#include <string>

class Crowler
{
private:
httpclient* client;
ThreadPool* threadPool;

void Work(std::string url, int deep); // прокидывается в пулл потоков. Скачивает и Индеексирует страницу
};