#include "solver.h"

char logicSolve(Playfield* p){
    if(p->solvedCells == 81) return 1; // If the playfield is already solved, return 1
    removeOptions(p);
    
    int reduction = 1;
    while(reduction && p->solvedCells < 81){
        reduction = 0;
        
        // Try to find cells with only one option left in rows, cols and blocks
        for(int i = 0; i < 9; i++){
            reduction += onlyInReg(p->rows[i]);
            if(p->solvedCells + reduction >= 81) goto bigLoopExit; // Break the outer loop, if the field is solved

            reduction += onlyInReg(p->cols[i]);
            if(p->solvedCells + reduction >= 81) goto bigLoopExit; 

            reduction += onlyInReg(p->blocks[i]);
            if(p->solvedCells + reduction >= 81) goto bigLoopExit; 
        }

        p->solvedCells += reduction;
        reduction += removeOptions(p); // removeOptions already adds its result to p->solvedCells...
    }

    bigLoopExit:
    return checkSolved(p);
}

char recurse(Playfield* p);

char bruteSolve(Playfield* p){
    if(logicSolve(p)) return 1; // If the playfield is solvable using logic, do so and return 1

    // Else step into recursion (brute force)
    return recurse(p);
}

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
    field->solvedCells += totalReduction;
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

char recurse(Playfield* p){
    char minimumCount = 2;
    for(int i = 0; i < 81; i++){ // For every cell
        if(cellSolved(p->cells[i])) continue;   // Skip solved cells
        if(cellOpCount(p->cells[i]) == minimumCount) { // If the cell has the minimum number of options
            for(int n = 0; n < 9; n++){ // For every option
                if(!(p->cells[i]->options & (1 << n))) continue; // Only try options that are still available

                Playfield copy = clonePlayfield(p); // Prepare a deep copy of the playfield
                copy.cells[i]->options = 1 << n;  // Presume this option is correct
                if(bruteSolve(&copy)){  // Try to brute solve the rest of the copied field - thus recursing
                    *p = copy;  // If it works, copy the solved field to the original
                    return 1;   // Return 1 and so break the entire recursion
                }
            }
        }
    }

    return 0;
}