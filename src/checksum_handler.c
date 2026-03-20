// Copyright (c) goes to Jan Oliver Quant
#include "ip_utils.h"
#include "checksum_handler.h"
#include <openssl/evp.h>
#include <sched.h>
#include <stdint.h>

int genChecksum(const dynArray *arr, uint8_t *checksum, unsigned int *checksumLen)
{
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        return -1;
    if (!EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_DigestUpdate(ctx, &arr->size, sizeof(int));
    EVP_DigestUpdate(ctx, arr->data, sizeof(ipEntry) * arr->size);
    if (!EVP_DigestFinal_ex(ctx, checksum, checksumLen)) {
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_MD_CTX_free(ctx);
    return 0;
}
