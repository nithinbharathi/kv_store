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

void parse(char *line, char *buf){
    char line_copy[1024];
    strcpy(line_copy, line);
    char *command = strtok(line," ");
    to_lower(command);

    if(strcmp(command, "put") == 0){
        char *key = strtok(NULL, " ");
        char *val = strtok(NULL, "");

        if(key == NULL || val == NULL){
            snprintf(buf, sizeof(buf), "Key or value cannot be NULL\n");
        } else {
            add_entry(line_copy);
            put(key,val);
            snprintf(buf, sizeof(buf), "added\n");
        }
    } else if(strcmp(command, "print") == 0){
        // collect print output into buf
        //snprintf(buf, sizeof(buf), "%s\n", collect_print_output()); 
    } else if(strcmp(command, "get") == 0){
        char *key = strtok(NULL, " ");
        char *val = get(key);
        if(val)
            snprintf(buf, sizeof(buf), "%s\n", val);
        else
            snprintf(buf, sizeof(buf), "NULL\n");
    } else {
        snprintf(buf, sizeof(buf), "invalid command\n");
    }
}
