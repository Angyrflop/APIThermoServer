#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include "config_ip.h"

enum IpFlags{
        IP_FLAG_NONE = 0,
        IP_FLAG_INTERNAL = 1 << 0,
        IP_FLAG_WHITELISTED = 1 << 1,
        IP_FLAG_BLACKLISTED = 1 << 2,
        IP_FLAG_MALICIOUS = 1 << 3,
        IP_FLAG_FLAG_SCANNER = 1 << 4,
};

typedef struct
{
        bool isIpv6;
        union
        {
                uint32_t ipv4;
                uint8_t ipv6[16];
        } address;
        int callCount;
        uint8_t flags;
} ipHandler;

struct dynArray
{
        ipHandler *data;
        int size;
        int capacity;
};


void IpHandler_init(ipHandler *h)
{
        h->isIpv6 = false;
        h->flags = IP_FLAG_NONE;
        h->callCount = 0;
}

void dynArray_init(struct dynArray *arr)
{
        arr->capacity = 4;
        arr->size = 0;
        arr->data = (ipHandler *)malloc(arr->capacity * sizeof(ipHandler));
}

void dynArray_push(struct dynArray *arr, ipHandler entry)
{
        if (arr->size == arr->capacity) {
                arr->capacity *= 2;
                arr->data = (ipHandler *)realloc(arr->data, arr->capacity * sizeof(ipHandler));
        }
        arr->data[arr->size] = entry;
        arr->size++;
}

void dynArray_free(struct dynArray *arr)
{
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        arr->capacity = 0;
}

int main()
{
        const char testIpv6[] = "2a01:0db8:85a3:0000:0000:8a2e:0370:7334";
        const char testIpv4[] = "191.128.1.1";
        ipHandler entry1;
        IpHandler_init(&entry1);
        inet_pton(AF_INET, testIpv4, &entry1.address.ipv4);
        entry1.flags = IP_FLAG_BLACKLISTED | IP_FLAG_MALICIOUS;
        struct dynArray arr;
        dynArray_init(&arr);
        dynArray_push(&arr, entry1);
        for (int i = 0; i < arr.size; i++) {
                printf("entry[%d] flags: %d\n", i, arr.data[i].flags);
        }
        dynArray_free(&arr);
        // entry = (ipHandler*)malloc(sizeof(ipHandler) + size * sizeof(int));


        // char str6[INET6_ADDRSTRLEN];
        // char tmp[INET6_ADDRSTRLEN];
        // inet_ntop(AF_INET6, &entryv6.address.ipv6, str6, INET6_ADDRSTRLEN);
        // inet_ntop(AF_INET, &entryv4.address.ipv4, tmp, INET_ADDRSTRLEN);

        // printf("White: %s\n", WHITELISTED_FILE_PATH);
        // printf("Ban: %s\n", BAN_FILE_PATH);

        return 0;
}
