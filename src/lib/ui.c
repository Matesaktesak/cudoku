#include "ui.h"

int getNumber(char* message){
    printf("%s", message);
    int number;
    while(!scanf("%d", &number)){
        printf("Invalid input, please enter a number: ");
        clearInput();
    }

    return number;
}

void clearInput(){
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

FILE* getFile(){
    printf("Enter filename: ");
    char path[100] = "./saves/";
    do{
        char in = getc(stdin);
        if(in == '\n' || in == EOF) break;
        int pathlen = strlen(path);
        path[pathlen] = in;
        path[pathlen+1] = '\0';
    } while(1);

    strcat(path, ".cudoku");

    FILE* f = fopen(path, "r");
    if(!f){
        printf("Failed to open file\n");
        return getFile();
    }    
}