#include "sh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void greeting()
{
    printf("Командная оболочка запущена.\n");
    printf("Введите команды:\n");
}

void barash_loop()
{
    char *String;
    char **commands;
    char ***tokens;
    int status;
    int count_commands;
    int i;
    do
    {
        printf("> ");
        String = barash_read_String();
        commands = barash_split_String_to_commands(String, &count_commands);
        tokens = malloc(sizeof(char *) * count_commands);
        for (i = 0; i < count_commands; i++)
        {
            tokens[i] = barash_split_command(commands[i]);
        }
        status = barash_launch(tokens, count_commands);
        free(String);
        free(commands);
        free(tokens);

    } while (status);
}

char *barash_read_String()
{
    char *String = NULL;
    ssize_t bufsize = 0;
    getline(&String, &bufsize, stdin);
    return String;
}

char **barash_split_String_to_commands(char *String, int *count_commands)
{
    int bufsize = BEBRASH_COMMAND_BUFSIZE;
    int i = 0;
    char **commands = malloc(bufsize * sizeof(char *));
    char *istr;
    if (!commands)
    {
        fprintf(stderr, "bebrash: ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    istr = strtok(String, BEBRASH_COMMAND_DELIM);
    while (istr != NULL)
    {
        commands[i] = istr;
        i++;
        if (i == bufsize)
        {
            bufsize += BEBRASH_COMMAND_BUFSIZE;
            commands = realloc(commands, bufsize * sizeof(char *));
            if (!commands)
            {
                fprintf(stderr, "bebrash: ошибка выделения памяти\n");
                exit(EXIT_FAILURE);
            }
        }

        istr = strtok(NULL, BEBRASH_COMMAND_DELIM);
    }
    commands[i] = NULL;
    *count_commands = i;
    return commands;
}

char **barash_split_command(char *command)
{
    int bufsize = BEBRASH_TOK_BUFSIZE;
    int i = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;
    if (!tokens)
    {
        fprintf(stderr, "bebrash: ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, BEBRASH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        if (i == bufsize)
        {
            bufsize += BEBRASH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "bebrash: ошибка выделения памяти\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, BEBRASH_TOK_DELIM);
    }
    tokens[i] = NULL;
    return tokens;
}

int barash_launch(char ***tokens, int count_commands)
{

    if (count_commands == 1)
    {
        int flag = barash_execute(tokens[0]);
        if (flag != -1)
        {
            return flag;
        }
        else
        {
            pid_t pid;
            int status;
            pid = fork();
            if (pid == 0)
            {
                execvp(tokens[0][0], tokens[0]);
                perror("exec");
                exit(1);
            }
            else
            {
                do
                {
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
        }
    }
    else
    {
        pid_t pid;
        int fd[count_commands][2];
        int status;
        int step = 0;
        int i;
        for (i = 0; i < count_commands; i++)
        {
            pipe(fd[i]);
        }
        while (step != count_commands)
        {
            pid = fork();
            if (step == count_commands - 1)
            {
                if (pid == 0)
                {
                    dup2(fd[step][READ_END], STDIN_FILENO);
                        close(fd[step][WRITE_END]);
                    execvp(tokens[step][0], tokens[step]);
                    perror("exec");
                    exit(1);
                }
            }
            else
            {
                if (pid == 0)
                {
                    dup2(fd[step][READ_END],STDIN_FILENO); 
                    close(fd[step][WRITE_END]);
                    dup2(fd[step + 1][WRITE_END],STDOUT_FILENO); 
                    close(fd[step + 1][READ_END]); 
                    execvp(tokens[step][0], tokens[step]);
                    perror("exec");
                    exit(1);
                }
            }
            close(fd[step + 1][WRITE_END]); 
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            step++;
        }
    }

    return 1;
}

char *builtin_str[] = {"cd", "help", "exit"};

int (*builtin_func[])(char **) = {&barash_cd, &barash_help, &barash_exit};

int barash_execute(char **tokens)
{
    int i;
    if (tokens[0] == NULL)
    {
        // Была введена пустая команда.
        return 1;
    }
    for (i = 0; i < barash_num_builtins(); i++)
    {
        if (strcmp(tokens[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(tokens);
        }
    }
    return -1;
}

int barash_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

int barash_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "bebrash: ожидается аргумент для \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("bebrash");
        }
    }
    return 1;
}

int barash_help(char **args)
{
    int i;
    printf("Наберите название программы и её аргументы и нажмите enter.\n");
    printf("Вот список встроенных команд:\n");
    for (i = 0; i < barash_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }
    printf("Используйте команду man для получения информации по другим "
           "программам.\n");
    return 1;
}

int barash_exit(char **args) { return 0; }