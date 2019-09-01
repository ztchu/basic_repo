#include "thread_pool_test.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "Utility.h"

ThreadPoolTest::ThreadPoolTest() {
    thread_pool_.reset(new SimpleThreadPool(4));
    pool_.reset(new ThreadPool(4));
}

void ThreadPoolTest::DoworkSequential(int num) {
    int64_t start_time = Utility::GetMilliSecTimestamp();
    ans_ = SumNumber(0, num);
    std::cout << "The answer: " << ans_;
    std::cout << "Sequential total time in milli seconds: " << Utility::GetMilliSecTimestamp()
        - start_time << std::endl;
}

long long ThreadPoolTest::SumNumber(int start, int end) {
    std::cout << "SumNumber, thread number: " << std::this_thread::get_id();
    long long ans = 0;
    for (int i = start; i < end; ++i) {
        ans += i;
    }
    return ans;
}

void ThreadPoolTest::TestConcurrent(int number) {
    int64_t start_time = Utility::GetMilliSecTimestamp();
    size_t total_threads = pool_->GetThreadsNumber();
    size_t offset = number / total_threads;
    for (size_t i = 0; i < total_threads; ++i) {
        ans_futures_.emplace_back(pool_->Enqueue(&ThreadPoolTest::SumNumber, this,
            i * offset, ( i + 1) * offset < number ? (i + 1) * offset : number));
    }

    for (int i = 0; i < ans_futures_.size(); ++i) {
        ans_ += ans_futures_[i].get();
    }
    std::cout << "ans: " << ans_ << std::endl;
    std::cout << "Concurrent total time in milli seconds: " << Utility::GetMilliSecTimestamp()
        - start_time << std::endl;
}
