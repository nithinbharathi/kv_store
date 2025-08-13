#include<ctype.h>
#include<sys/stat.h>
#include "kv_store.h"
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include "parse.h"
#include "utils.h"
#include <unistd.h>

void parse(int client_fd, char *line){
    char *command = strtok(line," ");
    to_lower(command);

    char buf[1024]; // buffer for response

    if(strcmp(command, "put") == 0){
        char *key = strtok(NULL, " ");
        char *val = strtok(NULL, "");

        if(key == NULL || val == NULL){
            snprintf(buf, sizeof(buf), "Key or value cannot be NULL\n");
        } else {
            printf("%s\t%s",key,val);
            put(key,val);
            snprintf(buf, sizeof(buf), "added\n");
        }
    } else if(strcmp(command, "print") == 0){
        // collect print output into buf
        //snprintf(buf, sizeof(buf), "%s\n", collect_print_output()); 
    } else if(strcmp(command, "get") == 0){
        char *key = strtok(NULL, " ");
        char *val = get(key);
        printf("%s\t%s", key, val);
        if(val)
            snprintf(buf, sizeof(buf), "%s\n", val);
        else
            snprintf(buf, sizeof(buf), "NULL\n");
    } else {
        snprintf(buf, sizeof(buf), "invalid command\n");
    }

    write(client_fd, buf, strlen(buf));
}
