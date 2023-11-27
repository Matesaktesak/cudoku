#include<stdio.h>
#include<stdlib.h>

#include "./lib/cell.h"
#include "./lib/playfield.h"
#include "./lib/solver.h"
#include "./lib/ui.h"

int main(){
    printf("\x1B[1mCudoku\x1B[0m\n");
    
    int firstStart = 1;

    start: ;// goto start; is used to restart the program after a solve
    Cell* selectedCell = NULL;
    char selected[2] = {4,4};

    if(!firstStart) clearInput();
    firstStart = 0;
    FILE* f = getFile(0);

    Cell* cells = loadCells(f);
    Playfield p = playfieldFromCells(cells);
    printf("Loaded field:\n");
    drawPlayfield(p, NULL);

    char input[100];
    while(1){
        if(!scanf("%s", input)) continue;
        if(strcmp(input, "exit") == 0) return 0;
        if(strcmp(input, "solve") == 0) goto solve;
        if(strcmp(input, "load") == 0) goto start;
        if(strcmp(input, "save") == 0) {
            clearInput();
            FILE* f = getFile(1);
            if(savePlayfield(p.cells, f)) printf("Saved playfield\n"); else printf("Failed to save playfield\n");
        }
        if(strcmp(input, "set") == 0) {
            selectedCell->options = 1 << (getNumber("Value: ")-1);
        }

        if(strcmp(input, "u") == 0) selected[1]--;
        if(strcmp(input, "d") == 0) selected[1]++;
        if(strcmp(input, "l") == 0) selected[0]--;
        if(strcmp(input, "r") == 0) selected[0]++;
        selectedCell = p.cells[selected[0] + selected[1] * 9];

        printf("\033[2J");
        printf("Selected cell: %d, %d\n", selected[0], selected[1]);
        drawPlayfield(p, selectedCell);
    }

    solve: ;
    Playfield* pptr = &p;

    char solved = bruteSolve(&pptr);

    p = *pptr;

    drawPlayfield(p, selectedCell);
    printf("%s, %d cells solved.\n", solved ? "Playfield is solved" : "Playfield is not solved", p.solvedCells);

    if(p.solvedCells != 81) for(int i = 0; i < 9 * 9; i++) if(!cellSolved(p.cells[i])){
        printCell(p.cells[i]);
    }

    free(cells);

    goto start;

    return 0;
}