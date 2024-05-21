#include <argp.h>
#include <zlib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Defines.h"
#include "ArgParse.h"

const char *argp_program_version = "nzip (Nano ZIP) 0.1";
const char *argp_program_bug_address = "andshvets@yandex.ru";
static char doc[] = "Simple program for compressing files with basic ZIP algorithm.";
static char args_doc[] = "filename [ARCHIVE.zip]";

static struct argp_option options[] = {
        {"bs", 'b', "SIZE", OPTION_ARG_OPTIONAL, "Block size (e.g., 1K, 16K, 5M). bs=4K by default. bs must be greater than 4K and divisible by 4K.", 0},
        {"silent", 's', 0, 0, "Verbose output", 0},
        {"compression", 'c', "LEVEL", OPTION_ARG_OPTIONAL, "Compression level (0-9) 0 means NO_COMPRESSION, 9 - MAX_COMPRESSION.", 0},
        {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'b':
            if (!arg)
                argp_usage(state);
            arguments->block_size = arg;
            break;
        case 's':
            arguments->silent = true;
            break;
        case 'c':
            if (!arg)
                argp_usage(state);
            arguments->compression_level = atoi(arg);
            if (arguments->compression_level < 0 || arguments->compression_level > 9) {
                if(!arguments->silent)
                    perror("Compression level must be between 0 and 9\n");
                exit(1);
            }
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 2)
                argp_usage(state);
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1) // Must at least have one filename argument
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static void adjust_arguments(struct arguments *arguments) {
    if (arguments->block_size) {
        char *block_size = malloc(strlen(arguments->block_size) + 1);
        char* mem_block_size = block_size;
        strcpy(block_size, arguments->block_size);
        if(block_size[0] == '=') {
            block_size++;
        }
        if(block_size[strlen(block_size) - 1] == 'K') {
            block_size[strlen(block_size) - 1] = '\0';
            arguments->block_size_int = atoi(block_size) * 1024;
        } else if(block_size[strlen(block_size) - 1] == 'M') {
            block_size[strlen(block_size) - 1] = '\0';
            arguments->block_size_int = atoi(block_size) * 1024 * 1024;
        } else {
            arguments->block_size_int = atoi(block_size);
        }
        free(mem_block_size);

        if(arguments->block_size_int < 4096) {
            if(!arguments->silent)
                perror("Block size must be at least than 4K\n");
            exit(1);
        }
        if(arguments->block_size_int % 4096 != 0) {
            if(!arguments->silent)
                perror("Block size must be divisible by 4K\n");
            exit(1);
        }
    }
    else{
        arguments->block_size_int = 4096;
    }
    //If no archive name is provided, use the default one
    if (!arguments->args[1]) {
        arguments->args[1] = "archive.zip";
    }

    if(!arguments->compression_level) {
        arguments->compression_level = Z_DEFAULT_COMPRESSION;
    }
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

void parse_args(int argc, char **argv, struct arguments *arguments) {
    arguments->block_size = NULL;
    
    argp_parse(&argp, argc, argv, 0, 0, arguments);
    adjust_arguments(arguments);

    DEBUG("FILENAME: %s\n", arguments->args[0]);
    if (arguments->args[1]) {
        DEBUG("ARCHIVE: %s\n", arguments->args[1])
    }
    if (arguments->block_size) {
        DEBUG("Block size: %d\n", arguments->block_size_int)
    }
    if (arguments->silent) {
        DEBUG("Silence errors enabled\n")
    }
}