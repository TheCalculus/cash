#include <stdio.h>
#include <stdlib.h>

#include "cash.h"

size_t cash(size_t argc, char** argv) {
    printf("casinos cash, v0.0.1-(x86_64-pc-linux-gnu)\n");
    printf("$:<https://github.com/TheCalculus/cash>\n"); 

    return 0;
}

size_t hello(size_t argc, char** argv) {
    printf("world\n");

    return 0;
}

size_t cash_exit(size_t argc, char** argv) {
    exit(0);
}

command commands[NUM_CMDS] = {
    { "cash",  &cash },
    { "hello", &hello },
    { "exit",  &cash_exit },
};
