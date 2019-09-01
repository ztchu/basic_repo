cmake_minimum_required (VERSION 3.0)

project(basic_project)

# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

option(ENABLE_GLOG "Enable glog" OFF)
option(ENABLE_TEST "Enable test" OFF)
option(ENABLE_THREAD_POOL "Enable thread pool" ON)

if(ENABLE_TEST)
    add_definitions(-DENABLE_TEST)
endif()

if(ENABLE_GLOG)
    add_definitions(-DENABLE_GLOG)
    if(WIN32)
        add_definitions("-DGLOG_NO_ABBREVIATED_SEVERITIES")
        add_definitions("-DGOOGLE_GLOG_DLL_DECL=")
    endif()
endif()

list(APPEND SRCS 
    ./src/main.cpp
    ./src/all_test.cpp
    ./src/all_test.h)

list(APPEND SRCS 
    ./base/logger.h
    ./base/utility.cpp
    ./base/utility.h)

if(ENABLE_THREAD_POOL)
    list(APPEND SRCS
        ./base/thread/simple_thread_pool.cpp
        ./base/thread/simple_thread_pool.h)
    if(ENABLE_TEST)
        list(APPEND SRCS
            ./base/thread/thread_pool_test.cpp
            ./base/thread/thread_pool_test.h)
    endif()
endif()

message("SRCS:" ${SRCS})

include_directories(./src/)
include_directories(./base/)
include_directories(./base/thread)

include_directories(./libs/glog/include)
if(ENABLE_GLOG)
    if(WIN32)
        find_library(GLOG_LIB NAMES glog.lib PATHS ./libs/glog/lib)
    else()
    endif()
    set(EXTERNAL_LIBS ${GLOG_LIB})
endif()

message("EXTERNAL_LIBS: " ${EXTERNAL_LIBS})

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBS})