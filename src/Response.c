#include "../include/Response.h"
#include "../include/FileHandler.h"
#include "../include/Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Send HTTP response (always HTTP/1.1)
void send_response(int client_socket, int status, const char* type, const char* body, size_t length) {
    char header[512];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "X-Powered-By: C HTTP Server\r\n"
        "\r\n",
        status, type, length);
    write(client_socket, header, header_len);
    if (body && length > 0) write(client_socket, body, length);
}

// Handle API requests
void handle_api_request(int client_socket, struct HttpRequest* request) {
    if (strcmp(request->path, "/api/hello") == 0) {
        const char* msg = "{\"message\":\"Hello from C server!\"}";
        send_response(client_socket, 200, "application/json", msg, strlen(msg));
    } else {
        const char* msg = "{\"error\":\"Not Found\"}";
        send_response(client_socket, 404, "application/json", msg, strlen(msg));
    }
}

// Serve static files with SPA fallback
void serve_response(int client_socket, struct HttpRequest* request, const char* www_root, bool spa_mode) {
    char path[512];
    if (strcmp(request->path, "/") == 0) {
        snprintf(path, sizeof(path), "%s/index.html", www_root);
    } else {
        snprintf(path, sizeof(path), "%s%s", www_root, request->path);
    }

    // SPA fallback
    if (spa_mode && !file_exists(path)) {
        snprintf(path, sizeof(path), "%s/index.html", www_root);
    }

    if (!file_exists(path)) {
        const char* msg = "<html><body><h1>404 Not Found</h1></body></html>";
        send_response(client_socket, 404, "text/html", msg, strlen(msg));
        return;
    }

    char* buffer;
    size_t size = read_file(path, &buffer);
    if (size == 0) {
        const char* msg = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        send_response(client_socket, 500, "text/html", msg, strlen(msg));
        return;
    }

    send_response(client_socket, 200, get_mime_type(path), buffer, size);

    log_request("127.0.0.1", request->method, request->path, 200);
    free(buffer);
}
