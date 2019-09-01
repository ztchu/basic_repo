#include "simple_thread_pool.h"

#include <algorithm>

#include "logger.h"

SimpleThreadPool::SimpleThreadPool(unsigned int number_of_threads)
    :is_running_(true), is_task_queue_empty_(true) {
    try {
        number_of_threads_ = std::max(number_of_threads,
            std::thread::hardware_concurrency());
        for (int i = 0; i < number_of_threads_; ++i) {
            thread_pool_.push_back(std::thread(&SimpleThreadPool::Run, this));
        }
    } catch(...) {
        is_running_.store(false);
    }
}

SimpleThreadPool::~SimpleThreadPool() {
    is_running_.store(false);
    for (auto& thread : thread_pool_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void SimpleThreadPool::CommitTask(Task&& task) {
    if (!is_running_) {
        LOG_WARN << "Commit failed, the thread pool has been stopped.";
        return;
    }

    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]{return !!is_running_;});
    tasks_.emplace(task);
    is_task_queue_empty_.store(tasks_.empty());
    cond_.notify_one();
}

void SimpleThreadPool::StartThreadPool() {
    is_running_.store(true);
}

void SimpleThreadPool::CloseThreadPool() {
    is_running_.store(false);
}

size_t SimpleThreadPool::GetThreadsNumber() const {
    return number_of_threads_;
}

void SimpleThreadPool::Run() {
    while (is_running_ || !is_task_queue_empty_) {
        Task task;
        if (GetTask(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
}

bool SimpleThreadPool::GetTask(Task& task) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this](){ return !is_task_queue_empty_ || !is_running_; });
    if (!tasks_.empty()) {
        task = std::move(tasks_.front());
        tasks_.pop();
        is_task_queue_empty_.store(tasks_.empty());
        return true;
    }
    return false;
}