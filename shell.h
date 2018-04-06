#include <setjmp.h>
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
int execute_external(char **argv);

/** Builtin functions */
int shell_exit(char **argv);
int shell_help(char **argv);
int shell_cd(char **argv);

/** Builtins */
struct builtin {
  char *name;
  int (*func)(char **);
};
const static struct builtin builtins[] = {
    {"help", &shell_help}, {"cd", &shell_cd}, {"exit", &shell_exit}};
int builtins_number() { return sizeof(builtins) / sizeof(struct builtin); }