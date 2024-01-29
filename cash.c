#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cash.h"

size_t cash_execv(char** command, size_t argc) {
    pid_t  pid = fork();
    size_t retval;

    if (pid == -1) CASH_THROW("fork");
    
    if (pid == 0) {
        char* prefix = "/usr/bin/";
        char* result = (char*)malloc(strlen(prefix) + strlen(command[0]) + 1);

        strcpy(result, prefix);
        strcat(result, command[0]);

        command = (char**)realloc(command, (argc + 2) * sizeof(char*));
        command[argc] = NULL;

        execvp(result, command);

        free(result);
    } else {
        waitpid(pid, &retval, 0);
        retval = WEXITSTATUS(retval);
        // child process complete
    }
    
    return retval;
}

struct command_struct*
get_builtin(char* input) {
    for (int i = 0; i < NUM_CMDS; i++) {
        struct command_struct* curr = commands + i;
        if (strcmp(curr->command, input) == 0) return curr;
    }

    return NULL;
}

char** cash_parse(char* input, size_t buffsiz, size_t* argc_out) {
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
    *argc_out = argc;

    return arguments;
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


void user_prompt() {
    printf("root@cash:/cash$ "); 
}

void cash_main() {
    int RUNNING = 1;

    char*   input;
    char**  command;

    size_t  buffsiz, retval, argc = 0;
    ssize_t charsiz;

    while (RUNNING) {
        user_prompt();

        input   = cash_input(&buffsiz, &charsiz);
        command = cash_parse(input, --charsiz, &argc);

        struct command_struct* cmd = get_builtin(command[0]);

        if (cmd != NULL) {
            cmd->func(0, command);
            goto clean;
        }

        size_t retval = cash_execv(command, argc);
        
        if (errno = ENOENT) {
            // command not found by execvp
            printf("command not found: %s\n", command[0]);
        }

        fflush(stdout);

clean:
        for (size_t k = 0; k < argc; k++) {
//          printf("$%zu = %s\n", k, command[k]);
            free(command[k]);
        }

        free(input);
        free(command);
    }
}

int main() {
    signal(SIGINT, SIG_IGN);
    cash_main();

    return 0;
}
