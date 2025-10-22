#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define MAX_SITES 10
#define MAX_ROUTES 200
#define MAX_HOST_LEN 128
#define MAX_PATH_LEN 512

struct Site {
    char host[MAX_HOST_LEN];
    char root[MAX_PATH_LEN];
    bool spa_mode;
};

struct ApiRoute {
    char path[MAX_PATH_LEN];
    char method[8];
    char response[2048];      // static JSON (optional)
    char proxy_url[256];      // optional proxy target
    char handler_name[64];    // placeholder for future C handlers
};

struct ServerConfig {
    int site_count;
    struct Site sites[MAX_SITES];
    int api_count;
    struct ApiRoute routes[MAX_ROUTES];
    int default_port;
};

struct ServerConfig load_config(const char* path);
const char* get_site_root(struct ServerConfig* config, const char* host);
const struct ApiRoute* find_api_route(struct ServerConfig* config, const char* path, const char* method);

#endif
