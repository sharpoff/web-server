#include "utils.h"

int read_file(char *filename, char *buffer, int buf_size)
{
    FILE *fp = NULL;
    if (!(fp = fopen(filename, "r"))) {
        fprintf(stderr, "failed to open file: %s", filename);
        return -1;
    }
    if (fp == NULL) {
        fprintf(stderr, "file pointer is null\n");
        return -1;
    }

    int s = 0;
    for (char ch = getc(fp); ch != EOF; ch = getc(fp)) {
        buffer[s++] = ch;
    }
    fclose(fp);

    printf("read_file buf with size %d:\n%s\n", s, buffer);

    return s;
}

int streq(char *str1, char *str2)
{
    if (strcmp(str1, str2) == 0)
        return 1;

    return 0;
}