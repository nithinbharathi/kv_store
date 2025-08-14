#include "kv_store.h"
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include "utils.h"
#include "wal.h"
#include <unistd.h>
#include "parse.h"

const size_t key_len = 64, val_len = 128;

void get_random_str(char *s, const size_t len){
    static char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i = 0;i<len;i++){
        s[i] = str[rand()%strlen(str)];
    }
    s[len] = '\0';
}

void benchmark_put(char key[][key_len+1], char val[][val_len+1], int epochs){
    double time_taken = 0.0;
    struct timespec start, end;

    for(int i = 0;i<epochs;i++){

        clock_gettime(CLOCK_MONOTONIC, &start);
        put(key[i], val[i]);
        clock_gettime(CLOCK_MONOTONIC, &end);

        time_taken += ((end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec))/1e9;
    }

    printf("avg time for put invoked %d times %.6f\n", epochs, time_taken/epochs);
}

void benchmark_get(char key[][key_len+1], int epochs){
    double time_taken = 0.0;
    struct timespec start, end;

    for(int i = 0;i<epochs;i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        get(key[i]);
        clock_gettime(CLOCK_MONOTONIC, &end);

        time_taken += ((end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec))/1e9;
    }

    printf("avg time for get invoked %d times %.6f\n", epochs, time_taken/epochs);
}

int main(){
    int epochs = 100;
    char keys[epochs][key_len+1], vals[epochs][val_len+1];
    srand(time(NULL));

    /*for(int i = 0;i<epochs;i++){
        get_random_str(keys[i], key_len);
        get_random_str(vals[i], val_len);
    }*/

    //benchmark_put(keys, vals, epochs);
    extern void read_wal(); 
    read_wal();
    char res[1024];
    char line[] = "GET tfgef";
    parse(line, res, false);
    

    return 0;
}