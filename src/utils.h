#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIELDLEN 1000
#define HEADERLEN 4000
#define BODYLEN HEADERLEN
#define BUFFERLEN HEADERLEN + BODYLEN + FIELDLEN

#define min(a,b) (((a) > (b)) ? (b) : (a))

int file_exist(char *filename);
int streq(char *str1, char *str2);

#endif