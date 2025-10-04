#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <windows.h>

#define width 9
#define height 9
#define gridsize 81

enum Entropy {zero, one, two, three, four, five, six, seven, eight, nine, mask};
const uint16_t entropies[11] = { 
    0b0000000000000000,
    0b0000000000000001,
    0b0000000000000010,
    0b0000000000000100,
    0b0000000000001000,
    0b0000000000010000,
    0b0000000000100000,
    0b0000000001000000,
    0b0000000010000000,
    0b0000000100000000,
    0b0000000111111111
};

static inline size_t get_entropy_count(uint16_t entropy) {
    return entropy >> 9;
}

static inline size_t calculate_entropy_count(uint16_t entropy) {
    uint16_t count = 0;
    entropy &= entropies[mask];
    while(entropy) {
        count += (entropy & 1);
        entropy >>= 1;
    }
    return count;
}

static inline void update_entropy_count(uint16_t *entropy) {
    *entropy &= entropies[mask];  // remove previous count.
    size_t count = calculate_entropy_count(*entropy);  // get current count.
    *entropy |= (count << 10);  // set the new count.
}


static inline void update_entropy(uint16_t *old, uint16_t new) {
    *old &= new;
    update_entropy_count(old);
}


typedef struct Cell Cell;
struct Cell {
    uint16_t entropy;
    uint8_t value;
};

typedef struct Row Row;
struct Row {
    Cell *ptr;
    uint16_t potential;
};
typedef struct Row Col;
typedef struct Row Non;

typedef struct {
    Row rows[height];
    Col cols[width];
    Non nons[9];
    Cell cells[gridsize];
} Grid;


int8_t collapse(Grid *grid);
void initialize_grid(Grid *grid);
void decrement_entropy(Grid *grid, size_t row, size_t col, size_t non);

int main(void) {
    srand((unsigned)time(NULL));

    Grid grid;

    initialize_grid(&grid);

    size_t tries = 0;
    size_t max_tries = 1000000000;
    size_t cells_filled = 0;
    size_t local_max = 0;
    while (tries < max_tries) {
        while ((collapse(&grid)) != -1) {
            cells_filled++;
            if (cells_filled == gridsize) {
                printf("Success!!\n");
                break;
            }
        }
        tries++;
        if (cells_filled > local_max) {
            printf("Cells filled = %zu\n", cells_filled);
            local_max = cells_filled;
            printf("Tries: %zu\n", tries);
        }
        if (tries % 1000000 == 0) {
            printf("Cells filled = %zu\n", cells_filled);
            printf("Tries: %zu\n", tries);
        }
        initialize_grid(&grid);
        cells_filled = 0;
    }
    printf("Tries: %zu\n", tries);

    return 0;
}

int8_t collapse(Grid *grid) {
    struct Pos {
        size_t y;
        size_t x;
    };
    struct Pos choices[81];
    size_t count = 0;
    uint16_t min_entropy = entropies[mask];
    uint16_t current_entropy;
    size_t i;
    for (i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            current_entropy = grid->cells[(i * width) + j].entropy;
            if (current_entropy == zero) {
                continue;
            }

            if (current_entropy < min_entropy) {
                min_entropy = current_entropy;
                count = 0;
                choices[count++] = (struct Pos) { .y = i, .x = j };
            } else if (current_entropy == min_entropy) {
                choices[count++] = (struct Pos) { .y = i, .x = j };
            }
        }
    }

    if (count == 0) {
        return -1;
    }

    size_t r = rand() % count;
    struct Pos choice = choices[r];
    Cell *collapsing = &grid->cells[(choice.y * width) + choice.x];

    // I need to get the values that the cell could be and choose a random one
    // set the value of the cell to that value,
    // then update the affected cells to reflect the lowered entropy and
    // remove that value from their sets.


    // intersect the row/col/non to determine possible values
    size_t nondrant = ((choice.y / 3) * 3) + (choice.x / 3);
    uint16_t possibilities = grid->rows[choice.y].potential &
                             grid->cols[choice.x].potential &
                             grid->nons[nondrant].potential;

    count = get_entropy_count(possibilities);
    if (count < 1 || count > 9) {  // we hate both of these
        fprintf(stderr, "Entropy count is bad\n");
        Sleep(1000);
        return -1;
    }
    uint8_t potentials[count];
    // This loops through the potential values in possibilities and then maps
    // the natural number to potentials in reverse order.
    for (uint8_t i = 1; i <= 9; i++) {
        if ((possibilities & entropies[i]) == entropies[i]) {
            potentials[count - i] = i;
        }
    }

    // Now we can choose the value for the collapsing cell
    r = rand() % count;
    collapsing->value = potentials[r];

    // Now we need to update the entropy to reflect the value of the collapsed
    // cell. Maybe break this into a function for looks.
    uint16_t entropy_to_remove;
    switch (r) {
        case 1:
            entropy_to_remove = one;
            break;
        case 2:
            entropy_to_remove = two;
            break;
        case 3:
            entropy_to_remove = three;
            break;
        case 4:
            entropy_to_remove = four;
            break;
        case 5:
            entropy_to_remove = five;
            break;
        case 6:
            entropy_to_remove = six;
            break;
        case 7:
            entropy_to_remove = seven;
            break;
        case 8:
            entropy_to_remove = eight;
            break;
        case 9:
            entropy_to_remove = nine;
            break;
    }

    uint16_t new_entropy = possibilities & ~entropy_to_remove;

    // Update the entropies
    update_entropy(&grid->rows[choice.y].potential, new_entropy);
    update_entropy(&grid->cols[choice.x].potential, new_entropy);
    update_entropy(&grid->nons[nondrant].potential, new_entropy);

    return collapsing->value;
}


void initialize_grid(Grid *grid) {
    size_t i, j;

    for (i = 0; i < width; i++) {
        grid->rows[i] = (Row){ .ptr = grid->cells + (i * width),
            .potential = entropies[mask] };
        grid->cols[i] = (Col){ .ptr = grid->cells + i,
            .potential = entropies[mask] };

        update_entropy_count(&(grid->rows[i].potential));
        update_entropy_count(&(grid->cols[i].potential));
    }

    size_t count = 0;
    for (i = 1; i < height; i += 3) {
        for (j = 1; j < width; j += 3) {
            grid->nons[count] = (Non){ .ptr = grid->cells + (i * width) + j,
                .potential = entropies[mask] };
            count++;
        }
        update_entropy_count(&(grid->nons[count].potential));
    }

    uint16_t cell_entropy = entropies[mask];
    update_entropy_count(&cell_entropy);
    for (i = 0; i < gridsize; i++) {
        grid->cells[i] = (Cell){ .entropy = cell_entropy, .value = 0 };
    }
}

void decrement_entropy(Grid *grid, size_t y, size_t x, size_t n) {
    Cell *row_start = grid->rows[y].ptr;
    Cell *col_start = grid->cols[x].ptr;
    Cell *non_start = grid->nons[n].ptr;

    for (size_t i = 0; i < width; i++) {
        if (row_start[i].entropy > 0) {
            row_start[i].entropy -= 1;
        }
        if (col_start[i * width].entropy > 0) {
            col_start[i * width].entropy -= 1;
        }
    }

    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {
            if (non_start[(i * width) + j].entropy > 0) {
                non_start[(i * width) + j].entropy -= 1;
            }
        }
    }
}
