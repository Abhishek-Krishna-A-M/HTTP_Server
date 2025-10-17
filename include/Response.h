#ifndef RESPONSE_H
#define RESPONSE_H

#include "Request.h"
#include <stdbool.h>

// Serve a response based on HttpRequest
void serve_response(int client_socket, struct HttpRequest* request, const char* www_root, bool spa_mode);

#endif

