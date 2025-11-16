#pragma once
#include "httpclient.h"
#include "threadpool.h"


class Crowler
{
private:
httpclient* client;
ThreadPool* threadPool;

};