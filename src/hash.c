#include<stdint.h>
#include "hash.h"

//DJB2 implementation
uint64_t hash(const char* key){
    uint64_t hash = 5381;
    int c;
    while((c = *key++)){
        hash = ((hash << 5)+hash)+c;
    }

    return hash;
}