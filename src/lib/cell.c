#include "cell.h"

Cell newCell(int x, int y, unsigned char value, char solveBased){
    Options options = 0b111111111;
    if(value != 0) options = 1 << (value-1);
    return (Cell){.x = x, .y = y, .solveBased = solveBased, .options = options};
}

Cell emptyCell(int x, int y) {
    return newCell(x, y, 0, 'n');
}

void printCell(Cell* cell){
    printf("Cell [%d, %d]: v = %d, options: ", cell->x, cell->y, cellValue(cell));
    printCellOptions(cell);
    // printf("\n");
    char* solve;
    switch(cell->solveBased){
        case 'p': solve = "P (preset value)"; break; // Last option
        case 'l': solve = "L (single option)"; break; // Last option
        case 'o': solve = "O (only in group)"; break; // Only cell with option in group
        case 'u': solve = "U (user set)"; break; // User set
        case 'b': solve = "B (brute force)"; break; // Brute force
        default: solve = "";
    }
    printf(" %s\n", solve);
}

char cellOpCount(Cell* cell){
    char n = 0; 
    for(int i = 0; i < 9; i++){
        n += (cell->options >> i) & 1;
    }
    return n;
}

char cellValue(Cell* c){
    if(!cellSolved(c)) return 0; 

    for(char i = 0; i < 9; i++) if((c->options >> i) & 1) { 
        return i+1;
    }
}

char checkAllValues(Cell* cells[]){
    Options vals = 0;
    for(int i = 0; i < 9; i++){
        if(!cellSolved(cells[i])) return 0; // If cell has multiple options, it is not solved
        vals |= cells[i]->options; // Binary add the option to the value bitfield
    }

    //printf("%s\n", vals == 0b111111111 ? "Solved" : "Not solved");

    return vals == 0b111111111;
}

void printCellOptions(Cell* cell){
    for(int i = 0; i < 9; i++) if((cell->options >> i) & 1) printf("%d",  i+1); else printf(" ");
}

char cellSolved(Cell* cell){
    return !(cell->options&(cell->options-1)); // If a cells options are not a power of 2, it has multiple options and therefore isn't solved
}

Cell* loadCells(FILE* f){
    Cell* cells = malloc(sizeof(Cell) * 81);
    if(cells == NULL){
        printf("Failed to allocate memory for cells\n");
        return NULL;
    }

    for(int i = 0; i < 9*9; i++){
        *(cells+i) = emptyCell(i%9, (int)i/9);
    }

    int data[4];
    int count = 0;
    do{
        char input = fgetc(f);
        if(input == ',') count++;
        else if(input == '\n' || input == EOF){
            cells[data[0] + data[1]*9] = newCell(data[0], data[1], data[2], data[3]);
            // printCell(cells[data[0] + data[1]*9]);
            count = 0;
            if(input == EOF) break;
            continue;
        } else if(input >= '0' && input <= '9'){
            data[count] = input - '0';
        } else if(count == 3){
            data[3] = input;
        }
    } while(1);

    fclose(f);

    return cells;
}