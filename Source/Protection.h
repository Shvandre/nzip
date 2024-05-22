#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <sys/ptrace.h>

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

#define IS_DEBUGGER_PRESENT(result) \
    do { \
        result = 0; \
        if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1) { \
            result = 1; \
        } else { \
            ptrace(PTRACE_DETACH, 0, NULL, 0); \
        } \
    } while (0)

#define TIME_CHECK_BEGIN() \
    clock_t start##__LINE__ = clock();

#define TIME_CHECK_END(time) \
    if((double)(clock() - start##__LINE__) / CLOCKS_PER_SEC > time) \
        exit(111); //Но лучше, конечно, передать управление куда-нибудь подальше в junk-код