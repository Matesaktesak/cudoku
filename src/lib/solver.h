#include "cell.h"
#include <stdio.h>
#include <stdlib.h>
#include "playfield.h"

#ifndef CUDOKU_SOLVER_H
#define CUDOKU_SOLVER_H

/*
    @brief Removes an option from all unsolved cells in a group
    @param o: option bitfield to remove (prefarable only one option set)
    @param cells: array of cells to remove option from
    @return number of cells solved by this operation
*/
int removeOptionFromGroup(Options o, Cell* cells[]);


/*
    @brief Removes invalid options from all cells in a playfield
    @param field: playfield to remove options from
    @return number of cells solved by this operation
*/
int removeOptions(Playfield* field);

/*
    @brief Checks if a cell hass an option no other cell in the region has and sets it as the value of the cell (disables all other options)
    @param reg: array of cells in the region
    @return number of cells solved by this operation
*/
int onlyInReg(Cell* reg[]);

/*
    @brief Solves a playfield using logic
    @param p: playfield to solve
    @return 1 if solved, 0 if not
*/
char logicSolve(Playfield* p);

/*
    @brief Solves a playfield using brute force
    @param p: pointer to a pointer to a playfield to solve
    @return 1 if solved, 0 if not
    @details This function is recursive and consumes a lot of memory, but should eventually solve EVERY solvable playfield
*/
char bruteSolve(Playfield** p);

#endif //CUDOKU_SOLVER_H