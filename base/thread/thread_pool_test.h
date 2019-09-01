#include "simple_thread_pool.h"
#include "thread_pool.h"

#include <memory>
#include <mutex>

class ThreadPoolTest {
public:
    ThreadPoolTest();
    void DoworkSequential(int num);

    void TestConcurrent(int number);

private:
    long long SumNumber(int start, int end);

private:
    std::unique_ptr<SimpleThreadPool> thread_pool_;
    std::unique_ptr<ThreadPool> pool_;
    long long ans_ = 0;
    std::mutex mutex_;
    std::vector<std::future<long long>> ans_futures_;
};