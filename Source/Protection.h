#pragma once
#include <stdint.h>
#include <stdlib.h>

uint32_t mycrc32(const char *filename);

extern volatile unsigned char hash_sum[];
extern unsigned* hash_sum_ptr;

#define CHECK_CRC() \
    if(mycrc32(0) != *hash_sum_ptr) { \
        printf("CRC32 mismatch\n, expected %u, got %u\n", mycrc32(0), *hash_sum_ptr); \
        exit(1); \
    }



#define IS_TRAP_FLAG_SET(result) \
    __asm__ __volatile__ ( \
        "pushfq\n"                    /* Сохранить флаги регистра EFLAGS на стеке */ \
        "pop %%rax\n"                /* Поместить значение EFLAGS в регистр RAX */ \
        "and $0x100, %%eax\n"        /* Применить маску для извлечения 8-го бита (TF) */ \
        "shrl $8, %%eax\n"           /* Сдвинуть результат вправо на 8 битов */ \
        "mov %%eax, %0\n"            /* Поместить результат в переменную result */ \
        : "=r" (result)              /* Выход: result получает значение из EAX */ \
        :                            /* Нет входных параметров */ \
        : "rax"                      /* RAX испортится после выполнения */ \
    )