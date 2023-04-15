
#include <ctype.h>
#include "codecA.h"


// Function to encode/decode a string using codecA
void codecA(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (islower(str[i]))
        {
            str[i] = (char) toupper(str[i]);
        }
        else if (isupper(str[i]))
        {
            str[i] = (char) tolower(str[i]);
        }
    }
}

