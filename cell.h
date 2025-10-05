/**
 * @file cell_entropy.h
 * @brief Sudoku cell entropy management using bit manipulation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#ifndef CELL_H_
#define CELL_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum Entropy {zero, one, two, three, four, five, six, seven, eight, nine, all};
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
#define entropy_count_mask 0b0001111000000000

/**
 * @brief Get the count of entropy for the cell.
 * @returns The count of entropy.
 */
static inline size_t get_entropy_count(uint16_t cell) {
    cell &= entropy_count_mask;
    return cell >> 9;
}

/**
 * @brief Count the number of bits set within the last 9 bits of the cell.
 * @param cell The cell to count bits.
 * @returns The amount of bits counted.
 */
static inline size_t calculate_entropy_count(uint16_t cell) {
    size_t count = 0;
    cell &= entropies[all];  // Wipe the flags except available entropy values.
    while (cell != 0) {
        count += (cell & 1);  // See if the least significant bit is set.
        cell >>= 1;  // Shift the pattern down 1.
    }
    return count;
}

/**
 * @brief Calculates and updates entropy count.
 * @param cell Pointer to the cell.
 * @note If cell is NULL, the function returns without action.
 */
static inline void recalculate_entropy_count(uint16_t *cell) {
    if (cell == NULL) {
        return;
    }
    *cell &= entropies[all];
    size_t count = calculate_entropy_count(*cell);
    *cell |= (count << 9);
}

/**
 * @brief Remove a digit from the cell's possible values and update count.
 * @param cell Pointer to the cell to be updated.
 * @param value The digit value (1-9) to remove from possibilities.
 * @note Passing zero, all, or invalid enum values is undefined behavior.
 * @note If cell is NULL, the function returns without action.
 */
static inline void remove_entropy_value(uint16_t *cell, enum Entropy value) {
    if (cell == NULL) {
        return;  // Maybe error at some point, just ignore for now.
    }

    *cell &= ~entropies[value];
    recalculate_entropy_count(cell);
}

/**
 * @brief Get initialized cell (all possible values + count).
 * @returns A initialized cell.
 */
static inline uint16_t get_initialized_cell(void) {
    uint16_t cell = entropies[all];
    recalculate_entropy_count(&cell);
    return cell;
}


/**
 * @brief Is_valid_entropy checks whether an entropy is present in a cell.
 * @returns true if present, false otherwise.
 */
static inline bool is_valid_entropy(uint16_t cell, enum Entropy entropy) {
    return cell & entropies[entropy];
}


/**
 * @brief Collapse collapses a cell to a set value and sets it's count to 0.
 * @param cell Pointer to the cell to collapse.
 * @param entropy The entropy state to set (zero through nine)
 * @returns The numeric value (1-9) corresponding to the entropy.
 *          INT8_MAX if value is 'all' (invalid input).
 *          -1 if the value is not a valid entropy for the cell.
 */
static inline int8_t collapse(uint16_t *cell, enum Entropy entropy) {
    if (entropy == all) {
        return INT8_MAX;
    }

    if (!is_valid_entropy(*cell, entropy)) {
        return -1;
    }

    *cell = entropies[entropy];
    return (int8_t)entropy;
}

#endif  // CELL_H_
