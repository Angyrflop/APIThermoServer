// Copyright (c) goes to Jan Oliver Quant
#include <microhttpd.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ip_utils.h"
#include "hashmap.h"

uint32_t fnv1a_32(const void *data, size_t len)
{
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t hash = FNV_OFFSET_BASIS;
    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

int hashmap_init(hashmap_t *map)
{
    map->capacity = STARTING_CAPACITY;
    map->size = 0;
    map->slots = (ipEntry *)calloc(map->capacity, sizeof(ipEntry));
    if (map->slots == NULL)
        return -1;
    return 0;
}

void hashmap_free(hashmap_t *map)
{
    free(map->slots);
    map->size = 0;
    map->capacity = 0;
}

static const void *ipEntry_key(const ipEntry *entry)
{
    return entry->isIpv6
        ?   (const void *)&entry->address.ipv6
        :   (const void *)&entry->address.ipv4;
}

static size_t ipEntry_keylen(const ipEntry *entry)
{
    return entry->isIpv6
        ?   sizeof(struct in6_addr)
        :   sizeof(struct in_addr);
}

int hashmap_resize(hashmap_t *map)
{
    size_t tmpCapacity = map->capacity * 2;
    ipEntry *tmp = calloc(tmpCapacity, sizeof(ipEntry));
    if (!tmp) {
        fprintf(stderr, "[     MAP]: hashmap_resize: allocation failed, requested %zu slots\n", tmpCapacity);
        return -1;
    }
    for (size_t i = 0; i < map->capacity; i++) {
        if (!map->slots[i].isOccupied)
            continue;

        const void *key = ipEntry_key(&map->slots[i]);
        size_t klen = ipEntry_keylen(&map->slots[i]);
        size_t index = fnv1a_32(key, klen) % tmpCapacity;

        while (tmp[index].isOccupied)
            index = (index + 1) % tmpCapacity;

        tmp[index] = map->slots[i];
    }

    free(map->slots);
    map->slots = tmp;
    map->capacity = tmpCapacity;
    return 0;
}

int hashmap_insert(hashmap_t *map, ipEntry entry)
{
    if (map->size * LOAD_FACTOR_DEM >= map->capacity *LOAD_FACTOR_NUM) {
        if (hashmap_resize(map) != 0)
            return -1;
    }
    
    const void *key = ipEntry_key(&entry);
    size_t klen = ipEntry_keylen(&entry);
    size_t index = fnv1a_32(key, klen) % map->capacity;

    while (map->slots[index].isOccupied)
        index = (index + 1) % map->capacity;

    entry.isOccupied = true;
    map->slots[index] = entry;
    map->size++;
    return 0;
}

int hashmap_contains(hashmap_t *map, ipEntry entry)
{
    const void *key = ipEntry_key(&entry);
    size_t klen = ipEntry_keylen(&entry);
    size_t index = fnv1a_32(key, klen) % map->capacity;

    while (map->slots[index].isOccupied) {
        const void *slotKey = ipEntry_key(&map->slots[index]);
        size_t slotKlen = ipEntry_keylen(&map->slots[index]);
        
        if (map->slots[index].isIpv6 == entry.isIpv6 &&
                slotKlen == klen &&
                memcmp(slotKey, key, klen) == 0)
        {
            return 1;
        }
        index = (index + 1) % map->capacity;
    }
    return 0;
}

int addIp(hashmap_t *map, const union MHD_ConnectionInfo *info)
{
    if (info == NULL)
        return -1;
    ipEntry entry;
    ipEntry_init(&entry);
    struct sockaddr *addr = info->client_addr;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
        entry.address.ipv4 = addr4->sin_addr;
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
        entry.isIpv6 = true;
        entry.address.ipv6 = addr6->sin6_addr;
    } else {
        fprintf(stderr, "[     MAP]:Unkown IP address family tried to connect.\n");
        return -1;
    }

    if (hashmap_contains(map, entry))
        return 0;

    return hashmap_insert(map, entry);
}
