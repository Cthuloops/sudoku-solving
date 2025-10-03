#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define width 9
#define height 9
#define gridsize 81


typedef struct Cell Cell;
struct Cell {
    uint8_t entropy;
    uint8_t value;
};

typedef struct Row Row;
struct Row {
    Cell *ptr;
    uint8_t potential[9];
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
void print_values(const Grid *grid);
void print_entropies(const Grid *grid);
void print_potentials(const Grid *grid);

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
            if (cells_filled == 86) {
                printf("Success!!\n");
                print_values(&grid);
                break;
            }
        }
        tries++;
        if (cells_filled > local_max) {
            printf("Cells filled = %zu\n", cells_filled);
            local_max = cells_filled;
            printf("Tries: %zu\n", tries);
        }
        initialize_grid(&grid);
        cells_filled = 0;
        if (tries % 1000000 == 0) {
            printf("Tries: %zu\n", tries);
        }
    }
    printf("Tries: %zu\n", tries);

    // print_values(&grid);
    // printf("\n");
    // print_entropies(&grid);
    // printf("\n");
    // print_potentials(&grid);

    return 0;
}

int8_t collapse(Grid *grid) {
    struct Pos {
        size_t y;
        size_t x;
    };
    struct Pos choices[81];
    size_t count = 0;
    // uint8_t max_entropy = 0;
    uint8_t min_entropy = 9;
    uint8_t current_entropy;
    size_t i;
    for (i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            current_entropy = grid->cells[(i * width) + j].entropy;
            if (current_entropy == 0) {
                continue;
            }

            if (current_entropy < min_entropy) {
                min_entropy = current_entropy;
                count = 0;
                choices[count++] = (struct Pos) { .y = i, .x = j };
            } else if (current_entropy == min_entropy) {
                choices[count++] = (struct Pos) { .y = i, .x = j };
            }
            // if (current_entropy > max_entropy) {
            //     max_entropy = current_entropy;
            //     count = 0;
            //     choices[count++] = (struct Pos) { .y = i, .x = j };
            // } else if (current_entropy == max_entropy) {
            //     choices[count++] = (struct Pos) { .y = i, .x = j };
            // }
        }
    }

    if (count == 0) {
        return -1;
    }

    size_t r = rand() % count;
    struct Pos choice = choices[r];
    Cell *collapsing = &grid->cells[(choice.y * width) + choice.x];
    collapsing->entropy = 0;

    // Get the intersection of the potential values from the row/column/nondrant
    // that corresponds to the position in the grid that the chosen cell is
    // located at. If there are no nonzero values, as indicated by count, then
    // we've hit a dead end.
    // Packt the values in the front of the array and keep the count to reduce
    // misses when choosing a value for the cell to collapse to.
    uint8_t remaining_potential[9];
    size_t nondrant = ((choice.y / 3) * 3) + (choice.x / 3);
    count = 0;
    for (i = 0; i < 9; i++) {
        r = grid->rows[choice.y].potential[i] &
            grid->cols[choice.x].potential[i] &
            grid->nons[nondrant].potential[i];
        if (r == 0) {
            continue;
        } else {
            remaining_potential[count++] = r;
        }
    }
    if (count == 0) {
        return -1;
    }

    r = rand() % count;
    collapsing->value = remaining_potential[r];

    // Update the potential arrays to reflect the collapsed value
    grid->rows[choice.y].potential[collapsing->value - 1] = 0;
    grid->cols[choice.x].potential[collapsing->value - 1] = 0;
    grid->nons[nondrant].potential[collapsing->value - 1] = 0;

    decrement_entropy(grid, choice.y, choice.x, nondrant);

    return collapsing->value;
}


void initialize_grid(Grid *grid) {
    size_t i, j;

    for (i = 0; i < width; i++) {
        grid->rows[i] = (Row){ .ptr = grid->cells + i,
            .potential = {1, 2, 3, 4, 5, 6, 7, 8, 9} };
        grid->cols[i] = (Col){ .ptr = grid->cells + i,
            .potential = {1, 2, 3, 4, 5, 6, 7, 8, 9} };
    }

    size_t count = 0;
    for (i = 1; i < height; i += 3) {
        for (j = 1; j < width; j += 3) {
            grid->nons[count] = (Non){ .ptr = grid->cells + (i * width) + j,
                .potential = {1, 2, 3, 4, 5, 6, 7, 8, 9} };
            count++;
        }
    }

    for (i = 0; i < gridsize; i++) {
        grid->cells[i] = (Cell){ .entropy = 9, .value = 0 };
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

void print_values(const Grid *grid) {
    printf("Values:\n");
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            printf("%d ", grid->cells[(i * width) + j].value);
        }
        printf("\n");
    }
}

void print_entropies(const Grid *grid) {
    printf("Entropies:\n");
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            printf("%d ", grid->cells[(i * width) + j].entropy);
        }
        printf("\n");
    }
}

void print_potentials(const Grid *grid) {
    printf("Rows:\n");
    for (size_t i = 0; i < width; i++) {
        printf("Row %zu: ", i + 1);
        for (size_t j = 0; j < width; j++) {
            printf("%d ", grid->rows[i].potential[j]);
        }
        printf("\n");
    }

    printf("Cols:\n");
    for (size_t i = 0; i < width; i++) {
        printf("Col %zu: ", i + 1);
        for (size_t j = 0; j < width; j++) {
            printf("%d ", grid->cols[i].potential[j]);
        }
        printf("\n");
    }

    printf("Nons:\n");
    for (size_t i = 0; i < width; i++) {
        printf("Non %zu: ", i + 1);
        for (size_t j = 0; j < width; j++) {
            printf("%d ", grid->nons[i].potential[j]);
        }
        printf("\n");
    }
}
