#ifndef ROUTER_H
#define ROUTER_H

#include "Request.h"
#include "Config.h"
#include <stdbool.h>

// returns true if route handled (response already sent)
bool handle_api_route(int client_socket, struct HttpRequest* req, struct ServerConfig* config);

#endif

