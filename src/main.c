/**
 * @file main.c
 * @brief Entrypoint to sudoku solver.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/grid.h"
#include "../include/cell.h"

int8_t collapse_and_propagate(struct Grid *grid, uint8_t *start_count,
                              bool (*compare)(uint8_t, uint8_t));

bool greater_than(uint8_t a, uint8_t b) {
    return a > b;
}

bool less_than(uint8_t a, uint8_t b) {
    return a < b;
}

void print_grid(struct Grid *grid);

int main(void) {
    srand( (unsigned)time(NULL) );

    struct Grid grid;
    initialize_grid(&grid);

    // size_t cells_filled = 0;
    // size_t local_max = 0;
    // size_t tries = 0;
    // const size_t max_tries = 10000000;
    // while (tries < max_tries) {
    //     while (collapse_and_propagate(&grid) != -1) {
    //         cells_filled++;
    //         if (cells_filled == grid_size) {
    //             printf("We did it!!\n");
    //             print_grid(&grid);
    //             break;
    //         }
    //     }
    //     tries++;
    //     if (cells_filled > local_max) {
    //         local_max = cells_filled;
    //         printf("Cells filled: %zu\nTries: %zu\n", cells_filled, tries);
    //     }
    //     grid_reset_cells(&grid);
    //     cells_filled = 0;
    //     if (tries % 100000 == 0) {
    //         printf("Tries: %zu\n", tries);
    //     }
    // }
    // printf("Tries: %zu\n", tries);

    size_t cells_filled = 0;
    int8_t ret_val;
    uint8_t start_val = 0;
    size_t tries = 0;
    const size_t max_tries = 1000;
    while (tries < max_tries) {
        if (cells_filled < 17) {
            start_val = 0;
            while (cells_filled < 17) {
                ret_val = collapse_and_propagate(&grid, &start_val, greater_than);
                switch (ret_val) {
                    case -2:
                    fprintf(stderr, "Ran out of cells to collapse after %zu\n",
                            cells_filled);
                    exit(1);
                    break;

                    case -1:
                    fprintf(stderr, "Grid or compare was NULL\n");
                    exit(1);
                    break;

                    case 0:
                    fprintf(stdout, "Propogate resulted in a cell reaching 0 entropy\n");
                    break;

                    default:
                    cells_filled++;
                    break;
                }
            }
        } else if (cells_filled < 81) {
            start_val = 9;
            while (cells_filled < 81) {
                ret_val = collapse_and_propagate(&grid, &start_val, less_than);
                switch (ret_val) {
                    case -2:
                    fprintf(stderr, "Ran out of cells to collapse after %zu\n",
                            cells_filled);
                    exit(1);
                    break;

                    case -1:
                    fprintf(stderr, "Grid or compare was NULL\n");
                    exit(1);
                    break;

                    case 0:
                    fprintf(stdout, "Propogate resulted in a cell reaching 0 entropy\nCells filled: %zu\n", cells_filled);
                    break;

                    default:
                    cells_filled++;
                    break;
                }
            }
        }

        if (cells_filled >= 81) {
            printf("Did it fill really?\n");
            exit(0);
            break;
        }
        tries++;
        if (tries % 100 == 0) {
            printf("%zu\n", tries);
        }
        grid_reset_cells(&grid);
        cells_filled = 0;
    }
    return 0;
}

void print_grid(struct Grid *grid) {
    for (int i = 0; i < grid_size; i++) {
        if (i % 9 == 0 && i > 0) {
            printf("\n");
        }
        for (int j = 0; j < enum_entropy_size; j++) {
            if (is_valid_entropy(grid->cells[i], entropies[j])) {
                printf("%3d ", j);
                break;
            }
        }
    }
    printf("\n");
}

int8_t collapse_and_propagate(struct Grid *grid, uint8_t *start_count,
                              bool (*compare)(uint8_t, uint8_t)) {
    if (grid == NULL || compare == NULL) {
        return -1;
    }
    
    struct Pos {
        size_t y;
        size_t x;
    };
    struct Pos valid_cells[grid_size];

    // Collect a list of positions of cells that have the minimum entropy.
    size_t count = 0;
    uint8_t current_entropy_count;
    for (size_t i = 0; i < grid_size; i++) {
        current_entropy_count = get_entropy_count(grid->cells[i]);
        // We don't want to consider cells that are already collapsed.
        if (compare(current_entropy_count, *start_count) &&
            current_entropy_count > 1) {
            *start_count = current_entropy_count;
            count = 0;
            valid_cells[count++] = (struct Pos){ .y = i / 9, .x = i % 9 };
        } else if (current_entropy_count == *start_count) {
            valid_cells[count++] = (struct Pos){ .y = i / 9, .x = i % 9};
        }
    }
    // There are no cells left that are available to collapse.
    if (count == 0) {
        return -2;
    }

    size_t r = rand() % count;
    struct Pos choice = valid_cells[r];
    uint16_t *collapsing_cell = &grid->cells[(choice.y * grid_width) + choice.x];

    enum Entropy available_values[enum_entropy_size];
    uint8_t amount_available = get_entropy_values(collapsing_cell,
                                                  available_values);

    r = rand() % amount_available;
    uint8_t collapsed_value = collapse(collapsing_cell, available_values[r]);
    uint8_t propagate_value = propagate_collapse(grid, choice.y, choice.x,
                                                 entropies[collapsed_value]);
    if (propagate_value == 0) {
        return 0;
    }

    return INT8_MAX;
}

// int8_t collapse_and_propagate(struct Grid *grid) {
//     // TODO(Cthuloops): Do better error handling. Consider logging and graceful
//     // recovery.
//     if (grid == NULL) {
//         fprintf(stderr, "Grid passed to collapse and propagate is NULL\n");
//         exit(1);
//     }

//     struct Pos {
//         size_t y;
//         size_t x;
//     };
//     struct Pos min_entropy_cells[grid_size];

//     // Collect a list of positions of cells that have the minimum entropy.
//     size_t count = 0;
//     uint8_t min_entropy_count = 9;
//     uint8_t current_entropy_count;
//     for (size_t i = 0; i < grid_size; i++) {
//         current_entropy_count = get_entropy_count(grid->cells[i]);
//         // We don't want to consider cells that are already collapsed.
//         if (current_entropy_count < min_entropy_count &&
//             current_entropy_count > 1) {
//             min_entropy_count = current_entropy_count;
//             count = 0;
//             min_entropy_cells[count++] = (struct Pos){ .y = i / 9, .x = i % 9 };
//         } else if (current_entropy_count == min_entropy_count) {
//             min_entropy_cells[count++] = (struct Pos){ .y = i / 9, .x = i % 9};
//         }
//     }

//     // There are no cells left that are available to collapse.
//     if (count == 0) {
//         return -1;
//     }

//     size_t r = rand() % count;
//     struct Pos choice = min_entropy_cells[r];
//     uint16_t *collapsing_cell = &grid->cells[(choice.y * grid_width) + choice.x];

//     enum Entropy available_values[enum_entropy_size];
//     uint8_t amount_available = get_entropy_values(collapsing_cell,
//                                                   available_values);

//     // printf("Cell (%zu, %zu) available values: ", choice.y, choice.x);
//     // for (size_t i = 0; i < amount_available; i++) {
//     //     printf("%d, ", available_values[i]);
//     // }
//     // printf("\n");
//     // TODO(Cthuloops): Another good candidate for real logging.
//     if (amount_available == 0) {
//         fprintf(stderr, "Something went wrong, expected cell %zu, %zu to have "
//                         "entropy available\n", choice.y, choice.x);
//         exit(1);
//     }

//     r = rand() % amount_available;
//     // TODO(Cthuloops): Reconsider this api. Do I need the returned value
//     //                  if I have available_values[r]?
//     // printf("Collapsing cell: %zu, %zu\n", choice.y, choice.x);
//     uint8_t collapsed_value = collapse(collapsing_cell, available_values[r]);
//     // Should I pass the return value or the available_values[r]?
//     propagate_collapse(grid, choice.y, choice.x, entropies[collapsed_value]);

//     return collapsed_value;
// }
