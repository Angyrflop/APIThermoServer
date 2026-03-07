#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>

enum IpFlags{
        IP_FLAG_NONE = 0,
        IP_FLAG_INTERNAL = 1 << 1,
        IP_FLAG_WHITELISTED = 1 << 2,
        IP_FLAG_BLACKLISTED = 1 << 3,
        IP_FLAG_MALICIOUS = 1 << 4,
        IP_FLAG_FLAG_SCANNER = 1 << 5,
};

struct ipHandler
{
        bool isIpv6;
        union
        {
                uint32_t ipv4;
                uint8_t ipv6[16];
        } address;
        int callCount;
        uint8_t flags;
};

void IpHandler_init(struct ipHandler *h)
{
        h -> isIpv6 = false;
        h -> flags = IP_FLAG_NONE;
        h -> callCount = 0;
}

int main()
{
        const char testIpv6[] = "2a01:0db8:85a3:0000:0000:8a2e:0370:7334";
        const char testIpv4[] = "191.128.1.1";
        struct sockaddr_in sa;
        uint32_t bin;
        struct ipHandler entryv6;
        struct ipHandler entryv4;
        IpHandler_init(&entryv4);
        IpHandler_init(&entryv6);
        entryv6.isIpv6 = true;

        inet_pton(AF_INET, testIpv4, &entryv4.address.ipv4);
        inet_pton(AF_INET6, testIpv6, &entryv6.address.ipv6);

        char str6[INET6_ADDRSTRLEN];
        char tmp[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &entryv6.address.ipv6, str6, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET, &entryv4.address.ipv4, tmp, INET_ADDRSTRLEN);

        printf("ipv4: %s\n", tmp);
        printf("ipv6: %s\n", str6);

        return 0;
}
