#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *argv[MAX_ARGS];
    int pipe_fd[2], redir_fd;
    pid_t pid;
    int child_running = 0;

    while (1)
    {
        printf("stshell> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* check for exit command */
        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        /* split command line on pipe symbol */
        char *pipe_cmd = strtok(command, "|");
        char *next_cmd = strtok(NULL, "|");

        /* parse first command */
        int i = 0;
        char *token = strtok(pipe_cmd, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* check for redirection */
        int redir_index = -1;
        for (i = 0; i < MAX_ARGS - 1 && argv[i] != NULL; i++)
        {
            if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0)
            {
                redir_index = i;
                break;
            }
        }

        if (redir_index >= 0)
        {
            if (argv[redir_index + 1] == NULL)
            {
                fprintf(stderr, "no output file specified for redirection\n");
                continue;
            }
            redir_fd = open(argv[redir_index + 1], O_WRONLY | O_CREAT | (argv[redir_index][0] == '>' ? O_TRUNC : O_APPEND), 0644);
            if (redir_fd < 0)
            {
                perror("failed to open output file");
                continue;
            }
            dup2(redir_fd, STDOUT_FILENO);
            close(redir_fd);
            argv[redir_index] = NULL;
        }

        /* parse and execute remaining commands */
        while (next_cmd != NULL)
        {
            /* parse next command */
            i = 0;
            token = strtok(next_cmd, " ");
            while (token != NULL)
            {
                argv[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            argv[i] = NULL;

            /* create pipe */
            pipe(pipe_fd);

            /* fork and execute command */
            pid = fork();
            if (pid == 0)
            {
                /* child process */
                close(pipe_fd[0]);               // close read end of pipe
                dup2(pipe_fd[1], STDOUT_FILENO); // redirect stdout to the write end of the pipe
                if (execvp(argv[0], argv) < 0)
                {
                    perror("failed to execute command");
                    exit(1);
                }
            }
            else if (pid > 0)
            {
                /* parent process /
                if (child_running)
                {
                / if previous command was running, wait for it to finish */
                waitpid(pid, NULL, 0);
            }
            close(pipe_fd[1]);              // close write end of pipe
            dup2(pipe_fd[0], STDIN_FILENO); // redirect stdin to the read end of the pipe
            child_running = 1;
        }
        else
        {
            perror("failed to fork");
        }
        /* parse next command */
        next_cmd = strtok(NULL, "|");
    }

    /* execute last command */
    pid = fork();
    if (pid == 0)
    {
        /* child process */
        if (execvp(argv[0], argv) < 0)
        {
            perror("failed to execute command");
            exit(1);
        }
    }
    else if (pid > 0)
    {
        /* parent process */
        if (child_running)
        {
            /* if previous command was running, wait for it to finish */
            waitpid(pid, NULL, 0);
        }
        else
        {
            /* no previous command running, wait for current command to finish */
            waitpid(pid, NULL, 0);
        }
    }
    else
    {
        perror("failed to fork");
    }

    /* reset variables */
    child_running = 0;
    memset(command, 0, MAX_COMMAND_LENGTH);
    memset(argv, 0, MAX_ARGS * sizeof(char *));
}

return 0;
}