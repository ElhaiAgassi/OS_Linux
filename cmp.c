#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Function to compare two files
int cmp(char *file1, char *file2, bool verbose, bool ignore_case)
{
    FILE *f1, *f2;
    int c1, c2;
    bool equal = true;

    // Open the two files for reading
    f1 = fopen(file1, "r");
    f2 = fopen(file2, "r");

    // Check if files were opened successfully
    if (f1 == NULL || f2 == NULL)
    {
        printf("Error: Failed to open files\n");
        return 1;
    }
    c1 = fgetc(f1);
    c2 = fgetc(f2);
    // Read characters from both files and compare them
    while (c1 != EOF || c2 != EOF)
    {
        if (ignore_case)
        {
            // Convert characters to lowercase if ignore_case flag is enabled
            c1 = tolower(c1);
            c2 = tolower(c2);
        }
        if (c1 != c2)
        {
            // If characters are not equal, set equal flag to false and break loop
            equal = false;
            break;
        }
        c1 = fgetc(f1);
        c2 = fgetc(f2);
    }

    // Close file pointers
    fclose(f1);
    fclose(f2);

    // Print result and return appropriate exit code
    if (equal)
    {
        if (verbose)
        {
            printf("equal\n");
        }
        return 0;
    }
    else
    {
        if (verbose)
        {
            printf("distinct\n");
        }
        return 1;
    }
}

int main(int argc, char *argv[])
{
    bool verbose = false;
    bool ignore_case = false;
    char *file1, *file2;

    // Check if minimum required arguments are provided
    if (argc < 3)
    {
        printf("Usage: %s <file1> <file2> [-v] [-i]\n", argv[0]);
        return 1;
    }

    file1 = argv[1];
    file2 = argv[2];

    // Parse optional flags
    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            verbose = true;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            ignore_case = true;
        }
    }

    // Call cmp function with provided arguments
    return cmp(file1, file2, verbose, ignore_case);
}
