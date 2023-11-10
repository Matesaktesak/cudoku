#include "cell.h"

typedef struct Playfield{
    Cell* rows[9][9];
    Cell* cols[9][9];
    Cell* blocks[9][9];
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
void drawPlayfield(Playfield p);
