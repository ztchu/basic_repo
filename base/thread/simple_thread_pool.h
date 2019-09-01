#pragma once

#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

typedef std::function<void()> Task;

class SimpleThreadPool {
public:
    SimpleThreadPool(unsigned int number_of_threads_);
    ~SimpleThreadPool();
    void CommitTask(Task&& task);

    void StartThreadPool();
    void CloseThreadPool();

private:
    void Run();
    bool GetTask(Task& task);

private:
    std::vector<std::thread> thread_pool_;
    std::queue<Task> tasks_;
    std::condition_variable cond_;
    std::mutex mutex_;

    std::atomic<bool> is_running_;
    std::atomic<bool> is_task_queue_empty_;
};