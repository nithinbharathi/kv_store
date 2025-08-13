#include<stdio.h>
#include "utils.h"
#include "event_loop.h"


int main(){
    printf("***********************************************************************************\n"); 
    create_directory(get_dir_path());
    event_loop();
    
    return 0;
}