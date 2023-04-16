#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LEN 100
#define MAX_ARGS 10

void sigintHandler(int sig)
{
    // Signal handler for SIGINT (Ctrl+C)
    printf("\nReceived SIGINT (Ctrl+C). Exiting gracefully...\nstshell> ");
}

int main()
{
    char input[MAX_CMD_LEN];
    char *args[MAX_ARGS];
    char *token;
    int i;

    signal(SIGINT, sigintHandler); // Register SIGINT signal handler

    while (1)
    {
        printf("stshell> ");
        fgets(input, MAX_CMD_LEN, stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        // Tokenize input to extract program name and arguments
        token = strtok(input, " ");
        i = 0;
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0)
        {
            // Exit loop if 'exit' is entered
            break;
        }
        else
        {
            // Fork a child process
            pid_t pid = fork();

            if (pid == -1)
            {
                // Fork failed
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // Child process

                // Check for output redirection
                int out_redirect = 0; // 0: no redirection, 1: '>' redirection, 2: '>>' redirection
                char *outfile = NULL; // output file name
                for (int j = 0; args[j] != NULL; j++)
                {
                    if (strcmp(args[j], ">") == 0)
                    {
                        out_redirect = 1;
                        outfile = args[j + 1];
                        args[j] = NULL;
                        break;
                    }
                    else if (strcmp(args[j], ">>") == 0)
                    {
                        out_redirect = 2;
                        outfile = args[j + 1];
                        args[j] = NULL;
                        break;
                    }
                }

                // Check for piping
                int pipe_flag = 0;  // 0: no piping, 1: piping
                int pipe_index = 0; // index of pipe character '|' in args
                int pipe_fds[2];    // file descriptors for pipe
                for (int j = 0; args[j] != NULL; j++)
                {
                    if (strcmp(args[j], "|") == 0)
                    {
                        pipe_flag = 1;
                        pipe_index = j;
                        args[j] = NULL;
                        break;
                    }
                }

                if (pipe_flag)
                {
                    // Piping is requested

                    // Create a pipe
                    if (pipe(pipe_fds) == -1)
                    {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                    }

                    // Fork a child process for the left side of the pipe
                    pid_t left_pid = fork();
                    if (left_pid == -1)
                    {
                        // Fork failed
                        perror("fork");
                        exit(EXIT_FAILURE);
                    }

                    else if (left_pid == 0)
                    {
                        // Child process for the left side of the pipe
                        // Close the read end of the pipe
                        close(pipe_fds[0]);

                        // Redirect stdout to the write end of the pipe
                        dup2(pipe_fds[1], STDOUT_FILENO);
                        close(pipe_fds[1]);

                        // Execute the left command
                        if (execvp(args[0], args) == -1)
                        {
                            perror("execvp");
                            exit(EXIT_FAILURE);
                        }
                        // Close the pipe before exiting
                        close(pipe_fds[1]);
                    }
                    else
                    {
                        // Parent process

                        // Fork another child process for the right side of the pipe
                        pid_t right_pid = fork();
                        if (right_pid == -1)
                        {
                            // Fork failed
                            perror("fork");
                            exit(EXIT_FAILURE);
                        }
                        else if (right_pid == 0)
                        {
                            // Child process for the right side of the pipe

                            // Close the write end of the pipe
                            close(pipe_fds[1]);

                            // Redirect stdin to the read end of the pipe
                            dup2(pipe_fds[0], STDIN_FILENO);
                            close(pipe_fds[0]);

                            // Execute the right command
                            if (execvp(args[pipe_index + 1], &args[pipe_index + 1]) == -1)
                            {
                                perror("execvp");
                                exit(EXIT_FAILURE);
                            }
                            // Close the pipe before exiting
                            close(pipe_fds[0]);
                        }
                        else
                        {
                            // Parent process

                            // Wait for both child processes to complete
                            waitpid(left_pid, NULL, 0);
                            waitpid(right_pid, NULL, 0);

                            // Close the pipe
                            close(pipe_fds[0]);
                            close(pipe_fds[1]);
                        }
                    }
                }
                else
                {
                    // No piping requested

                    // Check for output redirection
                    if (out_redirect)
                    {
                        int fd;
                        if (out_redirect == 1)
                        {
                            // '>' redirection
                            fd = open(outfile, O_RDWR);
                        }
                        else if (out_redirect == 2)
                        {
                            // '>>' redirection
                            fd = open(outfile, O_RDWR);
                        }
                        if (fd == -1)
                        {
                            perror("open");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }

                    // Execute the command
                    if (execvp(args[0], args) == -1)
                    {
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else
            {
                // Parent process

                // Wait for the child process to complete
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
