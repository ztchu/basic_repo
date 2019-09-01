#pragma once

#if defined(ENABLE_TEST)
#include "thread_pool_test.h"
#endif

namespace test_all {
void Test();

#if defined(ENABLE_TEST)
void TestThreadPool();
#endif

}
