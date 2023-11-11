#include "playfield.h"

Playfield playfieldFromCells(Cell* cells){
    Playfield p;
    p.solvedCells = 0;
    for(int i = 0; i < 81; i++){
        Cell* cell = cells+i;
        p.solvedCells += cell->solveBased == 'p';

        p.cells[cell->x + cell->y*9] = cell;
        //printCell(*cell);
        p.rows[cell->y][cell->x] = cell;
        p.cols[cell->x][cell->y] = cell;
        p.blocks[(cell->x/3) + (cell->y/3)*3][(cell->x%3) + (cell->y%3)*3] = cell;
    }

    return p;
}

void drawPlayfield(Playfield p) {
    printf("   0   1   2   3   4   5   6   7   8\n");
    for(int r = 0; r < 10; r++){
        if(r % 3 != 0) printf(" ┃───┼───┼───┃───┼───┼───┃───┼───┼───┃ \n"); else printf(" %s━━━━━━━━━━━%s━━━━━━━━━━━%s━━━━━━━━━━━%s\n", r == 0 ? "┏" : (r == 9 ? "┗" :"┣"), r == 0 ? "┳" : (r == 9 ? "┻" : "╋"),r == 0 ? "┳" : (r == 9 ? "┻" : "╋"), r == 0 ? "┓" : (r==9 ? "┛" : "┫"));
        if(r == 9) break;

        printf("%d", r);
        for(int i = 0; i < 9; i++){
            if(i % 3 != 0) printf("│"); else printf("┃");
            
            if(p.rows[r][i]->solveBased == 'p') printf("\x1B[1m"); // If the cell was preset, make it bold
            printf(" %c ", cellSolved(p.rows[r][i]) ? cellValue(p.rows[r][i]) + '0' : ' ');
            if(p.rows[r][i]->solveBased == 'p') printf("\x1B[0m");
        }

        int rowsum = 0;
        for(int n = 0; n < 9; n++) rowsum += cellValue(p.rows[r][n]);
        printf("┃%d\n", rowsum);
    }
    printf("  ");
    for(int c = 0; c < 9; c++){
        int colsum = 0;
        for(int n = 0; n < 9; n++) colsum += cellValue(p.cols[c][n]);
        printf("%d  ", colsum);
    }
    printf("\n");
}

int checkSolved(Playfield p){
    int solved = 0;
    for(int i = 0; i < 9; i++) {
        solved += checkAllValues(p.rows[i]);
    }
    for(int i = 0; i < 9; i++) {
        solved += checkAllValues(p.cols[i]);
    }
    for(int i = 0; i < 9; i++) {
        solved += checkAllValues(p.blocks[i]);
    }
    //printf("Solved: %d\n", solved);
    return solved == 27;
}