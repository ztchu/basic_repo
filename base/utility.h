#pragma once

#include <chrono>
#include <string>

#include "logger.h"

class Utility
{
public:
    static bool PathExists(const std::string& path);
    static std::string BackSlash2ForwardSlash(const std::string& path);
    static bool InitializeLogger(const std::string& path = "/log");
    static int64_t GetMilliSecTimestamp();

private:
    static bool CheckFolderExist(const std::string& path);
};

