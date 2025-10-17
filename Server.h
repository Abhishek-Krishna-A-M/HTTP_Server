#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int socket;
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    void (*launch)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog,
                                 void (*launch)(struct Server *server));

#endif
