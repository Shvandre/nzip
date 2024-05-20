#include <stdio.h>
#include "Source/ArgParse.h"
#include "Source/Tools.h"
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 2) {
        fprintf(stderr, "Usage: %s <Archive.zip> <filename>\n", argv[0]);
        return 1;
    }
    char*archive = argv[1];
    char*filename;
    if(argc == 2) {
        printf("No filename provided, defaulting to 'output' \n");
        filename = "output";
    } else {
        filename = argv[2];
    }
    uncompressFile(archive, filename);
    return 0;
}