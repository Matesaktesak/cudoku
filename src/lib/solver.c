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

int onlyInReg(Cell* reg[]){
    int reduction = 0;
    for(int n = 1; n <= 9; n++){ // For every cell in the region
        if(cellSolved(reg[n])) continue; // Skip solved cells
        // Binary sum the other cells options
        Options sum = 0;
        for(int i = 0; i < 9; i++) if(i != n) sum |= reg[i]->options;

        // If the cell has an option that is not in the sum
        if(reg[n]->options & ~sum){
            reg[n]->options &= ~sum; // Remove all other options
            reg[n]->solveBased = 'o'; // Set the diagnostic type to 'only in region'

            // Remove this option from all other cells in the region (and report the result as if done by this op... xd)
            reduction += removeOptionFromGroup(reg[n]->options, reg);

            reduction++; // Report the solved cell
        }
    }

    return reduction;
}