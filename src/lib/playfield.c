#include <stdio.h>
#include "playfield.h"

Playfield playfieldFromCells(Cell* cells){
    Playfield p;
    for(int i = 0; i < 81; i++){
        Cell* cell = cells+i;
        //printCell(*cell);
        p.rows[cell->y][cell->x] = cell;
        p.cols[cell->x][cell->y] = cell;
        p.blocks[(cell->x/3) + (cell->y/3)*3][(cell->x%3) + (cell->y%3)*3] = cell;
    }

    return p;
}

void drawPlayfield(Playfield p) {
    for(int r = 0; r < 10; r++){
        if(r % 3 != 0) printf("┃───┼───┼───┃───┼───┼───┃───┼───┼───┃ \n"); else printf("%s━━━━━━━━━━━%s━━━━━━━━━━━%s━━━━━━━━━━━%s\n", r == 0 ? "┏" : (r == 9 ? "┗" :"┣"), r == 0 ? "┳" : (r == 9 ? "┻" : "╋"),r == 0 ? "┳" : (r == 9 ? "┻" : "╋"), r == 0 ? "┓" : (r==9 ? "┛" : "┫"));
        if(r == 9) break;

        for(int i = 0; i < 9; i++){
            if(i % 3 != 0) printf("│"); else printf("┃");
            printf(" %c ", p.rows[r][i]->val != 0 ? p.rows[r][i]->val + '0' : ' ');
        }
        printf("┃\n");
    }
}