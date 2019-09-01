#include "all_test.h"

namespace test_all {
void Test() {
#if defined(ENABLE_TEST)
    TestThreadPool();
#endif
}

#if defined(ENABLE_TEST)
void TestThreadPool() {
    ThreadPoolTest thread_pool_test;
    thread_pool_test.TestConcurrent(1000000000);
    thread_pool_test.DoworkSequential(1000000000);
}
#endif
}