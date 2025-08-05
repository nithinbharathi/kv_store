#include "kv_store.h"
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>

void get_random_str(char *s, const size_t len){
    static char *str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i = 0;i<len;i++){
        s[i] = str[rand()%strlen(str)];
    }
    s[len] = '\0';
}

int main(){
    int ops = 10000;
    double time_taken = 0.0;
    const size_t key_len = 64, val_len = 128;
    char key[key_len+1], val[val_len+1];
    struct timespec start, end;
    srand(time(NULL));

    for(int i = 0;i<ops;i++){
        get_random_str(key, key_len);
        get_random_str(val, val_len);

        clock_gettime(CLOCK_MONOTONIC, &start);
        put(key, val);
        clock_gettime(CLOCK_MONOTONIC, &end);

        time_taken += ((end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec))/1e9;
    }

    printf("avg time for put invoked %d times %.6f", ops, time_taken/ops);
    print();

    return 0;
}