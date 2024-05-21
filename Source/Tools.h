#pragma once
#include "ArgParse.h"

int compressFile(const Arguments* arguments);
int uncompressFile(const char* filename, const char* output_filename);