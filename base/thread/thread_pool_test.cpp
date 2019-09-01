#include "thread_pool_test.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "Utility.h"

ThreadPoolTest::ThreadPoolTest() {
    thread_pool_.reset(new SimpleThreadPool(4));
}

void ThreadPoolTest::ThreadPoolTest::DoworkConcurrent() {
    int64_t start_time = Utility::GetMilliSecTimestamp();
    for (int i = 0; i < 10; ++i) {
        thread_pool_->CommitTask(std::bind(&ThreadPoolTest::SumNumber, this, 10000));
    }
    std::cout << "The answer: " << ans_;
    std::cout << "Total time in milli seconds: " << Utility::GetMilliSecTimestamp()
        - start_time << std::endl;
}

void ThreadPoolTest::DoworkSequential() {
    int64_t start_time = Utility::GetMilliSecTimestamp();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10000; ++j) {
            ans_ += i * 10000 + j;
        }
    }
    std::cout << "The answer: " << ans_;
    std::cout << "Total time in milli seconds: " << Utility::GetMilliSecTimestamp()
        - start_time << std::endl;
}

void ThreadPoolTest::SumNumber(int num) {
    std::cout << "SumNumber, thread number: " << std::this_thread::get_id();
    long long ans = 0;
    for (int i = 0; i < num; ++i) {
        ans += i;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ans_ += ans;
    }
}

// void ThreadPoolTest::PrintStr(int num) {
//     std::cout << "PrintStr, thread number: " << std::this_thread::get_id();
//     std::this_thread::sleep_for(std::chrono::seconds(2));
//     /*for (int i = 0; i < num; ++i) {
//         for (char a = 'a'; a <= 'z'; ++a) {
//             std::cout << a << " ";
//         }
//     }*/
// }