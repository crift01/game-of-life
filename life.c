#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GRID_COLS 40
#define GRID_ROWS 40
#define GRID_CELLS (GRID_COLS*GRID_ROWS)
#define ALIVE '#'
#define DEAD '.'
#define RESET_CURSOR "\x1b\x5b\x48"

//The function given x, y returns the index position in a linear array.
//This function implements wrapping so both ends of the grid will wrap around
int getPosition(int x, int y) {
    x = x % GRID_COLS;
    if(x < 0) x += GRID_COLS;

    y = y % GRID_ROWS;
    if(y < 0) y += GRID_ROWS;

    return y*GRID_COLS + x;
}

//The function sets the cell in x, y position to a specified state.
void setCell(char *grid, int x, int y, char state) {
    if(state != ALIVE && state != DEAD) { return; }
    grid[getPosition(x,y)] = state;
    return;
}

//The fuction returns the state of a specified cell in x,y position.
char getCell(const char *grid, int x, int y){
    return grid[getPosition(x,y)];
}

//Show the grid on terminal
//Automatically clears the console before printing next (VT100 escape sequence)
void printGrid(const char *grid) {
    fputs(RESET_CURSOR, stdout); //Cleans screen
    for (int rows = 0; rows < GRID_ROWS; rows++) {
        for (int cols = 0; cols < GRID_COLS; cols++) {
            putchar(getCell(grid, cols, rows));
        }
        putchar('\n');
    }    
}

//Sets all the grid cells to the specified state
void setGrid(char *grid, char state) {
    memset(grid, state, GRID_CELLS);       
}

//Returns the number of living cells adjacent to the given position x,y in a grid
int countNeighbours(const char *grid, int x, int y) {
    int count = 0;
    for (int rows = -1; rows <= 1; rows++) {
        for (int cols = -1; cols <= 1; cols++) {
            if(cols == 0 && rows == 0) continue;
            if(getCell(grid, cols+x,rows+y) == ALIVE) count++;
        }        
    }
    return count;
}

//Creates the new grid based on the old one and the rules set by the Game of life
void newState(char *old_grid, char *new_grid) {
    for (int rows = 0; rows < GRID_ROWS; rows++) {
        for (int cols = 0; cols < GRID_COLS; cols++) {
            int alive_count = countNeighbours(old_grid, cols, rows); 
            char state = DEAD;       
            if(alive_count == 3) {
                state = ALIVE;
            }
            else if((getCell(old_grid, cols, rows) == ALIVE) && alive_count == 2) {
                state = ALIVE;
            }
            setCell(new_grid, cols, rows, state);
            
        }
    }  
}

int main() {
    char old_grid[GRID_CELLS];
    char new_grid[GRID_CELLS];
    setGrid(old_grid, DEAD);

    setCell(old_grid, 27, 3, ALIVE);
    setCell(old_grid, 27, 4, ALIVE);
    setCell(old_grid, 27, 5, ALIVE);

    setCell(old_grid, 10, 10, ALIVE);
    setCell(old_grid, 9, 10, ALIVE);
    setCell(old_grid, 11, 10, ALIVE);
    setCell(old_grid, 11, 9, ALIVE);
    setCell(old_grid, 10, 8, ALIVE);

    setCell(old_grid, 5, 30, ALIVE);
    setCell(old_grid, 5, 29, ALIVE);
    setCell(old_grid, 6, 29, ALIVE);
    setCell(old_grid, 6, 28, ALIVE);
    setCell(old_grid, 7, 28, ALIVE);
    setCell(old_grid, 8, 28, ALIVE);
    setCell(old_grid, 8, 27, ALIVE);
    
    setCell(old_grid, 5, 30, ALIVE);
    setCell(old_grid, 5, 29, ALIVE);
    setCell(old_grid, 6, 29, ALIVE);
    setCell(old_grid, 6, 28, ALIVE);
    setCell(old_grid, 7, 28, ALIVE);
    setCell(old_grid, 8, 28, ALIVE);
    setCell(old_grid, 8, 27, ALIVE);


    while (1) {
        newState(old_grid, new_grid);
        printGrid(new_grid);
        usleep(100000);
        newState(new_grid, old_grid);
        printGrid(old_grid);
        usleep(100000);
    }
    return 0;
}
