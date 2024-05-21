#include <stdio.h>
#include "Source/ArgParse.h"
#include "Source/Tools.h"
int main(int argc, char **argv)
{
    Arguments arguments = {0};
    parse_args(argc, argv, &arguments);
    int ret = compressFile(&arguments);
    if(ret != 0) {
        fprintf(stderr, "Error occurred\n");
        return 1;
    }
    return 0;
}