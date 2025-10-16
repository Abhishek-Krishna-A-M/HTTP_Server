#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include<netinet/in.h>


struct Server{
    int domain;
    int protocol;
    int service;
    u_long interface;
    int port;
    int backog;

    struct sockaadr_in address;

    void (*launch)(void);
};

struct Server server_constructor(int domain,int service,int protocol,u_long interface,int port,int backlog,
                                 void (*launch)(void));

#endif
