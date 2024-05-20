cmake_minimum_required(VERSION 3.22)
project(nzip C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG_MODE")
#add all the files to the project
set(SOURCE_FILES Defines.h ArgParse.c ArgParse.h FileMapping.c FileMapping.h Archive.c Archive.h
        Tools.c
        Tools.h)
add_executable(zip zip.c ${SOURCE_FILES})
target_link_libraries(zip z)

add_executable(unzip unzip.c ${SOURCE_FILES})
target_link_libraries(unzip z)