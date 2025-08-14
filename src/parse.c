#include<ctype.h>
#include<sys/stat.h>
#include "kv_store.h"
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include "parse.h"
#include "utils.h"
#include <unistd.h>
#include<stdlib.h>
#include "wal.h"
#include<stdbool.h>

void parse(char *line, char *buf, size_t buf_size, bool add_to_log){
    char line_copy[1024];
    strcpy(line_copy, line);
    char *command = strtok(line," ");
    to_lower(command);

    if(strcmp(command, "put") == 0){
        char *key = strtok(NULL, " ");
        char *val = strtok(NULL, "");

        if(key == NULL || val == NULL){
            snprintf(buf, buf_size, "Key or value cannot be NULL\n");
        } else {
            if(add_to_log)add_entry(line_copy);
            put(key,val);
            snprintf(buf, buf_size, "added\n");
        }
    } else if(strcmp(command, "print") == 0){
        // collect print output into buf
        print(buf, buf_size); 
    } else if(strcmp(command, "get") == 0){
        char *key = strtok(NULL, " ");
        char *val = get(key);
        if(val)
            snprintf(buf, buf_size, "%s\n", val);
        else
            snprintf(buf, buf_size, "NULL\n");
    } else {
        snprintf(buf, buf_size, "invalid command\n");
    }
}
