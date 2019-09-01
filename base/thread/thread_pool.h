#pragma once

// Come from https://github.com/progschj/ThreadPool.git

#include <condition_variable>
#include <future>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool {
public:
    ThreadPool(size_t num);

    template<typename Func, typename ...Args>
    auto Enqueue(Func&& func, Args&& ... args)->
        std::future<typename std::result_of<Func(Args...)>::type>;
    
    size_t GetThreadsNumber() const;
    ~ThreadPool();

private:
    std::vector<std::thread> pool_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool stop_;
    size_t number_of_threads_;
};

inline ThreadPool::ThreadPool(size_t num): stop_(false) {
    number_of_threads_ = num < std::thread::hardware_concurrency()
        ? num : std::thread::hardware_concurrency();
    for (size_t i = 0; i < number_of_threads_; ++i) {
        pool_.emplace_back([this](){
            for (;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cond_.wait(lock, [this]{return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

template<typename Func, typename ...Args>
auto ThreadPool::Enqueue(Func&& func, Args&& ... args)->
    std::future<typename std::result_of<Func(Args...)>::type> {
    using result_type = typename std::result_of<Func(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<result_type()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    auto ret = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (stop_)
            throw std::runtime_error("Enquque on stopped threadpool.");

        tasks_.emplace([task]{(*task)();});
    }
    cond_.notify_one();
    return ret;
}

inline size_t ThreadPool::GetThreadsNumber() const {
    return number_of_threads_;
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cond_.notify_all();

    for (auto & thread : pool_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}