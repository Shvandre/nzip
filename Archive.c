#include "Archive.h"
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void compressInit(z_stream *stream) {
    stream->zalloc = Z_NULL;
    stream->zfree = Z_NULL;
    stream->opaque = Z_NULL;

    // Initialize the compressor with default settings.
    if (deflateInit(stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        perror("Could not initialize the compressor\n");
        exit(1);
    }
}


int compressBlock(z_stream* strm, unsigned char *in, size_t inSize, unsigned char *out, size_t outSize, size_t *produced, int flush) {
    static int initialized = 0;
    if (!initialized) {
        compressInit(strm);
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
