#include "codecB.h"

// Function to encode a string using codecB
void codecB_encode(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = (char)(str[i] + 3);
    }
}
// Function to decode a string using codecB

void codecB_decode(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = (char)(str[i] - 3);
    }
}