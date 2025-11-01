#pragma once
#include <thread>
#include <functional>
#include "safe_queue/SafeQueue.h"
#include <atomic>

class ThreadPool
{
private:
SafeQueue<std::function<void()>> queue_;
std::vector<std::thread> Thr;
std::atomic<bool> stop_flag_{false};

public:

    ThreadPool(size_t numThreads = std::thread::hardware_concurrency());

    void work();

    void submit(std::function<void()> func);

    ~ThreadPool();
};