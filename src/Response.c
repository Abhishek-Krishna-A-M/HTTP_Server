#include "../include/Response.h"
#include "../include/FileHandler.h"
#include "../include/Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
        char *not_found =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 45\r\n\r\n"
            "<html><body><h1>404 Not Found</h1></body></html>";
        write(client_socket, not_found, strlen(not_found));
        return;
    }

    // Read file
    char* buffer;
    size_t size = read_file(path, &buffer);
    if (size == 0) {
        char *error500 =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 57\r\n\r\n"
            "<html><body><h1>500 Internal Server Error</h1></body></html>";
        write(client_socket, error500, strlen(error500));
        return;
    }

    // Send HTTP headers
    const char* mime = get_mime_type(path);
    char header[256];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n",
        mime, size);
    write(client_socket, header, header_len);

    int status_code = 200; // default
if (!file_exists(path)) status_code = 404;
    log_request("127.0.0.1", request->method, request->path, status_code);


    // Send file content
    write(client_socket, buffer, size);
    free(buffer);
}

