#ifndef HTTP_H_
#define HTTP_H_

#include <sys/socket.h>
#include <time.h>
#include "utils.h"
#include <math.h>

void handle_request(int sockfd);
char *get_mime_type(char *filename);
void send_response_404(int fd);
void send_response_200(int fd, char *filename);
int send_data(int fd, void *data, int datasize);

#endif