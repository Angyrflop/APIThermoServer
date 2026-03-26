#include <microhttpd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hashmap.h"
#include "ip_utils.h"
#include "read_write_handler.h"
#include "server_config.h"
#include "mime.h"

enum MHD_Result answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **req_cls)
{
    const char *page  = "<html><body>Hello, browser!</body></html>";
    const char *style = "\0";
    struct MHD_Response *response;
    enum MHD_Result ret;
    const char *mimeType = mime_from_ext("html");

    if (strcmp(method, "GET") != 0)
        return MHD_NO;
    
    if (strcmp(url, "/") == 0) {
        mimeType = mime_from_ext("html");
    }
    
    if (strcmp(url, "/style.css") == 0) {
        page = style;
        mimeType = mime_from_ext("css");
    }

    response = MHD_create_response_from_buffer (strlen (page),
                                            (void*) page, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(response, "Content-Type", mimeType);
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);

    return ret;
}

int main() {
    hashmap_t map;
    hashmap_init(&map);
    
    if ( READ_IP_ON_STARTUP == true) {
        if (readIPFile(&map) != 0) {
            printf("[ STARTUP]: Failed to read ips.bin!!!\n");
            if ( ABORT_IF_FILE_IS_EMPTY == true)
                return -1;
        }
    } else {
        printf("[ STARTUP]: Not reading ips.bin\n");
    }
    
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
            &answer_to_connection, NULL, MHD_OPTION_END);
    if (daemon == NULL)
        return -1;

    // Start daemon

    getchar();

    if (WRITE_IP_DURING_SHUTDOWN == true) {
        if (writeIPFile(&map) != 0) {
            printf("[SHUTDOWN]: Failed to write ips.bin!!!\n");
            return -1;
        }
    } else {
        printf("[SHUTDOWN]: Not writting ips.bin\n");
    }
    MHD_stop_daemon(daemon);
    hashmap_free(&map);
    return 0;
}
