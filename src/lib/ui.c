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

int savePlayfield(Cell* p[], FILE* f){
    for(int i = 0; i < 9 * 9; i++){
        Cell c = **(p + i);
        if(cellSolved(&c)) fprintf(f, "%d,%d,%d\n", c.x, c.y, cellValue(&c));
    }
    fclose(f);

    return 1;
}

FILE* getFile(int writing){
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

    FILE* f = fopen(path, writing ? "w" : "r");
    if(!f && !writing){
        printf("Failed to open file\n");
        return getFile(0);
    }

    return f;
}