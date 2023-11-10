#include "solver.h"

int removeOptionFromGroup(Options o, Cell* cells[]){ // Removes an option from a group of cells
    int solved = 0; // Number of cells solved (for reporting)

    for(int i = 0; i< 9; i++){ // For each cell in the group
        Cell* cell = cells[i]; // Just a 'macro', hopefully the compiler optimizes this
        if(cellSolved(cell)) continue; // Skip solved cells

        cell->options &= ~o; // Remove option from cell

        if(cellSolved(cell)){ // If cell has only one option left
            cell->solveBased = 'l';
            solved++;
        }
    }

    return solved;
}

int removeOptions(Playfield* field){
    int reduction = 1;
    int totalReduction = 0;

    while(reduction) {
        reduction = 0;

        for(char i = 0; i < 9*9; i++) if(cellSolved(field->cells[i])){
            const Cell* c = field->cells[i];
            reduction += removeOptionFromGroup(c->options, field->rows[c->y]);
            reduction += removeOptionFromGroup(c->options, field->cols[c->x]);
            reduction += removeOptionFromGroup(c->options, field->blocks[(c->x/3) + (c->y/3)*3]);
        }

        //printf("Reduction: %d\n", reduction);
        totalReduction += reduction;
    }

    return totalReduction;
}