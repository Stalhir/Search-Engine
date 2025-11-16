#include "threadpool.h"


void ThreadPool::work()
{
    while (!stop_flag_) {
        std::function<void()> task;
        if (queue_.S_pop(task)) {
            task();
        }
    }
}

void ThreadPool::submit(std::function<void()> func)
{
    queue_.S_push(func);
}

ThreadPool::ThreadPool(size_t numThreads)
{
    for (int i = 0; i < numThreads; i++)
    {
        Thr.emplace_back([this]{this->work();});
    }
}

ThreadPool::~ThreadPool()
{
    stop_flag_ = true;
    queue_.stop();

    for (auto& worker : Thr) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

