#include <stdio.h>
#include <string.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: encode <codec> <message>\n");
        return 1;
    }

    char *codec = argv[1];
    char *message = argv[2];

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