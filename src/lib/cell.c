#include "cell.h"

Cell newCell(int x, int y, int value){
    return (Cell){.x = x, .y = y, .val = value, .preset = (value != 0), .options = value ? 0 : 0b111111111};
}

Cell emptyCell(int x, int y) {
    return newCell(x, y, 0);
}

void printCell(Cell cell){
    printf("Cell [%d, %d]: v = %d, options:", cell.x, cell.y, cell.val);
    printCellOptions(&cell);
    printf("%s%s\n", cell.preset ? " (preset)" : "", cell.options || cell.preset ? "" : " (solved)");
}

int cellOpCount(Cell cell){
    int n = 0; 
    for(int i = 0; i < 9; i++){
        n += (cell.options >> i) & 1;
    }
    return n;
}

int checkAllValues(Cell* cells){
    int vals = 0;
    for(int i = 0; i < 9; i++){
        vals |= 1 << cells[i].val;
    }

    //printf("%s\n", vals == 0b111111111 ? "Solved" : "Not solved");

    return vals == 0b111111111;
}

void printCellOptions(Cell* cell){
    for(int i = 0; i < 9; i++) if((cell->options >> i) & 1) printf("%d",  i+1); else printf(" ");
}