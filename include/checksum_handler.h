#ifndef CHECKSUM_HANDLER_H
#define CHECKSUM_HANDLER_H
#include "hashmap.h"
#include <stdint.h>
    int genChecksum(const hashmap_t *map, uint8_t *checksum, unsigned int *checksumLen);
#endif /*CHECKSUM_HANDLER_H*/
