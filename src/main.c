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
            if(strcmp(argv[3], "-s") != 0){
                drawPlayfield(p, NULL);
                printf("%s, %d cells solved.\n", solved ? "Playfield is solved" : "Playfield is not solved", p.solvedCells);
                if(p.solvedCells != 81) for(int i = 0; i < 9 * 9; i++) if(!cellSolved(p.cells[i])){
                    printCell(p.cells[i]);
                }
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

                if(strcmp(argv[2], "interactive") == 0){
                    printf("Commands usable in interactive mode:\n  - U/w: Move cursor up\n  - D/s: Move cursor down\n  - L/a: Move cursor left\n  - R/d: Move cursor right\n  - set: Set value of selected cell\n  - solve: Solve playfield\n  - load: Load playfield\n  - save: Save playfield\n  - save!: Save playfield as preset\n  - choises: displays the valid choises for the selected cell\n  - check: checks the field and prints a list of cell that are wrong\n  - exit: Exit program\n");
                    return 0;
                }
            }
            printf("Usage: cudoku [command]\n");
            printf("Commands:\n");
            printf("  solve: Solve a playfield\n");
            printf("  help [object]: Show this help\n  - solve: Help for solve command\n  - interactive: Help for interactive mode\n");
            printf("Run without arguments to start in interactive mode\n");
            // printf("  version: Show version\n");
            return 0;
        }

        // Unknown command
        printf("Unknown command. Use 'cudoku help' for help.\n");
        return 0;
    }


    // Interactive mode
    printf("\x1B[1mCudoku\x1B[0m\n");
    
    int firstStart = 1; // Set to 1 to load a playfield on first start

    start: ; // goto start; is used to restart the program after a solve

    Cell* selectedCell = NULL;
    char selected[2] = {4,4};

    if(!firstStart) clearInput();
    firstStart = 0;
    FILE* f = getFile(0);

    Cell* cells = loadCells(f);
    Playfield p = playfieldFromCells(cells);
    Playfield* guide = clonePlayfield(&p);
    // TODO: Remove non-preset cells from the check playfield - rigth now all loaded cells are assumed correctly solved
    Playfield* check = clonePlayfield(&p);
    removeOptions(guide, 0);
    char cacheValid = 1;
    char checkValid = 0;

    printf("Loaded field:\n");

    char input[100];
    while(1){
        // Update cursor and draw playfield
        selectedCell = p.cells[selected[0] + selected[1] * 9];
        //printf("\033[2J");
        // printf("\n\n\n\n\n");
        printf("Selected cell: %d, %d\n", selected[0], selected[1]);
        drawPlayfield(p, selectedCell);
        
        // Get input
        if(!scanf("%s", input)) continue;
        
        // Move cursor
        if(!strcmp(input, "U") || !strcmp(input, "w")) selected[1]+=8;
        if(!strcmp(input, "D") || !strcmp(input, "s")) selected[1]++;
        if(!strcmp(input, "L") || !strcmp(input, "a")) selected[0]+=8;
        if(!strcmp(input, "R") || !strcmp(input, "d")) selected[0]++;
        selected[0] %= 9;
        selected[1] %= 9;
        
        if(strcmp(input, "exit") == 0) return 0;
        if(strcmp(input, "solve") == 0) goto solve;
        if(strcmp(input, "load") == 0) goto start;
        if(strcmp(input, "save") == 0) {
            clearInput();
            FILE* f = getFile(1);
            if(savePlayfield(p.cells, f, 0)) printf("Saved playfield\n"); else printf("Failed to save playfield\n");
            continue;
        }
        if(strcmp(input, "save!") == 0) {
            clearInput();
            FILE* f = getFile(1);
            if(savePlayfield(p.cells, f, 1)) printf("Saved playfield\n"); else printf("Failed to save playfield\n");
            p = playfieldFromCells(loadCells(f));
            cacheValid = 0; // Invalidate cache
            continue;
        }
        if(strcmp(input, "set") == 0) {
            int n = getNumber("Value: ");
            selectedCell->options = n ? 1 << (n-1) : 0b111111111;
            selectedCell->solveBased = 'u';
            cacheValid = 0; // Invalidate cache
            continue;
        }
        if(!strcmp(input, "choises")){
            if(!cacheValid) {
                free(guide);
                guide = clonePlayfield(&p);
                removeOptions(guide, 0);
            }
            printf("Choises for cell [%d, %d]: ", selected[0], selected[1]);
            printCellOptions(guide->cols[selected[0]][selected[1]]);
            printf("\n");
        }
        if(!strcmp(input, "check")){
            if(!checkValid) bruteSolve(&check);
            for(int i = 0; i < 9*9; i++){
                if(cellSolved(p.cells[i]) && check->cells[i]->options != p.cells[i]->options){
                    printf("Cell [%d, %d] is wrong\n", i%9, i/9);
                }
            }
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
    free(guide);
    free(check);

    goto start;

    return 0;
}