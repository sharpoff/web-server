#include "http.h"

void handle_request(int sockfd, char *request, int request_len)
{
    if (recv(sockfd, request, request_len, 0) == -1) {
        perror("recv");
        return;
    }

    char reqtype[5];
    char filename[10];
    sscanf(request, "%4s %9s ", reqtype, filename);
    printf("Request type is %s\nfilename: \"%s\"\n", reqtype, filename);

    char response[2048];
    memset(response, 0, sizeof response);
    if (streq(reqtype, "GET")) {
        char buf[2048];
        int content_len = -1;
        memset(buf, 0, sizeof buf);

        if (streq(filename, "/")) { // root request
            if ((content_len = read_file("index.html", buf, sizeof buf)) == -1) {
                create_response_404(response);
            } else {
                // TODO: make it more abstract
                // TODO: add mime types check (using strchr to check extension)
                create_response_root(response, buf, content_len);
            }
        } else { // other requested files
            create_response_404(response);
        }
    } else { // other request types
        create_response_404(response);
    }

    printf("Response:\n%s\n", response);

    int response_len = strlen(response)+1;
    send_response(sockfd, response, response_len);
}

void send_response(int sockfd, char *response, int response_len)
{
    if (send(sockfd, response, response_len, 0) == -1) {
        perror("send");
    }
}

void create_response_date(char *date)
{
    time_t current_time = time(NULL);
    struct tm *tm_local = gmtime(&current_time);

    char week[10], month[10], day[10], time[10], year[10];
    sscanf(asctime(tm_local), "%9s %9s %9s %9s %9s\n", week, month, day, time, year);
    sprintf(date, "Date: %s, %s %s %s %s GMT\n", week, day, month, year, time);
}

void create_response_404(char *response)
{
    strcat(response, "HTTP/1.1 404 NOT FOUND\n");

    // is this a good way to concat strings?
    char date[62];
    create_response_date(date);
    strcat(response, date);
    strcat(response, "Connection: close\n");
    strcat(response, "Content-Length: 13\n");
    strcat(response, "Content-Type: text/plain\n\n");
    strcat(response, "404 Not Found");
}

void create_response_root(char *response, char *body, int content_len)
{
    strcat(response, "HTTP/1.1 200 OK\n");

    // is this a good way to concat strings?
    char date[62];
    create_response_date(date);
    strcat(response, date);
    strcat(response, "Connection: close\n");
    char cont_str[100];
    snprintf(cont_str, 100, "Content-length: %d\n", content_len);
    printf("%s\n", cont_str);
    strcat(response, cont_str);
    strcat(response, "Content-Type: text/html\n\n");
    strcat(response, body);
}