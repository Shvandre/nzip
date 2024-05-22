#include "Protection.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "Defines.h"
#include "errno.h"

volatile unsigned char hash_sum[] = {0xAA, 0xBB, 0xCC, 0xDD}; //0xAA, 0xBB ... is a signature, so that we will find it later
unsigned* hash_sum_ptr = (unsigned*)hash_sum;

// Maybe this should be more complicated, but for now it's enough
uint32_t mycrc32(const char *filename) {
    static char command[256];
    char buffer[256];
    unsigned long checksum = 0;

    if(filename != 0) {
        printf("%s\n", filename);
        snprintf(command, sizeof(command), "cksum %s", filename);
        command[strlen(filename) + 6] = '\0';
        printf("%s\n", command);
        fflush(stdout);

    }
    // Открываем pipe для выполнения команды
    FILE* p = popen(command, "r");
    if (p == NULL) {
        perror("popen failed");
        return 0;
    }
    char* strerr = strerror(errno);
    printf("%s", strerr);
    fflush(stdout);
    printf("%d", fileno(p));
    return 0;
    // Читаем вывод команды
    if (read(fileno(p), buffer, sizeof(buffer)) != NULL) {
        // Парсим вывод команды, чтобы извлечь контрольную сумму
        sscanf(buffer, "%lu", &checksum);
    }

    // Закрываем pipe
    pclose(pipe);

    return checksum;
}