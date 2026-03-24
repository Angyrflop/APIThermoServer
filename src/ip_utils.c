// Copyright (c) goes to Jan Oliver Quant
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ip_utils.h"

void ipEntry_init(ipEntry *entry)
{
        memset(entry, 0, sizeof(ipEntry));
        entry->isIpv6 = false;
        entry->isOccupied = false;
        entry->flags = IP_FLAG_NONE;
}

