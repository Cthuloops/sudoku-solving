/**
 * @file cell_entropy.h
 * @brief Sudoku cell entropy management using bit manipulation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#include <stdint.h>

#include "../include/cell.h"

const enum Entropy entropies[enum_entropy_size] = {
    zero, one, two, three, four, five, six, seven, eight, nine, all
};

const uint16_t entropy_masks[enum_entropy_size] = {
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

uint8_t get_entropy_values(uint16_t *cell, enum Entropy buf[]) {
    if (cell == NULL || buf == NULL) {
        return 0;
    }

    size_t count = 0;
    for (size_t i = 1; i < enum_entropy_size - 1; i++) {
        if (is_valid_entropy(*cell, entropies[i])) {
            buf[count++] = entropies[i];
        }
    }

    return count;
}
