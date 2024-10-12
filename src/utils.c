#include "utils.h"

int file_exist(char *filename)
{
    struct stat statbuffer;
    return stat(filename, &statbuffer) == 0 ? 1 : 0;
}

int streq(char *str1, char *str2)
{
    if (strcmp(str1, str2) == 0)
        return 1;

    return 0;
}