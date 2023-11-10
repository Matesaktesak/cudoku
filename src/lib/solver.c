#include "solver.h"

int removeOptionFromGroup(int o, Cell* cells[]){
    if(o == 0) return 0;
    int solved = 0;

    for(int i = 0; i< 9; i++){
        Cell* cell = cells[i];
        if(cell->val != 0) continue; // Skip solved cells

        cell->options &= ~(1 << o-1); // Remove option from cell

        if(cellOpCount(*cell) == 1){ // If cell has only one option left
            int val = 0;
            for(; val < 9; val++){
                if((cell->options >> val) & 1) break;
            }
            cell->val = val+1;
            cell->options = 0;
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
        for(int i = 0; i < 9; i++) for(int j = 0; j < 9; j++) {
            reduction += removeOptionFromGroup(field->rows[i][j]->val, field->rows[i]);
            reduction += removeOptionFromGroup(field->cols[i][j]->val, field->cols[i]);
            reduction += removeOptionFromGroup(field->blocks[i][j]->val, field->blocks[i]);
        }
        printf("Reduction: %d\n", reduction);
        totalReduction += reduction;
    }

    return totalReduction;
}

int onlyInReg(Cell* reg[]){
    int reduction = 0;
    for(int n = 1; n<=9; n++){
        int count = 0;
        Cell* found = reg[0];
        for(int i = 0; i < 9; i++){
            if(reg[i]->val) continue; // Skip solved cells
            if((reg[i]->options >> n-1) & 1){
                count++;
                found = reg[i];
            }
        }

        if(count == 1){
            found->val = n;
            found->options = 0;
            found->solveBased = 'o';
            reduction++;
        }
    }

    return reduction;
}