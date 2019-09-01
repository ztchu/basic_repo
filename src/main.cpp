#include <iostream>

#include "all_test.h"
#include "logger.h"
#include "utility.h"




int main(int argc, char** argv) {
    Utility::InitializeLogger();
    LOG_INFO << "hello world." << std::endl;

    test_all::Test();
}