#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "http.h"

#define PORT "1337" // leeeeet port

void *get_in_addr(struct sockaddr *in) {
    if (in->sa_family == AF_INET)
        return &(((struct sockaddr_in*)in)->sin_addr);
    
    return &(((struct sockaddr_in6*)in)->sin6_addr);
}

int main(int argc, char **argv)
{
    int servfd, clientfd;
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int infoerr;
    if ((infoerr = getaddrinfo(NULL, PORT, &hints, &servinfo))) {
        fprintf(stderr, "server: failed to getaddrinfo(%s)\n", gai_strerror(infoerr));
        exit(1);
    }

    if ((servfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        perror("server: socket");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(servfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(servfd);
        perror("server: bind");
        exit(1);
    }

    freeaddrinfo(servinfo);

    if (listen(servfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    // accept loop
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    char client_ip[INET6_ADDRSTRLEN];
    for (;;) {
        clientfd = accept(servfd, (struct sockaddr *)&client_addr, &addr_len);
        if (clientfd == -1) {
            perror("accept");
            continue;
        }
        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), client_ip, sizeof client_ip);
        printf("server: got connection from %s\n", client_ip);

        // process request and make response
        char request[2048];
        handle_request(clientfd, request, sizeof request);

        close(clientfd);
    }

    close(servfd);
    return 0;
}