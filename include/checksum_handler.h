#ifndef CHECKSUM_HANDLER_H
#define CHECKSUM_HANDLER_H
#include "ip_utils.h"
#include <stdint.h>
    int genChecksum(const dynArray *arr, uint8_t *checksum, unsigned int *checksumLen);
#endif /*CHECKSUM_HANDLER_H*/
