#include "simple_thread_pool.h"

#include <memory>
#include <mutex>

class ThreadPoolTest {
public:
    ThreadPoolTest();
    void DoworkConcurrent();
    void DoworkSequential();

private:
    void SumNumber(int num);
    static void PrintStr(int num);

private:
    std::unique_ptr<SimpleThreadPool> thread_pool_;
    long long ans_ = 0;
    std::mutex mutex_;
};