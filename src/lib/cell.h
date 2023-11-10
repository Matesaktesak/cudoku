#ifndef CUDOKU_CELL_H
#define CUDOKU_CELL_H

typedef struct Cell{
    int x;
    int y;
    int val; // Zero for empty
    int options;
    char solveBased;
    int preset;
} Cell;

/*
    @param x: x coordinate of cell
    @param y: y coordinate of cell
    @param value: value for the new cell
    @return: new cell struct
*/
Cell newCell(int x, int y, int value);

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
void printCell(Cell cell);

/*
    @brief: counts the number of options a cell has
    @param cell: cell to count options of
    @return: number of options
*/
int cellOpCount(Cell cell);

/*
    @brief: removes an option from a cell
    @param o: option to remove
    @param cells: array of cells to remove option from
    @return: number of cells solved by this operation
*/
int removeOption(int o, Cell* cells);

#endif //CUDOKU_CELL_H