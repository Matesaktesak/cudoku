#include "cell.h"
#include <stdio.h>
#include "playfield.h"

#ifndef CUDOKU_SOLVER_H
#define CUDOKU_SOLVER_H

/*
    @brief: removes an option from a cell
    @param o: option to remove
    @param cells: array of cells to remove option from
    @return: number of cells solved by this operation
*/
int removeOptionFromGroup(Options o, Cell* cells[]);


/*
    @brief Removes invalid options from all cells in a playfield
    @param field: playfield to remove options from
    @return: number of cells solved by this operation
*/
int removeOptions(Playfield* field);

#endif //CUDOKU_SOLVER_H