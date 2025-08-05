#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<sys/stat.h>
#include "kv_store.h"
#include "utils.h"
#include<errno.h>

#define PATH "/home/.kv_store"

int main(){
    printf("***********************************************************************************\n");
    char line[50];
    create_directory(get_dir_path());

    while(fgets(line, sizeof(line), stdin)){
        line[strcspn(line, "\n")] = '\0';
        char *command = strtok(line," ");
        to_lower(command);

       if(strcmp(command, "put") == 0){
            char *key = strtok(NULL, " ");
            char *val = strtok(NULL, "");

            if(key == NULL || val == NULL){
                fprintf(stderr, "Key or value cannot NULL\n");
                continue;
            }
            put(key,val);
            printf("added\n");
       }else if(strcmp(command, "print") == 0){
            print();
       }else if (strcmp(command, "get") == 0){
            char *key = strtok(NULL, " ");
            printf("%s\n", get(key));
       }
       else{
            printf("invalid command\n");
       }
       printf("\n");
    }
    return 0;
}