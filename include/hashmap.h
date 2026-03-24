#ifndef HASHMAP_H
#define HASHMAP_H
#include <microhttpd.h>
#include <stdint.h>
#include <unistd.h>
#include "ip_utils.h"

    typedef struct {
        ipEntry *slots;
        size_t size;
        size_t capacity;
    } hashmap_t;
    
    #define FNV_OFFSET_BASIS 2166136261u
    #define FNV_PRIME 16777619u
    #define STARTING_CAPACITY 16
    #define LOAD_FACTOR_NUM 3
    #define LOAD_FACTOR_DEM 4

    uint32_t fnv1a_32(const void *data, size_t len);
    int addIp(hashmap_t *map, const union MHD_ConnectionInfo *info);
    void hashmap_free(hashmap_t *map);
    int hashmap_init(hashmap_t *map);
    int hashmap_insert(hashmap_t *map, ipEntry entry);

#endif /*HASHMAP_H*/
