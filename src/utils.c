#include "utils.h"
#include<ctype.h>
#include<errno.h>
#include<sys/stat.h>
#include <stdio.h>
#include<stdlib.h>

void to_lower(char *str){
    while(*str){
        *str = tolower(*str);
        str++;
    }
}

int create_directory(const char *path){
    if(mkdir(path, 0755) == -1){
        if(errno == EEXIST){
            return 0;
        }else{
            perror("mkdir");
            return -1;
        }
    }

    return 0;
}

char* get_dir_path(){
    const char *home = getenv("HOME");
    char dir_path[200];
    snprintf(dir_path, sizeof(dir_path), "%s/.kv_store", home);

    return dir_path;
}
