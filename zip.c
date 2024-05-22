#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Source/ArgParse.h"
#include "Source/Tools.h"
#include "Source/Protection.h"
#include <unistd.h>
#include <time.h>
int main(int argc, char **argv)
{
    TIME_CHECK_BEGIN();
    Arguments arguments = {0};
    parse_args(argc, argv, &arguments);
    TIME_CHECK_END(2);
    int ret = compressFile(&arguments);
    if(ret != 0) {
        fprintf(stderr, "Error occurred\n");
        return 1;
    }
    return 0;
}
