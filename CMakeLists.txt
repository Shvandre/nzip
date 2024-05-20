cmake_minimum_required(VERSION 3.22)
project(zip C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG_MODE -Wall -Wextra")
#add all the files to the project
file(GLOB SOURCE_FILES "Source/*")
add_executable(zip zip.c ${SOURCE_FILES})
target_link_libraries(zip z)

add_executable(unzip unzip.c ${SOURCE_FILES})
target_link_libraries(unzip z)