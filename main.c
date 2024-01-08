#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CASH_THROW(error)       \
    do {                        \
        fprintf(stderr, error); \
        exit(EXIT_FAILURE);     \
    } while (0);

void user_prompt() {
    printf("cash >> "); 
}

char** cash_parse(char* input, size_t buffsiz) {
    char** arguments = (char**)malloc(sizeof(char*) * 10);
    size_t argc      = 0;

    for (int i = 0; i < buffsiz; i++) {
        char ch = input[i];
        while (isspace(ch)) ch = input[++i];

        arguments[argc] = (char*)malloc(100); // a maximum single token size of 100
                                              // should be more than enough

        size_t j = 0;

        while (!isspace(ch)) { 
            arguments[argc][j++] = ch;
            ch = input[++i];
        }

        arguments[argc] = (char*)realloc(arguments[argc], j + 1);
        argc++;
    }

    /* TODO: remove this free */
    for (int i = 0; i < argc - 1; i++) {
        printf("$%d = %s\n", i, arguments[i]);
        free(arguments[i]);
    }

    return arguments;
}

size_t cash_execv(char** command) {
    return -1;
}

char* cash_input(size_t* buffsiz) {
    char* input;

    if(getline(&input, buffsiz, stdin) == -1)
        CASH_THROW("getline");

    return input;
}

void cash_main() {
    int RUNNING = 1;

    char*  input;
    char** command;
    size_t retval;
    size_t buffsiz = 0;

    do {
        user_prompt();

        input   = cash_input(&buffsiz);
        command = cash_parse(input, buffsiz);
        retval  = cash_execv(command);

        free(input);
        free(command);

        RUNNING--;
    } while (RUNNING);
}

int main() {
    cash_main();
    return 0;
}
