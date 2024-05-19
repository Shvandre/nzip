#include <stdio.h>
#include <zlib.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "ArgParse.h"
#include "Defines.h"
#include "FileMapping.h"
#include "Archive.h"

int main(int argc, char **argv) {

    Arguments arguments;
    parse_args(argc, argv, &arguments);
    unsigned char *out;
    xmalloc(out, arguments.block_size_int);
    size_t cur_buf_size = arguments.block_size_int;

    char *next_block;
    size_t mapped_size;
    size_t total_produced_size = 0;

    z_stream strm;
    mapped_size = map_file(arguments.args[0], arguments.block_size_int, &next_block);
    while(mapped_size != -1) {
        DEBUG("Mapped size: %ld\n", mapped_size);

        int flush = Z_NO_FLUSH;
        if(mapped_size < arguments.block_size_int) {
            DEBUG("Last block\n");
            flush = Z_FINISH;
        }
        size_t produced_size = 0;
        int ret = compressBlock(&strm, next_block, mapped_size, out, cur_buf_size - total_produced_size, &produced_size, flush);
        if(ret != Z_OK) {
            perror("Error compressing the block");
            exit(1);
        }
        total_produced_size += produced_size;
        if(total_produced_size > cur_buf_size / 2) {
            xrealloc(out, cur_buf_size + arguments.block_size_int * 2); // Выделяем два блока сразу, чтобы не слишком часто делать realloc
        }

        // Очистка маппинга
        if (munmap(next_block, mapped_size) == -1) {
            perror("Error un-mapping the file");
        }
        map_file(0, arguments.block_size_int, &next_block);
    }

    // Write the compressed data to the archive
    FILE *archive = fopen(arguments.args[1], "wb");
    if (archive == NULL) {
        perror("Error opening the archive");
        exit(1);
    }
    fwrite(out, 1, total_produced_size, archive);
    fclose(archive);
    return 0;
}