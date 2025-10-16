#include "Server.h"

struct Server server_constructor(int server_domain, int service, int protocol, u_long interface, int port, int backlog, void (*launch)(void));
{
    struct Server server;
    server.domain = domain;
    server.service=service;
    server.protocol=protocol;
    server.port=port;
    server.interface=interface;
    server.backlog=backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr=htonl(interface);

    return server;

}
