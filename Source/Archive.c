#include "Archive.h"
#include "ArgParse.h"
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Protection.h"
#include "Defines.h"

static void compressInit(z_stream *stream, const Arguments* arguments) {
    stream->zalloc = Z_NULL;
    stream->zfree = Z_NULL;
    stream->opaque = Z_NULL;

    // Initialize the compressor with default settings.
    if (deflateInit(stream, arguments->compression_level) != Z_OK) {
        if(!arguments->silent)
            perror("Could not initialize the compressor\n");
        exit(1);
    }
}


int compressBlock(const Arguments* arguments, z_stream* strm, unsigned char *in, size_t inSize, unsigned char *out, size_t outSize, size_t *produced, int flush) {
    static int initialized = 0;
    if (!initialized) {
        compressInit(strm, arguments);
        initialized = 1;
    }
    int ret;

    strm->avail_in = inSize;
    strm->next_in = in;
    strm->avail_out = outSize;
    strm->next_out = out;

    ret = deflate(strm, flush);
    if (ret == Z_STREAM_ERROR) {
        return ret;
    }
    if(flush == Z_FINISH) {
        deflateEnd(strm);
        initialized = 0;
    }

    *produced = outSize - strm->avail_out;

    return ret;
}

#include <stdlib.h>

int decompress_buffer_dynamic(unsigned char *in, size_t in_length, unsigned char **out, size_t *out_length) {
    z_stream strm;
    int ret;
    size_t buffer_size = *out_length;
    unsigned char *buffer = (unsigned char*)malloc(buffer_size);

    if (!buffer) {
        return -1; // Ошибка выделения памяти
    }

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = in_length;
    strm.next_in = in;
    strm.avail_out = buffer_size;
    strm.next_out = buffer;

    int abacaba = 0;
    IS_TRAP_FLAG_SET(abacaba);
    if(abacaba) {
        DEBUG("Trap flag is set\n");
        exit(0);
    }

    if (inflateInit(&strm) != Z_OK) {
        free(buffer);
        return -1;
    }

    do {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            (void)inflateEnd(&strm);
            free(buffer);
            return -1;
        }

        if (strm.avail_out == 0) {
            buffer_size *= 2; // Удвоение размера буфера
            unsigned char *new_buffer = (unsigned char*)realloc(buffer, buffer_size);
            if (!new_buffer) {
                (void)inflateEnd(&strm);
                free(buffer);
                return -1;
            }
            buffer = new_buffer;
            strm.next_out = buffer + strm.total_out;
            strm.avail_out = buffer_size - strm.total_out;
        }
    } while (ret != Z_STREAM_END);

    (void)inflateEnd(&strm);

    if (ret != Z_STREAM_END) {
        free(buffer);
        return -1;
    }

    *out = buffer;
    *out_length = strm.total_out;
    return 0;
}



