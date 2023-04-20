#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_CMD_LEN 100

int isRunning;
pid_t pid;

void sigintHandler(int signum)
{
    if (signum == SIGINT)
    {
        if (isRunning)
        {
            if (kill(pid, SIGINT) == -1)
                perror("kill");
        }
        else
            printf("You typed Ctrl-C!\n");
    }
}

int main()
{
    char input[MAX_CMD_LEN];
    FILE *fp;
    char result[MAX_CMD_LEN];

    signal(SIGINT, sigintHandler); // Register SIGINT signal handler

    while (1)
    {
        printf("stshell> ");
        fgets(input, MAX_CMD_LEN, stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
        {
            // Exit loop if 'exit' is entered
            break;
        }
        else
        {
            // Execute the command using popen
            fp = popen(input, "r");
            if (fp == NULL)
            {
                perror("popen");
                exit(EXIT_FAILURE);
            }

            // Read the output of the command
            while (fgets(result, MAX_CMD_LEN, fp) != NULL)
            {
                printf("%s", result);
            }

            // Close the pipe
            pclose(fp);
        }
    }

    return 0;
}