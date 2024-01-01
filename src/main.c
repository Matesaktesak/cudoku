#include<stdio.h>
#include<stdlib.h>

#include "./lib/cell.h"
#include "./lib/playfield.h"
#include "./lib/solver.h"
#include "./lib/ui.h"

int main(int argc, char** argv){
    // Digest CLI arguments
    if(argc > 1) {
        // Solve command
        if(strcmp(argv[1], "solve") == 0){
            char path[100] = "./saves/";
            strcat(path, argv[2]);
            strcat(path, ".cudoku");
            FILE* f = fopen(path, "r");

            Cell* cells = loadCells(f);
            Playfield p = playfieldFromCells(cells);
            Playfield* pptr = &p;
            char solved = bruteSolve(&pptr);
            p = *pptr;
            drawPlayfield(p, NULL);
            printf("%s, %d cells solved.\n", solved ? "Playfield is solved" : "Playfield is not solved", p.solvedCells);
            if(p.solvedCells != 81) for(int i = 0; i < 9 * 9; i++) if(!cellSolved(p.cells[i])){
                printCell(p.cells[i]);
            }
            free(cells);
            return 0;
        }

        // Help command
        if(strcmp(argv[1], "help") == 0){
            if(argc == 3){
                // Help for solve command
                if(strcmp(argv[2], "solve") == 0){
                    printf("Usage: cudoku solve [filename]\n");
                    printf("Solves a playfield\n");
                    return 0;
                }
            }
            printf("Usage: cudoku [command]\n");
            printf("Commands:\n");
            printf("  solve: Solve a playfield\n");
            printf("  help: Show this help\n");
            printf("Run without arguments to start in interactive mode\n");
            // printf("  version: Show version\n");
            return 0;
        }

        // Unknown command
        printf("Unknown command. Use 'cudoku help' for help.\n");
        return 0;
    }

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
        // Update cursor and draw playfield
        selectedCell = p.cells[selected[0] + selected[1] * 9];
        printf("\033[2J");
        printf("Selected cell: %d, %d\n", selected[0], selected[1]);
        drawPlayfield(p, selectedCell);
        
        // Get input
        if(!scanf("%s", input)) continue;
        
        // Move cursor
        if(strcmp(input, "u") == 0) selected[1]+=8;
        if(strcmp(input, "d") == 0) selected[1]++;
        if(strcmp(input, "l") == 0) selected[0]+=8;
        if(strcmp(input, "r") == 0) selected[0]++;
        selected[0] %= 9;
        selected[1] %= 9;
        
        if(strcmp(input, "exit") == 0) return 0;
        if(strcmp(input, "solve") == 0) goto solve;
        if(strcmp(input, "load") == 0) goto start;
        if(strcmp(input, "save") == 0) {
            clearInput();
            FILE* f = getFile(1);
            if(savePlayfield(p.cells, f)) printf("Saved playfield\n"); else printf("Failed to save playfield\n");
        }
        if(strcmp(input, "set") == 0) {
            int n = getNumber("Value: ");
            selectedCell->options = n ? 1 << (n-1) : 0b111111111;
        }
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