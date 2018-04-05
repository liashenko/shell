/**
 * 
 *  shell
 *      author: Andrii Liashenko 
 *              ndrlyashenko@gmail.com
 *  shell has such builtins:
 *      help
 *      cd
 *      exit
 *  shell also supports linux commands as external executables.
 * 
 *  @TODO:
 *      ctrl-c should not kill the shell, but kill currently executing program if any executes
 *      quoting or backslash escaping
 *      globbing
 *      piping
 * 
 */
#include "shell.h"

#define PROMPT_TEXT "shell > "
char* read_input() {
    char *input = readline(PROMPT_TEXT);
    add_history(input);
    return input;
}

#define TOKENS_DELIMETERS " "
#define TOKENS_BUFFER_SIZE 64
char** parse_input(char* input) {
    if (input == NULL) {
        return NULL;
    }
    int capacity = TOKENS_BUFFER_SIZE;
    char **tokens = malloc(capacity * sizeof(char*));
    if (tokens == NULL) {
        printf("shell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    int size = 0;
    char *token = strtok(input, TOKENS_DELIMETERS);
    while (token) {
        if (size >= capacity) {
            printf("shell: command is too long\n");
            free(token);
            free(tokens);
            return NULL;
        }
        tokens[size++] = token;
        token = strtok(NULL, TOKENS_DELIMETERS);
    }
    tokens[size] = NULL;
    free(token);
    return tokens;
}

/**
 * Builtins
 */
int shell_exit(char** argv) {
    return 0;
}
int shell_help(char** argv) {
    printf("\tshell\n");
    printf("\tauthor: Andrii Liashenko\n");
    printf("\t\tndrlyashenko@gmail.com\n\n");
    printf("\tshell has such builtins:\n");
    printf("\t\thelp\n");
    printf("\t\tcd\n");
    printf("\t\texit\n");
    printf("\tshell also supports linux commands as external executables.\n");
    return 1;
}
int shell_cd(char** argv) {
    if (argv == NULL || argv[0] == NULL || argv[1] == NULL) {
        return 1;
    }
    if (chdir(argv[1])) {
        printf("shell: %s: No such file or directory\n", argv[1]);   
        return 1;
    }
    return 1;
}

int execute(char** argv) {
    if (argv == NULL || argv[0] == NULL) {
        return 1;
    }
    // check builtins
    int i = 0;
    while (builtins[i]) {
        if (!strcmp(argv[0], builtins[i]->name)) {
            return builtins[i]->func(argv);
        }
        ++i;
    }
    // execute external executable program
    return execute_command(argv);
}
// execute external executable
int execute_command(char** argv) {
    if (argv == NULL) {
        printf("shell: command not found\n");
        return 1;
    }
    int status;
    pid_t pid = fork();
    if (pid < 0) {
        printf("shell: forking child process failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // child
        if (execvp(argv[0], argv) < 0) {
            printf("shell: %s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }
     } else {
        // parent
        while (wait(&status) != pid);
     }
     return 1;
}

int main() {
    char *input = NULL;
    char **argv = NULL;
    int status = 1;
    do {
        input = read_input();
        argv = parse_input(input);
        status = execute(argv);
        free(input);
        free(argv);
    } while(status);
    return EXIT_SUCCESS;
}
