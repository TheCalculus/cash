#ifndef CASH_H
#define CASH_H

#include <stdio.h>

#define CASH_THROW(error)       \
    do {                        \
        fprintf(stderr, error); \
        exit(EXIT_FAILURE);     \
    } while (0);

#define MAX_ARGS 10
#define ARG_SIZE 100
#define NUM_CMDS 3

/* viability of character for argument */
#define NOT_VIABLE_CHAR(ch) (!isprint(input[i]) || isspace(input[i]))
#define VIABLE_CHAR(ch)     (!isspace(input[i]) && isprint(input[i]))

typedef struct
command_struct {
    char*  command;
    size_t (*func)(size_t, char**); // size_t argc, char** argv
} command;

void     cash_main();
void     user_prompt();
char*    cash_input(size_t* buffsiz, ssize_t* charsiz);
char**   cash_parse(char* input, size_t buffsiz, size_t* argc_out);
command* get_builtin(char* input);
size_t   cash_execv(char** command, size_t argc);

extern command commands[NUM_CMDS];

#endif
