#ifndef READ_WRITE_HANDLER_H
#define READ_WRITE_HANDLER_H
    #include "hashmap.h"
    int writeIPFile(const hashmap_t *map);
    int readIPFile(hashmap_t *map);
    int getStatic(char **file, char *fileName);
#endif /*READ_WRITE_HANDLER_H*/
