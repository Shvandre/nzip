#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "Defines.h"
#include "ArgParse.h"
#include "Archive.h"
#include "FileMapping.h"

int compressFile(const Arguments* arguments) {
    int result = 0;
    IS_TRAP_FLAG_SET(result);
    if(result) {
        DEBUG("Trap flag is set\n");
        exit(0); // В реальном коде лучше будет передать управление куда-нибудь подальше в junk-код
    }

    FILE *archive = fopen(arguments->args[1], "wb");
    if (archive == NULL) {
        perror("Error opening the archive");
        exit(1);
    }
    unsigned char *out;
    xmalloc(out, arguments->block_size_int);

    unsigned char *next_block;
    long long mapped_size;

    z_stream strm;
    mapped_size = map_file(arguments->args[0], arguments->block_size_int, &next_block);
    while(mapped_size != -1) {
        DEBUG("Mapped size: %lld\n", mapped_size);

        size_t produced_size = 0;
        int ret = compressBlock(arguments, &strm, next_block, mapped_size, out, arguments->block_size_int, &produced_size, Z_NO_FLUSH);
        if(ret != Z_OK) {
            perror("Error compressing the block");
            return 1;
        }

        fwrite(out, 1, produced_size, archive);
        // Очистка маппинга
        if (munmap(next_block, mapped_size) == -1) {
            perror("Error un-mapping the file");
            return 1; //But this is not a fatal error
        }
        mapped_size = map_file(0, arguments->block_size_int, &next_block);
    }
    size_t prod_size = 0;
    compressBlock(arguments, &strm, NULL, 0, out, arguments->block_size_int, &prod_size, Z_FINISH); // Finalize the compression
    fwrite(out, 1, prod_size, archive);
    fclose(archive);
    free(out);
    return 0;
}

int uncompressFile(const char* filename, const char* output_filename) {
    FILE *archive = fopen(filename, "rb");
    if (archive == NULL) {
        perror("Error opening the archive");
        return 1;
    }
    struct stat sb;
    if (fstat(fileno(archive), &sb) == -1) {
        perror("Could not get file size");
        fclose(archive);
        return 1;
    }
    size_t length = sb.st_size;
    FILE *output = fopen(output_filename, "wb");
    if (output == NULL) {
        perror("Error opening the output file");
        return 1;
    }
    //map archive
    char* in = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fileno(archive), 0);
    if (in == MAP_FAILED) {
        perror("Error mapping the archive");
        fclose(archive);
        fclose(output);
        return 1;
    }
    unsigned char *out;
    size_t out_length = length;
    if (decompress_buffer_dynamic((unsigned char*)in, length, &out, &out_length) == -1) {
        perror("Error decompressing the archive");
        munmap(in, length);
        fclose(archive);
        fclose(output);
        free(out);
        return 1;
    }
    fwrite(out, 1, out_length, output);

    munmap(in, length);
    free(out);
    fclose(archive);
    fclose(output);
    return 0;
}