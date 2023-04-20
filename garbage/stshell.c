#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_CMD_LEN 100

void sigintHandler(int sig)
{
    // Signal handler for SIGINT (Ctrl+C)
    printf("\nReceived SIGINT (Ctrl+C). Exiting gracefully...\nstshell> ");
}

int main()
{
    char *input;
    char *args[MAX_CMD_LEN];
    char *token;
    int i;

    signal(SIGINT, sigintHandler); // Register SIGINT signal handler

    while (1)
    {
        input = readline("stshell> "); // Read input using readline

        // If input is empty, continue to next iteration
        if (input == NULL || strlen(input) == 0)
        {
            continue;
        }

        // Add input to history
        add_history(input);

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
                execvp(args[0], args);
                // execvp only returns if there's an error
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else
            {
                // Parent process
                wait(NULL); // Wait for child to complete
            }
        }
    }

    return 0;
}
