cmake_minimum_required (VERSION 3.0)

project(basic_project)

# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

option(ENABLE_GLOG "Enable glog" ON)

if(ENABLE_GLOG)
    add_definitions(-DENABLE_GLOG)
    if(WIN32)
        add_definitions("-DGLOG_NO_ABBREVIATED_SEVERITIES")
        add_definitions("-DGOOGLE_GLOG_DLL_DECL=")
    endif()
endif()

list(APPEND SRCS ./src/main.cpp)
message("SRCS:" ${SRCS})

list(APPEND SRCS 
    ./Utils/logger.h
    ./Utils/Utility.cpp
    ./Utils/Utility.h)

include_directories(./src/)
include_directories(./Utils/)

include_directories(./libs/glog/include)
if(WIN32)
    find_library(GLOG_LIB NAMES glog.lib PATHS ./libs/glog/lib)
else()
endif()

set(EXTERNAL_LIBS ${GLOG_LIB})
message("EXTERNAL_LIBS: " ${EXTERNAL_LIBS})

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBS})