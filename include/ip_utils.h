#ifndef IP_UTILS_H
#define IP_UTILS_H
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <microhttpd.h>
#include <unistd.h>

enum IpFlags{
        IP_FLAG_NONE = 0,
        IP_FLAG_INTERNAL = 1 << 0,      /*1*/
        IP_FLAG_WHITELISTED = 1 << 1,   /*2*/
        IP_FLAG_BLACKLISTED = 1 << 2,   /*4*/
        IP_FLAG_MALICIOUS = 1 << 3,     /*8*/
        IP_FLAG_SCANNER = 1 << 4,       /*16*/
};

typedef struct
{
    union
    {
        struct in_addr ipv4;
        struct in6_addr ipv6;
    } address;
    bool isIpv6;
    bool isOccupied;
    uint8_t flags;
} ipEntry;

void ipEntry_init(ipEntry *entry);
#endif /*IP_UTILS_H*/
