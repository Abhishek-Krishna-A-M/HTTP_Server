#include "../include/Headers.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void add_cors_and_custom_headers(int client_socket) {
    char headers[256];
    snprintf(headers, sizeof(headers),
        "Access-Control-Allow-Origin: *\r\n"
        "X-Powered-By: C HTTP Server\r\n");
    write(client_socket, headers, strlen(headers));
}

