#include <iostream>

#include "logger.h"
#include "Utility.h"


int main(int argc, char** argv) {
    Utility::InitializeLogger();
    LOG_INFO << "hello world." << std::endl;
}