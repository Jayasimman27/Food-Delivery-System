#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"

//#include "restaurant_login.c"
int main(){
    int option;
    printf("1. For user Login (or) Registration\n");
    printf("2. For Restaurant Login (or) Registration\n\nChoose Your Option : ");
    scanf("%d",&option);

    if (option == 1){
        file_open();
    }

    if (option == 2){
        rest_log_reg();
    }
}