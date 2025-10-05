/**
 * @file cell_entropy.h
 * @brief Sudoku cell entropy management using bit manipulation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stdint.h>

#include "cell.h"

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
