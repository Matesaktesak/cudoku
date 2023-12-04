#include <stdio.h>
#include <stdlib.h>

#ifndef CUDOKU_CELL_H
#define CUDOKU_CELL_H

typedef unsigned int Options;

typedef struct Cell{
    char x; // x coordinate
    char y; // y coordinate
    Options options; // Bitfield, rightmost = 1, leftmost = 9, solved cell has only one option
    char solveBased; // 'p' = preset, 'l' = last option, 'o' = only in group, 'b' = brute force (recursive)
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

/*
    @brief: prints the options of a cell as a bitfield multiplied by '123456789' (eg 0b101010101 -> '9 7 5 3 1')
    @param cell: cell to print options of
    @return: void
*/
void printCellOptions(Cell* cell);

/*
    @brief: gets the value of a cell
    @param cell: cell to get value of
    @return: value of cell (1-9)
*/
char cellValue(Cell* c);

/*
    @brief: checks if a cell is solved
    @param cell: cell to check
    @return: 1 if solved, 0 if not
*/
char cellSolved(Cell* cell);

/*
    @brief Loads a cell array from a file
    @param path: path to file to load from
    @return: array of cells or null if failed
*/
Cell* loadCells(FILE* f);

#endif //CUDOKU_CELL_H