#include "cell.h"

Cell newCell(int x, int y, unsigned char value){
    Options options = 0b111111111;
    if(value != 0) options = 1 << (value-1);
    return (Cell){.x = x, .y = y, .preset = (value != 0), .options = options};
}

Cell emptyCell(int x, int y) {
    return newCell(x, y, 0);
}

void printCell(Cell* cell){
    printf("Cell [%d, %d]: v = %d, options: ", cell->x, cell->y, cellValue(cell));
    printCellOptions(cell);
    // printf("\n");
    char* solve;
    switch(cell->solveBased){
        case 'l': solve = "(single option)"; break; // Last option
        case 'o': solve = "(only in group)"; break; // Only cell with option in group¨
        default: solve = "";
    }
    printf("%s %s\n", cell->preset ? " (preset)" : "", solve);
}

char cellOpCount(Cell* cell){
    char n = 0; 
    for(int i = 0; i < 9; i++){
        n += (cell->options >> i) & 1;
    }
    return n;
}

char cellValue(Cell* c){
    if(!cellSolved(c)) return 0; 

    for(char i = 0; i < 9; i++) if((c->options >> i) & 1) { 
        return i+1;
    }
}

char checkAllValues(Cell* cells[]){
    Options vals = 0;
    for(int i = 0; i < 9; i++){
        if(!cellSolved(cells[i])) return 0; // If cell has multiple options, it is not solved
        vals |= 1 << cells[i]->options; // Binary add the option to the value bitfield
    }

    //printf("%s\n", vals == 0b111111111 ? "Solved" : "Not solved");

    return vals == 0b111111111;
}

void printCellOptions(Cell* cell){
    for(int i = 0; i < 9; i++) if((cell->options >> i) & 1) printf("%d",  i+1); else printf(" ");
}

char cellSolved(Cell* cell){
    return !(cell->options&(cell->options-1)); // If a cells options are not a power of 2, it has multiple options and therefore isn't solved
}