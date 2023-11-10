#include <stdio.h>

#ifndef CUDOKU_CELL_H
#define CUDOKU_CELL_H

typedef unsigned int Options;

typedef struct Cell{
    char x;
    char y;
    Options options; // Bitfield, rightmost = 1, leftmost = 9, solved cell has only one option
    char solveBased;
    int preset;
} Cell;

/*
    @brief Checks if an array of cells has all values
*/
char checkAllValues(Cell* cells[]);


/*
    @param x: x coordinate of cell
    @param y: y coordinate of cell
    @param value: value for the new cell
    @return: new cell struct
*/
Cell newCell(int x, int y, unsigned char value);

/*
    @param x: x coordinate of cell
    @param y: y coordinate of cell
    @return: new cell stuct with no value (0)
*/
Cell emptyCell(int x, int y);

/*
    @brief: prints a readable format cell desctription to stdout
    @param cell: cell to print
    @return: void
*/
void printCell(Cell* cell);

/*
    @brief: counts the number of options a cell has
    @param cell: cell to count options of
    @return: number of options
*/
char cellOpCount(Cell* cell);

void printCellOptions(Cell* cell);

char cellValue(Cell* c);

char cellSolved(Cell* cell);

#endif //CUDOKU_CELL_H