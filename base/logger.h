#pragma once

#if defined(ENABLE_GLOG)

#include "glog/logging.h"

#define LOG_INFO LOG(INFO)
#define LOG_WARN LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

#else

#include <iostream>

#define LOG_INFO std::cout 
#define LOG_WARN std::cerr 
#define LOG_ERROR std::cerr 

#endif
