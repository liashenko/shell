#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

#define TOKENS_DELIMETERS " "
#define TOKENS_BUFFER_SIZE 256
#define TOKEN_SIZE 256
#define PROMPT_SIZE 1024

/** Function declarations */
void set_prompt_text();
void sig_handler(int sig) {}
char *read_input();
char **parse_input(const char *input);
int is_delimeter(const char ch) { return strchr(TOKENS_DELIMETERS, ch) != NULL; }
int is_quote(const char ch) { return (ch == '\"' || ch == '\''); }
int execute(char **argv);
int execute_external(char **argv);
int execute_pipe(char** argv);

/** Builtin functions */
int shell_exit(char **argv);
int shell_help(char **argv);
int shell_cd(char **argv);

/** Builtins */
typedef struct {
  char *name;
  int (*func)(char **);
} builtin;
const static builtin builtins[] = {
    {"help", &shell_help}, {"cd", &shell_cd}, {"exit", &shell_exit}};
int builtins_number() { return sizeof(builtins) / sizeof(builtin); }