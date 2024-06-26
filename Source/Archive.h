#pragma once
#include <zlib.h>
#include "ArgParse.h"

int compressBlock(const Arguments* arguments, z_stream* strm, unsigned char *in, size_t inSize, unsigned char *out, size_t outSize, size_t *produced, int flush);
int decompress_buffer_dynamic(unsigned char *in, size_t in_length, unsigned char **out, size_t *out_length);