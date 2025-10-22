#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

struct ServerConfig; // forward

struct Server {
    int socket_fd;
    struct sockaddr_in address;
    int port;
    int backlog;
    struct ServerConfig* config;
};

struct Server server_constructor(int port, int backlog, struct ServerConfig* config);

#endif

