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
 *      ctrl-c should not kill the shell, but kill currently executing program
 *      piping (ls > out)
 *      globbing (ls *.c)
 */
#include "shell.h"

#define BUFFER_SIZE 1024
char prompt_text[BUFFER_SIZE];
void set_prompt_text() {
  getcwd(prompt_text, BUFFER_SIZE);
  sprintf(prompt_text, "%s: > ", prompt_text);
}
char *read_input() {
  char *input = readline(prompt_text);
  add_history(input);
  return input;
}

#define TOKENS_DELIMETERS " "
#define TOKENS_BUFFER_SIZE 256
#define TOKEN_SIZE 256
int is_delimeter(char ch) { return strchr(TOKENS_DELIMETERS, ch) != NULL; }
int is_quote(char ch) { return (ch == '\"' || ch == '\''); }
char **parse_input(char *input) {
  if (input == NULL) {
    return NULL;
  }
  int capacity = TOKENS_BUFFER_SIZE;
  char **tokens = malloc(capacity * sizeof(char *));
  if (tokens == NULL) {
    printf("shell: allocation error\n");
    exit(EXIT_FAILURE);
  }
  char *token = malloc(TOKEN_SIZE * sizeof(char));
  if (token == NULL) {
    printf("shell: allocation error\n");
    exit(EXIT_FAILURE);
  }
  int tokens_number = 0;
  int token_i = 0;
  int input_i = 0;
  char is_in_quote = 0;
  while (input[input_i] != '\0') {
    if (is_quote(input[input_i])) {
      is_in_quote = !is_in_quote;
    } else if (!is_in_quote && is_delimeter(input[input_i])) {
      if (token_i > 0) {
        tokens[tokens_number++] = strdup(token);
        memset(token, 0, TOKEN_SIZE * sizeof(char));
        token_i = 0;
      }
    } else {
      token[token_i++] = input[input_i];
    }
    ++input_i;
  }
  if (token_i > 0) {
    tokens[tokens_number++] = strdup(token);
  }
  tokens[tokens_number] = NULL;
  free(token);
  return tokens;
}

int execute(char **argv) {
  if (argv == NULL || argv[0] == NULL) {
    return 1;
  }
  /** check if command is a builtin */
  int i = 0;
  while (i < builtins_number()) {
    if (!strcmp(argv[0], builtins[i].name)) {
      return builtins[i].func(argv);
    }
    ++i;
  }
  return execute_external(argv);
}
/** Builtins */
int shell_exit(char **argv) { return 0; }
int shell_help(char **argv) {
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
int shell_cd(char **argv) {
  if (argv == NULL || argv[0] == NULL || argv[1] == NULL) {
    return 1;
  }
  if (chdir(argv[1])) {
    printf("shell: %s: No such file or directory\n", argv[1]);
    return 1;
  }
  set_prompt_text();
  return 1;
}

int execute_external(char **argv) {
  if (argv == NULL && argv[0] == NULL) {
    printf("shell: command not found\n");
    return 1;
  }
  int status;
  pid_t pid = fork();
  if (pid < 0) {
    printf("shell: forking child process failed\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    /** child process */
    if (execvp(argv[0], argv) < 0) {
      printf("shell: %s: command not found\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  } else {
    /** parent process */
    while (wait(&status) != pid)
      ;
  }
  return 1;
}

int main() {
  char *input = NULL;
  char **argv = NULL;
  int status = 1;
  set_prompt_text();
  rl_bind_key('\t', rl_complete);
  do {
    input = read_input();
    argv = parse_input(input);
    status = execute(argv);
    free(input);
    free(argv);
  } while (status);
  return EXIT_SUCCESS;
}
