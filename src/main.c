#include<stdio.h>
#include<stdlib.h>

#include "./lib/cell.h"
#include "./lib/playfield.h"

int main(){
    Cell* cells = malloc(sizeof(Cell) * 9 * 9);
    for(int i = 0; i < 9*9; i++){
        *(cells+i) = emptyCell(i%9, (int)i/9);
        //printCell(cells[i]);
    }

    FILE* f = fopen("./saves/preset1.cudoku", "r");
    if(!f){
        printf("Failed to open file\n");
        return 1;
    }

    int data[3];
    int count = 0;
    do{
        char input = fgetc(f);
        if(input == EOF) break;
        if(input == ',') count++;
        if(input == '\n'){
            cells[data[0] + data[1]*9] = newCell(data[0], data[1], data[2]);
            count = 0;
            continue;
        }
        if(input >= '0' && input <= '9'){
            data[count] = input - '0';
        }
    } while(1);
    fclose(f);

    for (int i = 0; i < 9 * 9; i++){
        //cells[i] = emptyCell(i % 9, (int)i / 9);
        printCell(cells[i]);
    }

    Playfield p = playfieldFromCells(cells);
    drawPlayfield(p);

    return 0;
}