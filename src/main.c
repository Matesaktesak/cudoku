#include<stdio.h>
#include<stdlib.h>

#include "./lib/cell.h"
#include "./lib/playfield.h"
#include "./lib/solver.h"
#include "./lib/ui.h"

int main(){
    printf("\x1B[1mCudoku\x1B[0m\n");
    
    start: ;// goto start; is used to restart the program after a solve
    Cell* selectedCell = NULL;
    char selected[2] = {4,4};

    FILE* f = getFile();

    Cell* cells = loadCells(f);
    Playfield p = playfieldFromCells(cells);
    printf("Loaded field:\n");
    drawPlayfield(p, NULL);

    char input[5];
    while(1){
        if(!scanf("%s", input)) continue;
        if(strcmp(input, "exit") == 0) return 0;
        if(strcmp(input, "solve") == 0) goto solve;

        if(strcmp(input, "up") == 0) selected[1]--; // Up
        if(strcmp(input, "down") == 0) selected[1]++; // Down
        if(strcmp(input, "right") == 0) selected[0]++; // Right
        if(strcmp(input, "left") == 0) selected[0]--; // Left
        selectedCell = p.cells[selected[0] + selected[1]*9];

        if(strcmp(input, "set") == 0) {
            selectedCell->options = 1 << (getNumber("Value: ")-1);
        }

        printf("\033[2J");
        printf("Selected cell: %d, %d\n", selected[0], selected[1]);
        drawPlayfield(p, selectedCell);
    }

    solve: ;
    Playfield* pptr = &p;

    char solved = bruteSolve(&pptr);

    p = *pptr;

    drawPlayfield(p, selected);
    printf("%s, %d cells solved.\n", solved ? "Playfield is solved" : "Playfield is not solved", p.solvedCells);

    if(p.solvedCells != 81) for(int i = 0; i < 9 * 9; i++) if(!cellSolved(p.cells[i])){
        printCell(p.cells[i]);
    }

    free(cells);

    goto start;

    return 0;
}