#include "http.h"

void handle_request(int sockfd)
{
    char request[BUFFERLEN];
    if (recv(sockfd, request, sizeof request, 0) == -1) {
        perror("recv");
        return;
    }

    // TODO: this looks ugly
    char reqtype[4];
    char url[100];
    sscanf(request, "%3s %99s HTTP/1.1", reqtype, url);

    char file[104];
    snprintf(file, 104, "root%s", url);
    char *filename = file;

    printf("Filename: %s\n", filename);

    if (streq(reqtype, "GET")) {
        if (streq(filename, "root/")) {
            sprintf(filename, "root/index.html");
            send_response_200(sockfd, filename);
        } else if (!file_exist(filename)) {
            send_response_404(sockfd);
        } else {
            send_response_200(sockfd, filename);
        }
    } else {
        send_response_404(sockfd);
    }
}

char *get_mime_type(char *filename)
{
    char *extension = strchr(filename, '.');

    if (extension == NULL)
        return "application/octet-stream";

    extension++; // delete '.'

    // most common one
    if (streq(extension, "html") || streq(extension, "htm")) return "text/html";
    else if (streq(extension, "txt")) return "text/plain";
    else if (streq(extension, "ico")) return "image/vnd.microsoft.icon";
    else if (streq(extension, "jpeg") || streq(extension, "jpg")) return "image/jpeg";
    else if (streq(extension, "csv")) return "text/csv";
    else if (streq(extension, "pdf")) return "application/pdf";
    else if (streq(extension, "epub")) return "application/epub+zip";
    else if (streq(extension, "docx")) return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    else if (streq(extension, "doc")) return "application/msword";
    else if (streq(extension, "json")) return "application/json";
    else if (streq(extension, "gif")) return "image/gif";
    else if (streq(extension, "png")) return "image/png";
    else if (streq(extension, "mp3")) return "audio/mpeg";
    else if (streq(extension, "mp4")) return "video/mp4";
    else if (streq(extension, "mpeg")) return "video/mpeg";

    return "application/octet-stream";
}

void send_response_404(int fd)
{
    char header[HEADERLEN];
    char content_type[FIELDLEN];
    char body[BODYLEN] = "404 Not Found";

    time_t rawtime;
    time(&rawtime);
    char date[100];
    strftime(date, sizeof date, "%a, %d %b %Y %H:%M:%S GMT", localtime(&rawtime));

    snprintf(header, HEADERLEN, "HTTP/1.1 404 NOT FOUND\r\nDate: %s\r\nConnection: close\r\nContent-Length: %ld\r\n", date, strlen(body));
    snprintf(content_type, FIELDLEN, "Content-Type: text/plain\r\n\r\n");

    char response[BUFFERLEN];
    snprintf(response, BUFFERLEN, "%s%s%s", header, content_type, body);

    if (send_data(fd, response, strlen(response)) == -1)
        close(fd);
}

void send_response_200(int fd, char *filename)
{
    char header[HEADERLEN];
    char content_type[FIELDLEN];
    char data[BODYLEN];

    time_t rawtime;
    time(&rawtime);
    char date[100];
    strftime(date, sizeof date, "%a, %d %b %Y %H:%M:%S GMT", localtime(&rawtime));

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        send_response_404(fd);
        return;
    }

    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    if (length <= 0) {
        fprintf(stderr, "File length is %d\n", length);
        send_response_404(fd);
        return;
    }
    rewind(fp);

    snprintf(header, HEADERLEN, "HTTP/1.1 200 OK\r\nDate: %s\r\nConnection: close\r\nContent-Length: %d\r\n", date, length);
    snprintf(content_type, FIELDLEN, "Content-Type: %s\r\n\r\n", get_mime_type(filename));

    char response[BUFFERLEN];
    snprintf(response, BUFFERLEN, "%s%s", header, content_type);

    if (send_data(fd, response, strlen(response)) == -1) {
        fprintf(stderr, "Failed to send header\n");
        close(fd);
        return;
    }

    while (length > 0) {
        int bytes = fread(data, 1, min(length, sizeof(data)), fp);
        if (bytes < 1) {
            close(fd);
            break;
        }
        if (send_data(fd, data, bytes) == -1) {
            close(fd);
            break;
        }
        //printf("Sent %d bytes\n", bytes);
        length -= bytes;
    }
}

int send_data(int fd, void *data, int datasize)
{
    char *ptr = (char*)data;
    ssize_t bytes;

    while (datasize > 0) {
        bytes = send(fd, data, datasize, 0);
        if (bytes < 0) return -1;
        ptr += bytes;
        datasize -= bytes;
    }

    return 0;
}