/**
 * @file main.c
 * @brief Entrypoint to sudoku solver.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stdio.h>

#include "grid.h"
#include "cell.h"

int main(void) {

    struct Grid grid;
    initialize_grid(&grid);

    // Visualize the initialized grid.
    for (size_t i = 0; i < grid_height; i++) {
        for (size_t j = 0; j < grid_width; j++) {
            printf("%zu ", get_entropy_count(grid.cells[(i * grid_width) + j]));
        }
        printf("\n");
    }


    return 0;
}
