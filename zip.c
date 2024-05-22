#include <stdio.h>
#include <unistd.h>
#include "Source/ArgParse.h"
#include "Source/Tools.h"
#include "Source/Protection.h"
int main(int argc, char **argv)
{
    Arguments arguments = {0};
    parse_args(argc, argv, &arguments);

    char binary_path[1024];
    size_t sz = readlink("/proc/self/exe", binary_path, sizeof(binary_path) - 1);
    binary_path[sz] = '\0';
    unsigned crc = mycrc32(binary_path);
    printf("CRC32: %u\n", crc);
    CHECK_CRC();
    int ret = compressFile(&arguments);
    if(ret != 0) {
        fprintf(stderr, "Error occurred\n");
        return 1;
    }
    return 0;
}