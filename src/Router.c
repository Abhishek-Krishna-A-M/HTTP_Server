#include "../include/Router.h"
#include "../include/Response.h"
#include "../include/Proxy.h"
#include <string.h>
#include <stdio.h>

bool handle_api_route(int client_socket, struct HttpRequest* req, struct ServerConfig* config) {
    if (!config) return false;
    const struct ApiRoute* route = find_api_route(config, req->path, req->method);
    if (!route) return false;
    if (route->proxy_url[0]) {
        // proxy to external backend
        proxy_request(client_socket, route->proxy_url, req->path);
        return true;
    }
    if (route->response[0]) {
        send_response(client_socket, 200, "application/json", route->response, strlen(route->response));
        return true;
    }
    // placeholder for handler_name mapping in future
    send_response(client_socket, 501, "application/json", "{\"error\":\"Not implemented\"}", 23);
    return true;
}

