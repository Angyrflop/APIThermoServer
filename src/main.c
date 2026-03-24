#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <unistd.h>
#include <stdio.h>
#include "server_config.h"
#include "ip_utils.h"
#include "hashmap.h"

static const char *HTML_INDEX = "<html><body><h1>Hello from microhttpd!</h1></body></html>";
static const char *HTML_404 = "<html><body><h1>404 Not Found</h1></body></html>";

static enum MHD_Result handler(
    void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
    const char *body;
    unsigned int status;
    hashmap_t *map = (hashmap_t *) cls;

    if (strcmp(url, HOMEPAGE) == 0) {
        printf("'/' Called\n");
        const union MHD_ConnectionInfo *info = 
            MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
        ipEntry entry;
        ipEntry_init(&entry);
        addIp(map, info);
        body = HTML_INDEX;
        status = MHD_HTTP_OK;
    } else if (strcmp(url, "/bathroom") == 0) {
        printf("'/bathrom' called\n");
        body = HTML_INDEX;
        status = MHD_HTTP_OK;
    } else {
        body = HTML_404;
        status = MHD_HTTP_NOT_FOUND;
    }

    struct MHD_Response *response = MHD_create_response_from_buffer(
            strlen(body), (void *)body, MHD_RESPMEM_PERSISTENT
    );

    MHD_add_response_header(response, "Content-Type", "text/html; charset=utf-8");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");

    enum MHD_Result ret = MHD_queue_response(connection, status, response);
    MHD_destroy_response(response);
    return ret;
}

int main() {
    hashmap_t map;
    hashmap_init(&map);
    struct MHD_Daemon *daemon = MHD_start_daemon(
            MHD_USE_INTERNAL_POLLING_THREAD,
            PORT,
            NULL, NULL,
            &handler, &map,
            MHD_OPTION_END
    );

    if (!daemon) {
        fprintf(stderr, "Failed to start daemon\n");
        return -1;
    }

    printf("Listening on port %d...\n", PORT);
    getchar();


    for (size_t i = 0; i < map.capacity; i++) {
        if (!map.slots[i].isOccupied)
            continue;
        printf("I[%zu]:\n", i);
        if (map.slots[i].isIpv6 == true) {
            char buf[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &map.slots[i].address.ipv6, buf, sizeof(buf));
            printf("IPv6: %s\n", buf);
        } else {
            char buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &map.slots[i].address.ipv4, buf, sizeof(buf));
            printf("IPv4: %s\n", buf);
        }
    }
    hashmap_free(&map);
    MHD_stop_daemon(daemon);
return 0;
}
