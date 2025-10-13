/**
 * @file cell_entropy.h
 * @brief Sudoku cell entropy management using bit manipulation.
 * @author Harley Coughlin
 * @copyright Copyright (c) 2025 Harley Coughlin
 * @license MIT
 */

#ifndef INCLUDE_CELL_H_
#define INCLUDE_CELL_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define enum_entropy_size 11
enum Entropy {zero, one, two, three, four, five, six, seven, eight, nine, all};
extern const uint16_t entropy_masks[enum_entropy_size];
extern const enum Entropy entropies[enum_entropy_size];
#define entropy_lookup_amount 512
extern uint8_t entropy_lookup[entropy_lookup_amount];

void initialize_entropy_lookup(void);
static inline size_t get_entropy_count(uint16_t cell);
static inline void remove_entropy_value(uint16_t *cell, enum Entropy value);
static inline uint16_t get_initialized_cell(void);
static inline bool is_valid_entropy(uint16_t cell, enum Entropy entropy);
static inline int8_t collapse(uint16_t *cell, enum Entropy entropy);

/**
 * @brief Get_entropy_values places the available values for a cell in a buffer.
 * @returns The count of elements placed within the buffer.
 * @note If the cell or buf is NULL, does nothing and returns 0.
 */
uint8_t get_entropy_values(uint16_t *cell, enum Entropy buf[]);

/**
 * @brief Get the count of entropy for the cell.
 * @returns The count of entropy.
 */
static inline size_t get_entropy_count(uint16_t cell) {
    // Grab only the 9 lsbs.
    return entropy_lookup[cell & entropy_masks[all]];
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

    if (is_valid_entropy(*cell, value)) {
        *cell &= ~entropy_masks[value];
    }
}

/**
 * @brief Get initialized cell (all possible values).
 * @returns A initialized cell.
 */
static inline uint16_t get_initialized_cell(void) {
    return (uint16_t)entropy_masks[all];
}


/**
 * @brief Is_valid_entropy checks whether an entropy is present in a cell.
 * @returns true if present, false otherwise.
 */
static inline bool is_valid_entropy(uint16_t cell, enum Entropy entropy) {
    // cast to bool since explicit is better than implicit.
    return (bool)(cell & entropy_masks[entropy]);
}


/**
 * @brief Collapse collapses a cell to a set value.
 * @param cell Pointer to the cell to collapse.
 * @param entropy The entropy state to set (one through nine)
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

    *cell = entropy_masks[entropy];
    return (int8_t)entropy;
}

#endif  // INCLUDE_CELL_H_
