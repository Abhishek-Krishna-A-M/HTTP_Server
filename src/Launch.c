#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/Launch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;  // Declare client_socket at the top
    free(arg);                        // free memory allocated for client socket

    char buffer[BUFFER_SIZE];         // Declare buffer here
    int read_size = read(client_socket, buffer, sizeof(buffer) - 1);

    if (read_size > 0) {
        buffer[read_size] = '\0';

        // Parse the HTTP request
        struct HttpRequest request = parse_request(buffer);  // Declare request here

        printf("Received request:\n");
        printf("Method: %s, Path: %s\n", request.method, request.path);

        // Serve the requested file
        serve_response(client_socket, &request, "www", true);
    }

    close(client_socket);
    printf("Client disconnected\n");

    return NULL;
}

void launch_server(struct Server *server) {
    printf("Server listening on port %d...\n", server->port);
    while (1) {
        int client_socket = accept(server->socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Failed to accept client");
            continue;
        }

        printf("Client Connected!\n");


        // Handle each client in a separate thread
        pthread_t thread_id;
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&thread_id, NULL,handle_client, pclient);
        pthread_detach(thread_id);
    }
}

