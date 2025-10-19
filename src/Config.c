#include "../include/Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"

struct ServerConfig load_config(const char* path) {
    struct ServerConfig config = {8080, true, false, 1, 0};

    FILE* f = fopen(path, "r");
    if (!f) {
        printf("Config file not found, using defaults.\n");
        return config;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    if (!json) return config;

    cJSON* port = cJSON_GetObjectItem(json, "port");
    if (port) config.port = port->valueint;

    cJSON* spa = cJSON_GetObjectItem(json, "spa_mode");
    if (spa) config.spa_mode = spa->valueint;

    cJSON* remote = cJSON_GetObjectItem(json, "remote_access");
    if (remote) config.remote_access = remote->valueint;

    cJSON* sites = cJSON_GetObjectItem(json, "sites");
    if (sites && cJSON_IsArray(sites)) {
        int count = cJSON_GetArraySize(sites);
        if (count > MAX_SITES) count = MAX_SITES;
        config.site_count = count;
        for (int i = 0; i < count; i++) {
            cJSON* site = cJSON_GetArrayItem(sites, i);
            cJSON* host = cJSON_GetObjectItem(site, "host");
            cJSON* root = cJSON_GetObjectItem(site, "root");
            if (host && root) {
                strncpy(config.sites[i].host, host->valuestring, MAX_PATH_LEN);
                strncpy(config.sites[i].root, root->valuestring, MAX_PATH_LEN);
            }
        }
    }

    cJSON_Delete(json);
    return config;
}
