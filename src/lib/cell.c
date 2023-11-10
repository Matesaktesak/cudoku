#include "cell.h"

Cell newCell(int x, int y, int value){
    return (Cell){.x = x, .y = y, .val = value, .preset = (value != 0), .options = value ? 0 : 0b111111111};
}

Cell emptyCell(int x, int y) {
    return newCell(x, y, 0);
}

void printCell(Cell cell){
    printf("Cell [%d, %d]: v = %d%s%s\n", cell.x, cell.y, cell.val, cell.preset ? " (preset)" : "", cell.options || cell.preset ? "" : " (solved)");
}

int cellOpCount(Cell cell){
    int n; 
    for(int i = 0; i < 9; i++){
        n += (cell.options >> i) & 1;
    }
    return n;
}

int removeOption(int o, Cell* cells){
    int solved = 0;

    for(int i = 0; i< 9; i++){
        Cell* cell = (cells + i);
        if(!cell->val) continue; // Skip solved cells

        cell->options &= ~(1 << o); // Remove option from cell

        if(cellOpCount(*cell) == 1){ // If cell has only one option left
            int val = 0;
            for(; val < 9; val++){
                if((cell->options >> val) & 1) break;
            }
            cell->val = val;
            cell->options = 0;
            cell->solveBased = 'l';
            solved++;
        }
    }

    return solved;
}