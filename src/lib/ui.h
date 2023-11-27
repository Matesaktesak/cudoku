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
    @param writing: 0 if file must exist and is opened only for reading, 0 if not
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
    @return int: 1 if successful, 0 if not
*/
int savePlayfield(Cell* p[], FILE* f);

#endif //CUDOKU_UI_H