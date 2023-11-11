#include<stdio.h>
#include<stdlib.h>

#include "./lib/cell.h"
#include "./lib/playfield.h"
#include "./lib/solver.h"

Cell* loadCells(char* path);

int main(){
    Cell* cells = loadCells("./saves/preset2.cudoku");

    // for (int i = 0; i < 9 * 9; i++){
    //     //cells[i] = emptyCell(i % 9, (int)i / 9);
    //     printCell(cells[i]);
    // }

    Playfield p = playfieldFromCells(cells);
    drawPlayfield(p);

    removeOptions(&p);

    drawPlayfield(p);

    printf("Only in reg strategy:\n");

    int reduction = 1;
    while(reduction){
        reduction = 0;
        for(int i = 0; i < 9; i++){
            reduction += onlyInReg(p.rows[i]);
        }
        for(int i = 0; i < 9; i++){
            reduction += onlyInReg(p.cols[i]);
        }
        for(int i = 0; i < 9; i++){
            reduction += onlyInReg(p.blocks[i]);
        }
        printf("Reduction: %d\n", reduction);
    }

    for (int i = 0; i < 9 * 9; i++){
        //cells[i] = emptyCell(i % 9, (int)i / 9);
        printCell(&cells[i]);
    }

    drawPlayfield(p);

    printf("%s\n", checkSolved(p) ? "Playfield is solved" : "Playfield is not solved");

    free(cells);

    return 0;
}

/*
    @brief Loads a cell array from a file
    @param path: path to file to load from
    @return: array of cells or null if failed
*/
Cell* loadCells(char* path){
    Cell* cells = malloc(sizeof(Cell) * 9 * 9);
    if(cells == NULL){
        printf("Failed to allocate memory for cells\n");
        return NULL;
    }

    for(int i = 0; i < 9*9; i++){
        *(cells+i) = emptyCell(i%9, (int)i/9);
        // printCell(&cells[i]);
    }

    FILE* f = fopen(path, "r");
    if(!f){
        printf("Failed to open file\n");
        return NULL;
    }

    int data[3];
    int count = 0;
    do{
        char input = fgetc(f);
        if(input == ',') count++;
        if(input == '\n' || input == EOF){
            cells[data[0] + data[1]*9] = newCell(data[0], data[1], data[2]);
            // printCell(cells[data[0] + data[1]*9]);
            count = 0;
            if(input == EOF) break;
            continue;
        }
        if(input >= '0' && input <= '9'){
            data[count] = input - '0';
        }
    } while(1);

    fclose(f);

    return cells;
}