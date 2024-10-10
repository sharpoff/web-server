#ifndef HTTP_H_
#define HTTP_H_

#include <sys/socket.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void handle_request(int sockfd, char *request, int request_len);
void send_response(int sockfd, char *response, int response_len);
void create_response_404(char *response);
void create_response_date(char *date);
void create_response_root(char *response, char *body, int content_len);

#endif