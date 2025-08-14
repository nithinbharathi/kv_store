#ifndef KV_STORE_H
#define KV_STORE_H
#include <stddef.h>
struct node{
    char *key;
    char *val;
    struct node *next;
};

void put(const char *key, const char *val);
void print(char* buf, size_t sz);
char* get(const char *key);

#endif