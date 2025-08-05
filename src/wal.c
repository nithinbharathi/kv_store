#include "wal.h"
#include <stdio.h>
#include<stdlib.h>
#include "utils.h"
#include<string.h>

void write_wal(const char *command){
    const char *dir_path = get_dir_path();
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "%s/.kv_log", dir_path);

    FILE *fp = fopen(file_path, "a");
    fprintf(fp, "%s\n",command);
    fclose(fp);
}

void read_wal(){
    const char *dir_path = get_dir_path();
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "%s/.kv_log", dir_path);

    FILE *fp = fopen(file_path, "r");
    if(!fp){
        perror("fopen");
        return;
    }

    char line[100];
    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line,"\n")] = '\0';
        printf("command read: %s\n", line);
    }
    fclose(fp);
}