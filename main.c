#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

#define ARGC_ERROR 1
#define BUFLEN 256
#define MAXSTRINGS 1024


int main(int argc, const char* argv[]){

    char buffer[BUFLEN];
    char *file1[MAXSTRINGS], *file2[MAXSTRINGS];

    //Allocate memory for buf and strings that hold the files

    memset(buffer, 0, sizeof(buffer));
    memset(file1,  0, sizeof(file1));
    memset(file2,  0, sizeof(file2));

    if (argc != 3){
        printf("Please provide a valid argument: ./diff [arg] [file1] [file2] \n");
        exit(ARGC_ERROR);
    }
    
    while(argc > 3){
        int i = 1;
        arghandler(argv[i]);
        argc--;
        i++;
    }

    FILE *fin1 = openfile(argv[argc-1],"r");
    FILE *fin2 = openfile(argv[argc-2],"r");

    int 

}