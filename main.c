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

#define MAX_ARGS 10
#define ARG_SIZE 100

void user_prompt() {
    printf("cash >> "); 
}

/* viability of character for argument */
#define NOT_VIABLE_CHAR(ch) (!isprint(input[i]) || isspace(input[i]))
#define VIABLE_CHAR(ch)     (!isspace(input[i]) && isprint(input[i]))

char** cash_parse(char* input, size_t buffsiz) {
    char** arguments = (char**)malloc(sizeof(char*) * MAX_ARGS);
    size_t argc = 0, i = 0;

    while (i < buffsiz && argc < MAX_ARGS) {
        while (i < buffsiz && NOT_VIABLE_CHAR(input[i])) i++;

        arguments[argc] = (char*)malloc(ARG_SIZE);

        size_t j = 0;
        while (i <= buffsiz && j < ARG_SIZE &&
               VIABLE_CHAR(input[i]))
        {
            /* assignment to ch might be cleaner overall */
            arguments[argc][j++] = input[i++];
        }
        
        arguments[argc][j] = '\0';
        argc++;
    }

    arguments[argc] = NULL;

    /* TODO: remove this free */
    for (size_t k = 0; k < argc; k++) {
        printf("$%zu = %s\n", k, arguments[k]);
        free(arguments[k]);
    }

    return arguments;
}

size_t cash_execv(char** command) {
    return -1;
}

char* cash_input(size_t* buffsiz, ssize_t* charsiz) {
    char*   input = NULL;
    ssize_t chars = 0;

    if ((chars = getline(&input, buffsiz, stdin)) == -1) {
        free(input);
        CASH_THROW("getline");
    }

    if (input[chars - 1] == '\n') {
        input[chars - 1] = '\0';
        chars--;
    }

    *charsiz = chars;
    
    return input;
}

size_t cash(size_t argc, char** argv) {
    printf("$\n");
    return 0;
}

struct command_struct {
    char*  command;
    size_t (*func)(size_t, char**); // size_t argc, char** argv
};

struct command_struct
commands[] = {
    { "cash", &cash },
};

struct command_struct*
get_builtin(char* input) {
    for (int i = 0; i < sizeof(commands) / sizeof(struct command_struct); i++) {
        struct command_struct* curr = commands + i;
        if (strcmp(curr->command, input) == 0) return curr;
    }

    printf("match for command not found\n");
    return NULL;
}

void cash_main() {
    int RUNNING = 1;

    char*   input;
    char**  command;

    size_t  buffsiz, retval = 0;
    ssize_t charsiz;

    while (RUNNING) {
        user_prompt();

        input   = cash_input(&buffsiz, &charsiz);
        command = cash_parse(input, --charsiz);
        retval  = cash_execv(command);

        struct command_struct* cmd = get_builtin(input);

        if (cmd != NULL)
            cmd->func(0, command);

        free(input);
        free(command);
    }
}

int main() {
    cash_main();
    return 0;
}
