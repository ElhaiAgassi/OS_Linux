#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void print_usage(char *program_name) {
    printf("Usage: %s <file1> <file2> [-v] [-f]\n", program_name);
}

int copy(char *src_filename, char *dest_filename, bool verbose, bool force) {
    FILE *src_file, *dest_file;
    int c;

    // Open source file for reading
    src_file = fopen(src_filename, "r");
    if (src_file == NULL) {
        printf("general failure\n");
        return 1;
    }

    // Check if destination file already exists
    if ((dest_file = fopen(dest_filename, "w")) != NULL)
    {
        if (!force)
        {
            printf("target file exist\n");
            fclose(src_file);
            fclose(dest_file);
            return 1;
        }
        // Copy contents from source to destination
        while ((c = fgetc(src_file)) != EOF)
        {
            fputc(c, dest_file);
        }
        if (verbose)
            {
                printf("success\n");
            }
    }
    else
    { 
        fclose(src_file);
        printf("general failure\n");
        return 1;
    }

    // Close files
    fclose(src_file);
    fclose(dest_file);

    return 0;
}

int main(int argc, char *argv[]) {
    bool verbose = false;
    bool force = false;
    char *file1, *file2;

    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    file1 = argv[1];
    file2 = argv[2];

    // Parse optional flags
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            force = true;
        } else {
            printf("Error: Unrecognized option '%s'\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    return copy(file1, file2, verbose, force);
}
