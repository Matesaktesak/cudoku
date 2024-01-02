#include <stdio.h>
#include <stdlib.h>
#include "cell.h"

#ifndef CUDOKU_PLAYFIELD_H
#define CUDOKU_PLAYFIELD_H

typedef struct {
    Cell* rows[9][9];
    Cell* cols[9][9];
    Cell* blocks[9][9];
    Cell* cells[9*9];

    char solvedCells; // Number of solved cells
} Playfield;

/*
    @brief: Creates a playfield from a 1D array of cells
    @param: cells - 1D array of cells pointer
    @return: Playfield of the cells
*/
Playfield playfieldFromCells(Cell* cells);

/*
    @brief: Render the playfield to stdout
    @param: p: Playfield to draw
*/
void drawPlayfield(Playfield p, Cell* selected);

/*
    @param p: Playfield to check
    @return: 1 if playfield is solved, 0 if not
*/
char checkSolved(Playfield* p);

Playfield* clonePlayfield(Playfield* original);

#endif //CUDOKU_PLAYFIELD_H