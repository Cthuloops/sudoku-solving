/**
 * @file grid.h
 * @brief Sudoku grid management header.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#ifndef INCLUDE_GRID_H_
#define INCLUDE_GRID_H_

#include <stdint.h>


#define grid_width 9
#define grid_height 9
#define grid_nondrants 9
#define grid_size 81


/**
 * @struct Grid
 * @brief contains convenience pointers and actual grid information
 */
struct Grid {
    /** Stores pointers to the start of each row. */
    uint16_t *rows[9];

    /** Stores pointers to the start of each column. */
    uint16_t *cols[9];

    /** Stores pointers to the middle element of each nondrant. */
    uint16_t *nons[9];

    /** Underlying array that represents the grid. */
    uint16_t cells[81];
};

/**
* @brief Initialize initializes the grid.
*
* @details Sets up all of the pointers to the rows/cols/nons and initializes
* the cells in the cell array to their default starting value.
*/
void initialize_grid(struct Grid *grid);

/**
 * @brief Grid_reset_cells resets the cells in the grid to their default
 * starting value.
 */
void grid_reset_cells(struct Grid *grid);


#endif  // INCLUDE_GRID_H_
