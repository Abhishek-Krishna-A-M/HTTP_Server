#include "../include/Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <unistd.h>

struct ServerConfig load_config(const char* path) {
    struct ServerConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.default_port = 8080;

    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Config file '%s' not found. Using defaults.\n", path);
        return cfg;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f); fseek(f, 0, SEEK_SET);
    char* data = malloc(size + 1);
    if (!data) { fclose(f); fprintf(stderr, "Out of memory\n"); exit(1); }
    fread(data,1,size,f); data[size]=0; fclose(f);

    cJSON* root = cJSON_Parse(data);
    free(data);
    if (!root) { fprintf(stderr, "Failed to parse config JSON\n"); return cfg; }

    cJSON* port = cJSON_GetObjectItem(root, "port");
    if (port && cJSON_IsNumber(port)) cfg.default_port = port->valueint;

    cJSON* sites = cJSON_GetObjectItem(root, "sites");
    if (sites && cJSON_IsArray(sites)) {
        int n = cJSON_GetArraySize(sites);
        if (n > MAX_SITES) n = MAX_SITES;
        for (int i = 0; i < n; ++i) {
            cJSON* s = cJSON_GetArrayItem(sites, i);
            cJSON* host = cJSON_GetObjectItem(s, "host");
            cJSON* rootp = cJSON_GetObjectItem(s, "root");
            cJSON* spa = cJSON_GetObjectItem(s, "spa");
            if (host && rootp) {
                strncpy(cfg.sites[cfg.site_count].host, host->valuestring, MAX_HOST_LEN-1);
                strncpy(cfg.sites[cfg.site_count].root, rootp->valuestring, MAX_PATH_LEN-1);
                cfg.sites[cfg.site_count].spa_mode = cJSON_IsTrue(spa);
                cfg.site_count++;
            }
        }
    }

    cJSON* apis = cJSON_GetObjectItem(root, "api_routes");
    if (apis && cJSON_IsArray(apis)) {
        int m = cJSON_GetArraySize(apis);
        if (m > MAX_ROUTES) m = MAX_ROUTES;
        for (int i = 0; i < m; ++i) {
            cJSON* r = cJSON_GetArrayItem(apis, i);
            cJSON* pathp = cJSON_GetObjectItem(r, "path");
            cJSON* meth = cJSON_GetObjectItem(r, "method");
            cJSON* resp = cJSON_GetObjectItem(r, "response");
            cJSON* proxy = cJSON_GetObjectItem(r, "proxy");
            if (pathp && meth) {
                strncpy(cfg.routes[cfg.api_count].path, pathp->valuestring, MAX_PATH_LEN-1);
                strncpy(cfg.routes[cfg.api_count].method, meth->valuestring, 7);
                if (resp && cJSON_IsString(resp))
                    strncpy(cfg.routes[cfg.api_count].response, resp->valuestring, sizeof(cfg.routes[cfg.api_count].response)-1);
                if (proxy && cJSON_IsString(proxy))
                    strncpy(cfg.routes[cfg.api_count].proxy_url, proxy->valuestring, sizeof(cfg.routes[cfg.api_count].proxy_url)-1);
                cfg.api_count++;
            }
        }
    }

    cJSON_Delete(root);
    return cfg;
}

const char* get_site_root(struct ServerConfig* config, const char* host) {
    if (!config || !host) return "www";
    for (int i=0;i<config->site_count;i++) {
        if (strcmp(config->sites[i].host, host) == 0) return config->sites[i].root;
    }
    return (config->site_count>0)?config->sites[0].root:"www";
}

const struct ApiRoute* find_api_route(struct ServerConfig* config, const char* path, const char* method) {
    if (!config) return NULL;
    for (int i=0;i<config->api_count;i++) {
        if (strcmp(config->routes[i].path, path)==0 && strcmp(config->routes[i].method, method)==0) {
            return &config->routes[i];
        }
    }
    return NULL;
}
