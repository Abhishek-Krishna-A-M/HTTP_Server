#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[1024];
    int read_size = read(client_socket, buffer, sizeof(buffer) - 1);

    if (read_size > 0) {
        buffer[read_size] = '\0';
        printf("Request:\n%s\n", buffer);

        char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 38\r\n\r\n"
            "<html><body><h1>Hello from C!</h1></body></html>";
        write(client_socket, response, strlen(response));
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

        // Handle each client in a separate thread
        pthread_t thread_id;
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&thread_id, NULL, (void *(*)(void *))handle_client, pclient);
        pthread_detach(thread_id);
    }
}

