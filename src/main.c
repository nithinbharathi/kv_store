#include<stdio.h>
#include "wal.h"
#include "event_loop.h"


int main(){
    printf("***********************************************************************************\n"); 
    wal_init();
    event_loop();

    return 0;
}