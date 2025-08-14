#include<stdio.h>
#include "kv_store.h"
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>
#include "hash.h"

#define BUFFER_SIZE 16384U // should always be a power of 2 to support & instead of %

struct node *store[BUFFER_SIZE];

void put(const char *key, const char *val){
    uint32_t ind = hash(key) & (BUFFER_SIZE-1);
    struct node *curr = store[ind];

    while(curr != NULL){
        if(strcmp(curr->key, key) == 0){
            free(curr->val);
            curr->val = strdup(val);
            return;
        }
        curr = curr->next;
    }

    struct node *new_node = malloc(sizeof(struct node));
    new_node->key = strdup(key);
    new_node->val = strdup(val);
    new_node->next = store[ind];
    store[ind] = new_node;
}

char* get(const char *key){
    uint32_t ind = hash(key)&(BUFFER_SIZE-1);
    struct node *curr = store[ind];

    while(curr != NULL){
        if(strcmp(curr->key, key) == 0){
            return curr->val;
        }
        curr = curr->next;
    }

    return NULL;
}

void print(char* buf, size_t sz){
    size_t offset = 0;
    int written = snprintf(buf+offset,sz-offset, "Keys:\t\t\t\tValues:\n_______________________________________\n");
    for(int i = 0;i<BUFFER_SIZE;i++){
        if(store[i]){
            struct node *curr = store[i];
            while(curr != NULL){
                offset += written;
                written = snprintf(buf+offset,sz-offset, "%s\t\t\t\t%s\n",curr->key, curr->val);
                if (written < 0 || (size_t)written >= sz - offset) {
                    goto done; 
                }
                
                curr = curr->next;
            }
        }
    }

    done:
    buf[offset] = '\0';
}