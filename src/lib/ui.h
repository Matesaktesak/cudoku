#ifndef CUDOKU_UI_H
#define CUDOKU_UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./cell.h"

/*
    @brief Gets a number from stdin
    @param message: message to print before getting number
    @return number as int
*/
int getNumber(char* message);

/*
    @brief Gets a file name from stdin and returns a file pointer to the file
    @param writing: 0 if file must exist and is opened only for reading, 1 to open file for writing
    @return file pointer to file
*/
FILE* getFile(int writing);

/*
    @brief Clears stdin
    @return void
*/
void clearInput();

/*
    @brief Saves a playfield to a file
    @param p: playfield to save
    @param f: file pointer to file to save to
    @param preset: 1 to sa playfield as a preset, 0 to save the playfield as "in progress"
    @return int: 1 if successful, 0 if not
*/
int savePlayfield(Cell* p[], FILE* f, char preset);

#endif //CUDOKU_UI_H