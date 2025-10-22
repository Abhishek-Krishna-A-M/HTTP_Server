#include "../include/Launch.h"
#include "../include/Request.h"
#include "../include/Response.h"
#include "../include/Config.h"
#include "../include/Router.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

extern struct ServerConfig* g_config_pointer; // not used; we'll pass via server

void* client_thread(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[8192];
    int r = read(client_socket, buffer, sizeof(buffer)-1);
    if (r <= 0) { close(client_socket); return NULL; }
    buffer[r]=0;

    struct HttpRequest req = parse_request(buffer);
    printf("Request: %s %s Host:%s\n", req.method, req.path, req.host);

    // determine site root
    // server pointer will be passed via global in main; to keep code simple we will
    // read host → get site root via global config pointer stored in Launch.c
    extern struct ServerConfig* launch_config;
    const char* root = get_site_root(launch_config, req.host);

    // If path starts with /api, dispatch router
    if (strncmp(req.path, "/api/", 5) == 0) {
        if (!handle_api_route(client_socket, &req, launch_config)) {
            // not found
            const char* nf = "{\"error\":\"API Not Found\"}";
            send_response(client_socket, 404, "application/json", nf, strlen(nf));
        }
    } else {
        // static
        bool spa = true;
        // find spa mode for this host
        for (int i=0;i<launch_config->site_count;i++) {
            if (strcmp(launch_config->sites[i].host, req.host)==0) { spa = launch_config->sites[i].spa_mode; break;}
        }
        serve_response(client_socket, &req, root, spa);
    }

    close(client_socket);
    return NULL;
}

// global pointer used by Launch to let client threads access config
struct ServerConfig* launch_config = NULL;

void launch_server_with(struct Server* server) {
    launch_config = server->config;
    while (1) {
        int client_socket = accept(server->socket_fd, NULL, NULL);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        int *p = malloc(sizeof(int));
        *p = client_socket;
        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, p);
        pthread_detach(tid);
    }
}
