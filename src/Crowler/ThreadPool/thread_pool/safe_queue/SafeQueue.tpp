#include "SafeQueue.h"

/*
template<typename Var>
void SafeQueue<Var>::S_push(Var task)
{
    std::lock_guard<std::mutex> lock(mut);
    _queue_.push(task);
    cv.notify_one();
}

template<typename Var>
void SafeQueue<Var>::S_pop()
{

    std::unique_lock<std::mutex> lock(mut);
    cv.wait(lock, [this]() {
        return !_queue_.empty() || _stop_flag_;
    });

    if (_queue_.empty() && _stop_flag_) {
        return ;
    }

    _queue_.pop();

}
/*
template<typename Var>
void SafeQueue<Var>::stop()
{
    std::lock_guard<std::mutex> lock(mut);
    _stop_flag_ = true;
    cv.notify_all();
}
*\