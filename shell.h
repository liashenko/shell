#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

/** Function declarations */
char *read_input();
char **parse_input(char *input);
int execute(char **argv);
int execute_command(char **argv);

/** Builtin functions */
int shell_exit(char **argv);
int shell_help(char **argv);
int shell_cd(char **argv);

/** Builtins */
struct builtin {
  int (*func)(char **);
  char *name;
};
typedef struct builtin builtin;
const static builtin builtin_exit = {&shell_exit, "exit"};
const static builtin builtin_help = {&shell_help, "help"};
const static builtin builtin_cd = {&shell_cd, "cd"};
const static struct builtin *builtins[4] = {&builtin_help, &builtin_cd,
                                            &builtin_exit, NULL};