#include "solver.h"

char logicSolve(Playfield* p){
    if(p->solvedCells == 81) return 1; // If the playfield is already solved, return 1
    removeOptions(p, 1);

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
        reduction += removeOptions(p, 1); // removeOptions already adds its result to p->solvedCells...
    }

    bigLoopExit:
    return checkSolved(p);
}

char recurse(Playfield** field);

char bruteSolve(Playfield** p){
    if(logicSolve(*p)) return 1; // If the playfield is solvable using logic, do so and return 1

    // Else step into recursion (brute force)
    return recurse(p);
}

int removeOptionFromGroup(Options o, Cell* cells[], char setSolved){ // Removes an option from a group of cells
    int solved = 0; // Number of cells solved (for reporting)

    for(int i = 0; i< 9; i++){ // For each cell in the group
        Cell* cell = cells[i]; // Just a 'macro', hopefully the compiler optimizes this
        if(cellSolved(cell)) continue; // Skip solved cells

        cell->options &= ~o; // Remove option from cell

        if(cellSolved(cell) & setSolved){ // If cell has only one option left
            cell->solveBased = 'l';
            solved++;
        }
    }

    return solved;
}

int removeOptions(Playfield* field, char setSolved){
    //printf("Removing invalid options...\n");

    int reduction = 1;
    int totalReduction = 0;

    while(reduction) {
        reduction = 0;

        for(char i = 0; i < 9*9; i++) if(setSolved ? cellSolved(field->cells[i]) : field->cells[i]->solveBased != 'n'){
            const Cell* c = field->cells[i];
            reduction += removeOptionFromGroup(c->options, field->rows[c->y], setSolved);
            reduction += removeOptionFromGroup(c->options, field->cols[c->x], setSolved);
            reduction += removeOptionFromGroup(c->options, field->blocks[(c->x/3) + (c->y/3)*3], setSolved);
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
            reduction += removeOptionFromGroup(reg[n]->options, reg, 1);

            reduction++; // Report the solved cell
        }
    }

    return reduction;
}

/*
    The recursive approach is as follows:
    1. Find the cell with the least number of options
        - if there are multiple, pick the first one
        - if there are none with few options, try guessing a cell with more than 2 options
    2. Pick the an option and create a duplicate field with that option set
    3. Try to solve the duplicate field using logic
    4. Recourse if the duplicate field is not solved

    If at any point the duplicate field is solved, return 1, break the recursion
    And set the original field to the solved duplicate field (swap pointers)
*/

//int recourseCounter = 0;  // Debugging/logging variable
char recurse(Playfield** field){
    //printf("Recoursing for the %d. time\n", ++recourseCounter); // Just for debugging

    Playfield* p = *field; // Just a 'macro', hopefully the compiler optimizes this

    const char minimumCount = 2; // TODO: Make this dynamic - I sort of hope, there is no logicaly solvable field with no cell with just two options
    for(int i = 0; i < 81; i++){ // For every cell
        if(cellSolved(p->cells[i])) continue;   // Skip solved cells

        if(cellOpCount(p->cells[i]) == minimumCount) { // If the cell has the minimum number of options
            for(int n = 0; n < 9; n++){ // For every option
                if(!(p->cells[i]->options & (1 << n))) continue; // Only try options that are still available

                Playfield* copy = clonePlayfield(p); // Prepare a deep copy of the playfield
                copy->cells[i]->options = 1 << n;   // Guess this option is correct
                if(bruteSolve(&copy)){              // Try to brute solve the rest of the copied field - thus recursing
                    copy->cells[i]->solveBased = 'b'; // If it works, set the diagnostic type to 'brute force'
                    copy->solvedCells++; // And report the solved cell
                    
                    *field = copy;  // If it worked, copy the solved field to the original
                    return 1;   // Return 1 and so break the recursion
                }
                free(copy); // If it doesn't work, free the copy
            }
        }
    }

    // If we got here, all options and guesses have been tried and none worked, therefore:
    printf("Playfield cannot be solved\n");
    *field = p; // If the field cannot be solved, set the original field to the original field
    return 0;
}