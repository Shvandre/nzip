#pragma once

#ifdef DEBUG_MODE
#define DEBUG(s, ...) printf(s, ##__VA_ARGS__);
#else
#define DEBUG(s, ...)
#endif

#ifdef DEBUG_MODE
#define xmalloc(ptr, size) if((ptr = malloc(size)) == NULL) { \
    fprintf(stderr, "Error allocating memory in file %s at line %d\n", __FILE__, __LINE__); \
    exit(1); \
}
#else
#define xmalloc(ptr, size) if((ptr = malloc(size)) == NULL) { \
    perror("Error allocating memory"); \
    exit(1); \
}
#endif

#ifdef DEBUG_MODE
#define xrealloc(ptr, size) if((ptr = realloc(ptr, size)) == NULL) { \
    fprintf(stderr, "Error allocating memory in file %s at line %d\n", __FILE__, __LINE__); \
    exit(1); \
}
#else
#define xrealloc(ptr, size) if((ptr = realloc(ptr, size)) == NULL) { \
    perrot("Error reallocating memory") \
    exit(1); \
}
#endif

