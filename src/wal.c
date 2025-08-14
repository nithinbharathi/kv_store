#include "wal.h"
#include <stdio.h>
#include<stdlib.h>
#include "utils.h"
#include<string.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "parse.h"

void start_wal_thread();
void read_wal();

#define BUF_SIZE 1024

static char wal_buffer[BUF_SIZE];
static size_t wal_len;
static pthread_mutex_t wal_mutex = PTHREAD_MUTEX_INITIALIZER;
static int wal_fd = -1;
static volatile int stop_flush_thread = 0;

pthread_t flush_thread;


void wal_init() {
    wal_len = 0;
    const char *dir_path = get_dir_path();
    create_directory(dir_path);
    char file_path[200];
    snprintf(file_path, sizeof(file_path), "%s/.kv_log", dir_path);

    wal_fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (wal_fd == -1) {
        perror("Failed to open WAL file");
    }
    read_wal();
    start_wal_thread();
}

void write_wal(){
    if (wal_fd == -1 || wal_len == 0) return;
    
    write(wal_fd, wal_buffer, wal_len);
    fsync(wal_fd);
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

    char line[1024];
    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line,"\n")] = '\0';
        char response_buffer[1024];
        parse(line, response_buffer, false);
    }
    fclose(fp);
}

void add_entry(const char *entry) {
    pthread_mutex_lock(&wal_mutex);
    size_t entry_len = strlen(entry);
    if (wal_len + entry_len + 1 >= BUF_SIZE) {
       write_wal();  
       wal_len = 0;
    }

    memcpy(wal_buffer + wal_len, entry, entry_len);
    wal_len += entry_len;
    wal_buffer[wal_len++] = '\n';

    pthread_mutex_unlock(&wal_mutex);
}

void* flush_buffer(void *arg) {
    while (!stop_flush_thread) {
        sleep(5);  // flush every 5 secs
        pthread_mutex_lock(&wal_mutex);
        write_wal();  
        wal_len = 0;       
        pthread_mutex_unlock(&wal_mutex);
    }
    return NULL;
}

void wal_close() {
    pthread_mutex_lock(&wal_mutex);
    if (wal_len > 0) {
        write_wal();
        wal_len = 0;
    }

    if (wal_fd != -1) {
        close(wal_fd);
        wal_fd = -1;
    }

    pthread_mutex_unlock(&wal_mutex);
    pthread_mutex_destroy(&wal_mutex);
    stop_flush_thread = 1;
}

void start_wal_thread(){
    pthread_create(&flush_thread, NULL, flush_buffer, NULL);
    pthread_detach(flush_thread);
}