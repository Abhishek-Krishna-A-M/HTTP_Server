#ifndef RESPONSE_H
#define RESPONSE_H

#include "Request.h"
#include <stdbool.h>
#include <stdlib.h>

// Serve a response based on HttpRequest
void serve_response(int client_socket, struct HttpRequest* request, const char* www_root, bool spa_mode);
void handle_api_request(int client_socket, struct HttpRequest* request);
void send_response(int client_socket, int status, const char* type, const char* body, size_t length);


#endif

