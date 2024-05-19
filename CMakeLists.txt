cmake_minimum_required(VERSION 3.22)
project(nzip C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG_MODE")
#add all the files to the project
set(SOURCE_FILES main.c Defines.h ArgParse.c ArgParse.h FileMapping.c FileMapping.h Archive.c Archive.h)
add_executable(nzip ${SOURCE_FILES})
target_link_libraries(nzip z)
