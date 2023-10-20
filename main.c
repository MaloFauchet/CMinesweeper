#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define COLUMNS 10
#define ROWS 10
const int MINE_NUMBER = 10;
const char EMPTY_CELL = '-';
const char FLAG = '#';

struct position {
    int x;
    int y;
};

typedef int t_grid[ROWS][COLUMNS];

void grille_init(int *grid, int val);
void print_grid(int *grid);
void plant_mine(int *grid);
void get_mine_around(int *mine_grid, int *game_grid);
void get_input(int *game_grid, struct position *pos);
void discover_case_around(int *grid, int x, int y, int *position_checked, int *position_checked_index, int *mine_grid);
bool is_in_list(int x, int y, int *list, int position_checked_index);
void update_game_grid(int *game_grid, int x, int y, int *mine_grid);

int main() {

    // Declaring structures
    // input of the player
    struct position pos;

    // Declaring variables
    int position_checked[COLUMNS*ROWS];  // stores the positions already checked (0s and others)
    int position_checked_index = 0;  // index of the position_checked last element + 1
    bool end_game = false;

    t_grid mine_grid;
    t_grid game_grid;

    // init the tabs
    grille_init(*mine_grid, 0);
    grille_init(*game_grid, EMPTY_CELL);

    // Planting mines
    plant_mine(*mine_grid);

    
    print_grid(*mine_grid);

    // Getting mines around
    get_mine_around(*mine_grid, *game_grid);

    // Printing grid : debug
    print_grid(*mine_grid);

    // game loop
    while (!end_game) {
        // Printing grid
        print_grid(*game_grid);

        // Getting input
        get_input(*game_grid, &pos);

        // Discovering cases around
        discover_case_around(*game_grid, pos.x, pos.y, position_checked, &position_checked_index, *mine_grid);
    }
    

    // Printing grid : debvoid grille_init(int *grid)ug
    print_grid(*mine_grid);

    print_grid(*game_grid);

    get_input(*game_grid, &pos);

    printf("x: %d, y: %d\n", pos.x, pos.y);

    return EXIT_SUCCESS;
}

void grille_init(int *grid, int val) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            *(grid + i * ROWS + j) = val;
        }        
    }
    
}

void plant_mine(int *grid) {
    printf("Planting mines...\n");
    srand(time(NULL));
    int x = rand() % COLUMNS;
    srand(time(NULL));
    int y = rand() % ROWS;

    int i = 0;
    int mine_planted = 0;

    while (mine_planted < MINE_NUMBER) {
        if (*(grid + x * ROWS + y) != -1) {
            printf("%d ", mine_planted);
            *(grid + x * ROWS + y) = -1;
            ++mine_planted;
        }
        srand(time(NULL));
        x = rand() % COLUMNS;
        srand(time(NULL));
        y = rand() % ROWS;

        ++i;
    }
    printf("\n");
}

void print_grid(int *grid) {
    system("clear");
    printf("   |  0  1  2  3  4  5  6  7  8  9\n");
    printf("---+--------------------------------\n");
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < ROWS; j++) {
            // Print the indexes of the ROWS
            if (j == 0) {
                printf("%d. | ", i);
            }
            // Print the grid
            // if is used to print empty cell instead of 0
            if (*(grid + i * ROWS + j) > 8) {
                printf("%2c ", *(grid + i * ROWS + j));
            } else {
            printf("%2d ", *(grid + i * ROWS + j));
            }
        }
        printf("\n");
    }
    printf("\n");
}

void get_mine_around(int *mine_grid, int *game_grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            // If cell is a mine, skip
            if (*(mine_grid + i * ROWS + j) == -1) {
                continue;
            } 
            
            // check up
            if ((i > 0) && (*(mine_grid + (i - 1) * ROWS + j) == -1)) { 
                *(mine_grid + i * ROWS + j) += 1;
            } // check down
            if ((i < ROWS - 1) && (*(mine_grid + (i + 1) * ROWS + j) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check left 
            if ((j > 0) && (*(mine_grid + i * ROWS + (j - 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check right
            if ((j < COLUMNS - 1) && (*(mine_grid + i * ROWS + (j + 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check up left
            if ((i > 0 && j > 0) && (*(mine_grid + (i - 1) * ROWS + (j - 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check up right
            if ((i > 0 && j < COLUMNS - 1) && (*(mine_grid + (i - 1) * ROWS + (j + 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check down left
            if ((i < ROWS - 1 && j > 0) && (*(mine_grid + (i + 1) * ROWS + (j - 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            } // check down right
            if ((i < ROWS - 1 && j < COLUMNS - 1) && (*(mine_grid + (i + 1) * ROWS + (j + 1)) == -1)) {
                *(mine_grid + i * ROWS + j) += 1;
            }
        }
    }
}

void get_input(int *game_grid, struct position *pos) {
    int x, y;

    printf("Enter line: ");
    scanf("%d", &x);
    printf("Enter column: ");
    scanf("%d", &y);
    while ((x < 0 || x > COLUMNS || y < 0 || y > ROWS) || (*(game_grid + x * ROWS + y) != EMPTY_CELL)) {
        printf("Invalid input, please try again.\n");
        printf("Enter line: ");
        scanf("%d", &x);
        printf("Enter column: ");
        scanf("%d", &y);
    }

    pos->x = x;
    pos->y = y;
}

void discover_case_around(int *grid, int x, int y, int *position_checked, int *position_checked_index, int *mine_grid) {
    /* Discovers the case around the position one if the position one is a zero */
    
    int case_value = *(mine_grid + x * ROWS + y);

    if (case_value == -1) {
        printf("You lost !\n");
        exit(EXIT_SUCCESS);
    }

    if (case_value == 1 || case_value == 2 || case_value == 3 || case_value == 4 || case_value == 5 || case_value == 6 || case_value == 7 || case_value == 8) {
        update_game_grid(grid, x, y, mine_grid);
        return;
    }
    
    printf("%d; %d", x, y);

    if (is_in_list(x, y, position_checked, *position_checked_index) || case_value != 0) {
        return;
    }

    position_checked[*position_checked_index] = x;
    position_checked[*position_checked_index+1] = y;
    *position_checked_index += 2;

    // check up
    if (y != 0) {
        discover_case_around(grid, x, y-1, position_checked, position_checked_index, mine_grid);
        if (x != 0) {
            discover_case_around(grid, x-1, y-1, position_checked, position_checked_index, mine_grid);
        }
        if (x != ROWS-1) {
            discover_case_around(grid, x+1, y-1, position_checked, position_checked_index, mine_grid);
        }
    }
    // check left
    if (x != 0) {
        discover_case_around(grid, x-1, y, position_checked, position_checked_index, mine_grid);
    }
    // check right
    if (x != ROWS-1) {
        discover_case_around(grid, x+1, y, position_checked, position_checked_index, mine_grid);
    }
    // check down
    if (y != COLUMNS-1) {
        discover_case_around(grid, x, y+1, position_checked, position_checked_index, mine_grid);
        if (x != 0) {
            discover_case_around(grid, x-1, y+1, position_checked, position_checked_index, mine_grid);
        }
        if (x != ROWS-1) {
            discover_case_around(grid, x+1, y+1, position_checked, position_checked_index, mine_grid);
        }
    }
    update_game_grid(grid, x, y, mine_grid);
}

bool is_in_list(int x, int y, int *list, int position_checked_index) {
    /* return true if x and y a*/
    for (int i = 0; i < position_checked_index; i+=2) {
        if (list[i] == x && list[i+1] == y) {
            return true;
        }
    }
    return false;
}

void update_game_grid(int *game_grid, int x, int y, int *mine_grid) {
    /* Update the game grid with the mine grid */
    *(game_grid + x * ROWS + y) = *(mine_grid + x * ROWS + y);
    printf("x: %d, y: %d\n", x, y);
    printf("value: %d\n", *(mine_grid + x * ROWS + y));
    printf("game_grid: %d\n", *(game_grid + x * ROWS + y));
}