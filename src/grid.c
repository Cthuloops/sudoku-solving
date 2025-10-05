/**
 * @file grid.c
 * @brief Sudoku grid manaement impelmentation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stddef.h>

#include "../include/grid.h"
#include "../include/cell.h"

void initialize_grid(struct Grid *grid) {
    size_t i, j;

    // Get the pointers to the start of the rows and columns
    for (i = 0; i < grid_width; i++) {
        grid->rows[i] = grid->cells + (i * grid_width);
        grid->cols[i] = grid->cells + i;
    }

    // This maps to the middle element of each of the nine 3x3 grids that exist
    // within the grid. I'm calling them nondrants (like quadrants).
    size_t count = 0;
    for (i = 1; i < grid_height; i += 3) {
        for (j = 1; j < grid_width; j += 3) {
            grid->nons[count++] = grid->cells + (i * grid_width) + j;
        }
    }

    // Initialize all the cells in the grid.
    for (i = 0; i < grid_size; i++) {
        grid->cells[i] = get_initialized_cell();
    }
}

void grid_reset_cells(struct Grid *grid) {
    // Just overwrite each cell with a newly initialized cell.
    // Could consider doing something cuter and messing with the bits of the
    // existing cells, but unless there's a real need, meh.
    for (size_t i = 0; i < grid_size; i++) {
        grid->cells[i] = get_initialized_cell();
    }
}

void propagate_entropy(struct Grid *grid, size_t y, size_t x,
                       enum Entropy entropy) {
    if (grid == NULL) {
        return;  // I need to add more consistent error handling.
    }
    if (y >= 9 || x >= 9) {
        return;
    }

    // Get a pointer to the row/col.
    uint16_t *row = grid->rows[y];
    uint16_t *col = grid->cols[x];
    if (row == NULL || col == NULL) {
        return;
    }

    // Get a point to the middle element of the nondrant.
    size_t nondrant = ((y / 3) * 3) + (x / 3);
    uint16_t *non = grid->nons[nondrant];
    if (non == NULL || nondrant >= 9) {
        return;
    }

    // Remove the entropy from each cell except the collapsed cell.
    size_t i;
    for (i = 0; i < grid_width; i++) {
        if (i == x) {
            continue;
        }
        remove_entropy_value(row + i, entropy);
    }

    for (i = 0; i < grid_height; i++) {
        if (i == y) {
            continue;
        }
        remove_entropy_value((col + (i * grid_width)), entropy);
    }

    // Gotta get the relative position of the collapsed cell to the center of
    // it's nondrant
    int8_t rel_pos_y = (y % 3) - 1;
    int8_t rel_pos_x = (x % 3) - 1;
    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {
            if (i == rel_pos_y && j == rel_pos_x) {
                continue;
            }
            remove_entropy_value((non + ((i * grid_width) + j)), entropy);
        }
    }
}
