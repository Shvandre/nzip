#include "FileMapping.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

long long map_file(const char* filename, size_t size, unsigned char** dest) { //Works like strtok. Warning: This function is not thread-safe.
    static int fd = -1;                // File descriptor
    static size_t current_offset = 0;  // Current offset in the file
    static size_t file_size = 0;       // File size

    if (filename != NULL) {
        if (fd != -1) {
            close(fd); // Close the previously opened file
        }
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file");
            return -1;
        }
        // Get the file size
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            perror("Error getting the file size");
            close(fd);
            fd = -1;
            return -1;
        }
        file_size = sb.st_size;
        current_offset = 0; // Reset the offset
    }

    if (fd == -1) {
        perror("No file is currently open\n");
        return -1;
    }

    if (current_offset >= file_size) {
        perror("Meeted EOF\n");
        return -1;
    }

    // Calculate the size of the mapping (the last mapping may be smaller)
    size_t mapping_size = (current_offset + size > file_size) ? (file_size - current_offset) : size;

    // Actually map the file
    *dest = mmap(NULL, mapping_size, PROT_READ, MAP_PRIVATE, fd, (long)current_offset);
    if (*dest == MAP_FAILED) {
        perror("Error mapping the file");
        return -1;
    }

    // Update the offset
    current_offset += mapping_size;

    return (long long)mapping_size;
}
