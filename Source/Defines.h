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

#define IS_TRAP_FLAG_SET(result) \
    __asm__ __volatile__ ( \
        "pushfq\n"                    /* Сохранить флаги регистра EFLAGS на стеке */ \
        "pop %%rax\n"                /* Поместить значение EFLAGS в регистр RAX */ \
        "and $0x100, %%eax\n"        /* Применить маску для извлечения 8-го бита (TF) */ \
        "shrl $8, %%eax\n"           /* Сдвинуть результат вправо на 8 битов */ \
        "mov %%eax, %0\n"            /* Поместить результат в переменную result */ \
        : "=r" (result)              /* Выход: result получает значение из EAX */ \
        :                            /* Нет входных параметров */ \
        : "rax"                      /* EAX испортится после выполнения */ \
    )