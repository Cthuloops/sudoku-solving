/**
 * @file grid.c
 * @brief Sudoku grid manaement impelmentation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stddef.h>

#include "grid.h"
#include "cell.h"

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
