#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char *argv[])
{
    char *codec;
    char *message;

    if (argc == 2 && !isatty(STDIN_FILENO))
    {
        message = (char*) malloc(1248 * sizeof(char));
        if (fgets(message, 1248, stdin) != NULL)
        {
            // Remove the newline character at the end of the input
            message[strcspn(message, "\n")] = 0;
            codec = argv[1];
        }
    }
    else if (argc == 3)
    {
        codec = argv[1];
        message = argv[2];
    }
    else
    {
        printf("Usage: encode <codec> <message>\n");
        return 1;
    }

    if (strcmp(codec, "codecA") == 0)
    {
        codecA(message); // Call the encoding function in codecA shared library
        printf("%s\n", message);
    }
    else if (strcmp(codec, "codecB") == 0)
    {
        codecB_encode(message); // Call the encoding function in codecB shared library
        printf("%s\n", message);
    }
    else
    {
        printf("Invalid codec\n");
        return 1;
    }

    return 0;
}
