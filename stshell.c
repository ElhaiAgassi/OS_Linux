#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

void sigint_handler(int signum) {
    // Custom signal handler for SIGINT (Ctrl+C)
    // You can customize this function to perform any other actions as needed
    printf("\n");
}

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;

    signal(SIGINT, sigint_handler); // Set up signal handler for SIGINT

    while (1) {
        printf("stshell> "); // Change prompt to stshell
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0';

        if (strcmp(command, "exit") == 0) {
            exit(0); // Terminate the shell program if "exit" command is entered
        }

        // Check for ">" redirection
        char *output_redirection = strstr(command, ">");
        if (output_redirection != NULL) {
            *output_redirection = '\0'; // Null-terminate the command at ">"
            char *output_file = strtok(output_redirection + 1, " "); // Get the output file name
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            dup2(fd, STDOUT_FILENO); // Redirect stdout to the output file
            close(fd);
        }

        // Check for ">>" redirection
        char *append_redirection = strstr(command, ">>");
        if (append_redirection != NULL) {
            *append_redirection = '\0'; // Null-terminate the command at ">>"
            char *output_file = strtok(append_redirection + 2, " "); // Get the output file name
            int fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            dup2(fd, STDOUT_FILENO); // Redirect stdout to the output file
            close(fd);
        }

        // Check for "|" piping
        char *pipe_command = strstr(command, "|");
        if (pipe_command != NULL) {
            *pipe_command = '\0'; // Null-terminate the first command at "|"
            char *second_command = strtok(pipe_command + 1, " "); // Get the second command
            char *argv2[10];
            int j = 0;
            char *token2 = strtok(second_command, " ");
            while (token2 != NULL) {
                argv2[j] = token2;
                token2 = strtok(NULL, " ");
                j++;
            }
            argv2[j] = NULL;

            int fd[2];
            pipe(fd);

            if (fork() == 0) {
                // Child process for the first command
                close(fd[0]); // Close read end of pipe
                dup2(fd[1], STDOUT_FILENO); // Redirect stdout to write end of pipe
                execvp(argv[0], argv);
                exit(0);
            } else {
                wait(NULL);
                close(fd[1]); // Close write end of pipe
                if (fork() == 0) {
                    // Child process for the second command
                    dup2(fd[0], STDIN_FILENO); // Redirect stdin to read end of pipe
                    execvp(argv2[0], argv2);
                    exit(0
                }
            }
            close(fd[0]); // Close read end of pipe
            close(fd[1]); // Close write end of pipe
        } else {
            // No redirection or piping, execute the command normally
            if (fork() == 0) {
                execvp(argv[0], argv);
                wait(NULL);
            }
        }
    }
}

