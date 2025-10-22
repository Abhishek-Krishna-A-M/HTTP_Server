#include "../include/Proxy.h"
#include "../include/Response.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Memory {
    char* data;
    size_t size;
};

static size_t write_cb(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsz = size * nmemb;
    struct Memory* mem = (struct Memory*)userp;
    char* ptr = realloc(mem->data, mem->size + realsz + 1);
    if (!ptr) return 0;
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsz);
    mem->size += realsz;
    mem->data[mem->size] = 0;
    return realsz;
}

// proxy_request: forward request to url (basic GET) and return response body to client
void proxy_request(int client_socket, const char* base_url, const char* req_path) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        send_response(client_socket, 502, "application/json", "{\"error\":\"proxy init failed\"}", 26);
        return;
    }

    // build full URL (simple concat)
    char url[512];
    snprintf(url, sizeof(url), "%s%s", base_url, req_path);

    struct Memory chunk = {malloc(1), 0};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "C-Proxy/1.0");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        send_response(client_socket, 502, "application/json", "{\"error\":\"proxy failed\"}", 22);
        curl_easy_cleanup(curl);
        free(chunk.data);
        return;
    }

    // send proxied body
    send_response(client_socket, 200, "application/json", chunk.data, chunk.size);
    curl_easy_cleanup(curl);
    free(chunk.data);
}

