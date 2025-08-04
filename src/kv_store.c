#include<stdio.h>
#include "kv_store.h"
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<ctype.h>

#define BUFFER_SIZE 8U

struct node{
    char *key;
    char *val;
    struct node *next;
};

struct node *store[BUFFER_SIZE];

uint64_t hash(const char* key){
    uint64_t hash = 5381;
    int c;
    while((c = *key++)){
        hash = ((hash << 5)+hash)+c;
    }

    return hash;
}

void put(const char *key, const char *val){
    uint32_t ind = hash(key)%BUFFER_SIZE;
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

void to_lower(char *str){
    while(*str){
        *str = tolower(*str);
        str++;
    }
}

void print(){
    printf("KEYS\t\tVALUES\n");
    for(int i = 0;i<BUFFER_SIZE;i++){
        if(store[i]){
            struct node *curr = store[i];
            while(curr != NULL){
                printf("%s \t\t %s\n", curr->key, curr->val);
                curr = curr->next;
            }
        }
    }
}

int main(){
    printf("***********************************************************************************\n");
    char line[50];
    while(fgets(line, sizeof(line), stdin)){
        line[strcspn(line, "\n")] = '\0';
        char *command = strtok(line," ");
        to_lower(command);

       if(strcmp(command, "put") == 0){
            char *key = strtok(NULL, " ");
            char *val = strtok(NULL, " ");
            put(key,val);
       }else if(strcmp(command, "print") == 0){
            print();
       }else{
            printf("invalid command");
       }
     
    }
    return 0;
}