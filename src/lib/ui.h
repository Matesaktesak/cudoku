#ifndef CUDOKU_UI_H
#define CUDOKU_UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    @brief Gets a number from stdin
    @param message: message to print before getting number
    @return number as int
*/
int getNumber(char* message);

/*
    @brief Gets a file name from stdin and returns a file pointer to the file
    @return file pointer to file
*/
FILE* getFile();

/*
    @brief Clears stdin
    @return void
*/
void clearInput();

#endif //CUDOKU_UI_H