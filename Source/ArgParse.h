#pragma once
#include <stdbool.h>

typedef struct arguments {
    char *args[2];   // filename and ARCHIVE.zip
    char *block_size;
    bool verbose;
    int block_size_int;
    int compression_level;
} Arguments;

void parse_args(int argc, char **argv, struct arguments *arguments);