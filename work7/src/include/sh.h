#pragma once

#define BEBRASH_TOK_BUFSIZE 64
#define BEBRASH_COMMAND_BUFSIZE 16
#define BEBRASH_TOK_DELIM " \t\r\n\a"
#define BEBRASH_COMMAND_DELIM "|"
#define WRITE_END 1
#define READ_END 0

void greeting();
void barash_loop();
char *barash_read_String();
char **barash_split_String_to_commands(char *String, int *count_commands);
char **barash_split_command(char *command);
int barash_execute(char **tokens);
int barash_launch(char ***tokens, int count_commands);

int barash_cd(char **args);
int barash_help(char **args);
int barash_exit(char **args);

int barash_num_builtins();
