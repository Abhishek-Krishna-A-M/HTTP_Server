#include "../include/Launch.h"
#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/Server.h"
#include "../include/Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048

struct ServerConfig* global_config; // used by threads

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    int read_size = read(client_socket, buffer, sizeof(buffer) - 1);

    if (read_size > 0) {
        buffer[read_size] = '\0';
        struct HttpRequest request = parse_request(buffer);

        printf("Received request:\nMethod: %s, Path: %s, Host: %s\n",
               request.method, request.path, request.host);

        // API or static
        if (strncmp(request.path, "/api/", 5) == 0) {
            handle_api_request(client_socket, &request);
        } else {
            const char* www_root = get_site_root(global_config, request.host);
            serve_response(client_socket, &request, www_root, global_config->spa_mode);
        }
    }

    close(client_socket);
    printf("Client disconnected\n");
    return NULL;
}

void launch_server(struct Server *server) {
    global_config = server->config;

    printf("Server listening on port %d...\n", server->port);

    while (1) {
        int client_socket = accept(server->socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Failed to accept client");
            continue;
        }
        printf("Client Connected!\n");

        pthread_t thread_id;
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&thread_id, NULL, handle_client, pclient);
        pthread_detach(thread_id);
    }
}
