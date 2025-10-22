#include "../include/Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

struct Server server_constructor(int port, int backlog, struct ServerConfig* config) {
    struct Server server;
    server.port = port;
    server.backlog = backlog;
    server.config = config;

    server.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.socket_fd < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server.socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server.socket_fd);
        exit(1);
    }

    server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(port);

    if (bind(server.socket_fd, (struct sockaddr*)&server.address, sizeof(server.address)) < 0) {
        perror("bind");
        close(server.socket_fd);
        exit(1);
    }

    if (listen(server.socket_fd, backlog) < 0) {
        perror("listen");
        close(server.socket_fd);
        exit(1);
    }

    printf("Server listening on port %d\n", port);
    return server;
}
