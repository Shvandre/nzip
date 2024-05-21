cmake_minimum_required(VERSION 3.22)
project(zip C)

enable_language(C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG_MODE -Wall -Wextra")

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#add all the files to the project
file(GLOB SOURCE_FILES "Source/*")
add_executable(zip zip.c ${SOURCE_FILES})
target_link_libraries(zip z)

add_executable(unzip unzip.c ${SOURCE_FILES})
target_link_libraries(unzip z)

add_library(code STATIC ${SOURCE_FILES})

# Тесты
add_executable(runTests Tests.cpp)
target_link_libraries(runTests gtest gtest_main code)