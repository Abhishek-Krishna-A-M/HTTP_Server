#include "../include/Response.h"
#include "../include/FileHandler.h"
#include "../include/Headers.h"
#include "../include/Logger.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// send_response always sends proper HTTP/1.1 status line and headers.
void send_response(int client_socket, int status, const char* type, const char* body, size_t length) {
    char headers_extra[512];
    add_common_headers(headers_extra, sizeof(headers_extra));

    char header[1024];
    const char* status_text = (status==200)?"OK":(status==404)?"Not Found":(status==304)?"Not Modified":(status==500)?"Internal Server Error":(status==502)?"Bad Gateway":"OK";
    int n = snprintf(header, sizeof(header),
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu\r\n"
            "%s"
            "\r\n",
            status, status_text, type ? type : "application/octet-stream", length, headers_extra);
    write(client_socket, header, n);
    if (body && length > 0) write(client_socket, body, length);
}

void handle_api_request(int client_socket, struct HttpRequest* request) {
    // simple placeholder — real routing is in Router.c
    send_response(client_socket, 404, "application/json", "{\"error\":\"no api\"}", 16);
}

void serve_response(int client_socket, struct HttpRequest* request, const char* www_root, bool spa_mode) {
    char path[1024];
    if (strcmp(request->path, "/") == 0) {
        snprintf(path, sizeof(path), "%s/index.html", www_root);
    } else {
        // protect against path traversal
        const char* rel = request->path;
        if (strstr(rel, "..")) {
            send_response(client_socket, 403, "text/plain", "403 Forbidden", 13);
            log_request("0.0.0.0", request->method, request->path, 403);
            return;
        }
        snprintf(path, sizeof(path), "%s%s", www_root, rel);
    }

    if (spa_mode && !file_exists(path)) {
        snprintf(path, sizeof(path), "%s/index.html", www_root);
    }

    if (!file_exists(path)) {
        const char* nf = "<html><body><h1>404 Not Found</h1></body></html>";
        send_response(client_socket, 404, "text/html", nf, strlen(nf));
        log_request("0.0.0.0", request->method, request->path, 404);
        return;
    }

    // ETag check: find If-None-Match header
    const char* etag = generate_etag(path);
    for (int i=0;i<request->header_count;i++) {
        if (strcasecmp(request->headers[i].key, "If-None-Match")==0) {
            if (strcmp(request->headers[i].value, etag)==0) {
                // Not modified
                send_response(client_socket, 304, "text/plain", NULL, 0);
                log_request("0.0.0.0", request->method, request->path, 304);
                return;
            }
        }
    }

    char* buffer = NULL;
    size_t size = read_file(path, &buffer);
    if (size == 0 || !buffer) {
        const char* err = "<html><body><h1>500 Internal Server Error</h1></body></html>";
        send_response(client_socket, 500, "text/html", err, strlen(err));
        log_request("0.0.0.0", request->method, request->path, 500);
        if (buffer) free(buffer);
        return;
    }

    // Build headers including ETag and Cache-Control
    char headers_extra[512];
    add_common_headers(headers_extra, sizeof(headers_extra));
    char header[1024];
    int n = snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu\r\n"
            "ETag: %s\r\n"
            "Cache-Control: public, max-age=60\r\n"
            "%s"
            "\r\n",
            get_mime_type(path), size, etag ? etag : "\"0-0\"", headers_extra);
    write(client_socket, header, n);
    write(client_socket, buffer, size);
    log_request("0.0.0.0", request->method, request->path, 200);
    free(buffer);
}
