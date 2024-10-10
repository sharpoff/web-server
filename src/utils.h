#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int read_file(char *filename, char *buffer, int buf_size);
int streq(char *str1, char *str2);

#endif