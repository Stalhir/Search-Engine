#pragma once
#include  <queue>
#include <mutex>
#include <condition_variable>

template<typename Var>
class SafeQueue
{
private:
    std::queue<Var> _queue_;

    std::mutex mut;

    std::condition_variable cv;

    bool _stop_flag_ = false;
public:

    void S_push(Var task)
    {
        std::lock_guard<std::mutex> lock(mut);
        _queue_.push(task);
        cv.notify_one();
    }

    bool S_pop(Var& task)
    {
        std::unique_lock<std::mutex> lock(mut);
        cv.wait(lock, [this]() { return !_queue_.empty() || _stop_flag_;});

        if (_queue_.empty() && _stop_flag_)
            {
            return false;
        }

        task = std::move(_queue_.front());
        _queue_.pop();
        return true;
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock(mut);
        _stop_flag_ = true;
        cv.notify_all();
    }
};