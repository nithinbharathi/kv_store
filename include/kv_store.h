#ifndef KV_STORE_H
#define KV_STORE_H

struct node{
    char *key;
    char *val;
    struct node *next;
};

void put(const char *key, const char *val);
void print();
char* get(const char *key);

#endif