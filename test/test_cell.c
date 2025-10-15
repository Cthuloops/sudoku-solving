#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../include/cell.h"

///////////////////////////////////////////////////
TestSuite(EntropyInitializedCell);
Test(EntropyInitializedCell, test_cell_is_initialized) {
    uint16_t cell = get_initialized_cell();
    cr_assert(eq(u16, cell, entropy_masks[all]));
}

//////////////////////////////////////////////////
TestSuite(EntropyCount);
Test(EntropyCount, test_initialized_count) {
    uint16_t cell = get_initialized_cell();
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 9));
}

Test(EntropyCount, test_get_entropy_count_full) {
    uint16_t cell = get_initialized_cell();
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 9));
}

Test(EntropyCount, test_get_entropy_count_empty) {
    uint16_t cell = 0;
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 0));
}

Test(EntropyCount, test_partial_entropy_count) {
    uint16_t cell = 0;
    cell |= entropy_masks[one] | entropy_masks[four] | entropy_masks[five] |
            entropy_masks[two] | entropy_masks[nine];
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 5), "Expected 5, got %d", count);
    // cr_assert(eq(count, 5));
}

Test(EntropyCount, test_adding_all_values_iterative) {
    uint16_t cell = 0;
    size_t count;
    enum Entropy values[9] = { one, two, three, four, five,
                               six, seven, eight, nine };
    for (size_t i = 0; i < 9; i++) {
        cell |= entropy_masks[values[i]];
        count = get_entropy_count(cell);
        cr_assert(eq(ulong, count, i + 1));
    }
}

//////////////////////////////////////////////////
TestSuite(EntropyRemove);
Test(EntropyRemove, test_remove_value_1) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, one);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 8));
    cr_assert(not(cell & entropies[one]));
}

Test(EntropyRemove, test_remove_value_5) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, five);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 8));
    cr_assert(not(cell & entropy_masks[five]));
}

Test(EntropyRemove, test_remove_value_9) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, nine);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(ulong, count, 8));
    cr_assert(not(cell & entropy_masks[nine]));
}

Test(EntropyRemove, test_remove_all_values_iterative) {
    uint16_t cell = get_initialized_cell();
    size_t count;
    enum Entropy values[9] = { one, two, three, four, five,
                               six, seven, eight, nine };
    for (size_t i = 0; i < 9; i++) {
        remove_entropy_value(&cell, values[i]);
        count = get_entropy_count(cell);
        cr_assert(eq(ulong, count, (9 - (i + 1))));
        cr_assert(not(cell & entropies[values[i]]));
    }
}

/////////////////////////////////////////////////////
TestSuite(Collapse);
Test(Collapse, test_collapse_cell_to_one) {
    uint16_t cell = get_initialized_cell();
    int8_t val = collapse(&cell, one);
    // Test the cell acquired the collapsed bit flag.
    cr_assert(is_collapsed(cell));
    // Test the cell is value 1 with the collapsed bit flag masked out.
    cr_assert(eq(u16, (cell & entropy_masks[all]), entropy_masks[one]));
    // Test return value is 1
    cr_assert(eq(i8, val, 1));
}

Test(Collapse, test_collapse_cell_to_three) {
    uint16_t cell = get_initialized_cell();
    int8_t val = collapse(&cell, three);
    // Test the cell acquired the collapsed bit flag.
    cr_assert(is_collapsed(cell));
    // Test the cell is value 3 with the collapsed bit flag masked out.
    cr_assert(eq(u16, (cell & entropy_masks[all]), entropy_masks[three]));
    // Test return value is 3
    cr_assert(eq(i8, val, 3));
}

Test(Collapse, test_collapse_cell_to_nine) {
    uint16_t cell = get_initialized_cell();
    int8_t val = collapse(&cell, nine);
    // Test the cell acquired the collapsed bit flag.
    cr_assert(is_collapsed(cell));
    // Test the cell is value 9 with the collapsed bit flag masked out.
    cr_assert(eq(u16, (cell & entropy_masks[all]), entropy_masks[nine]));
    // Test the return value is 9.
    cr_assert(eq(i8, val, 9));
}

Test(Collapse, test_collapse_cell_to_all) {
    uint16_t cell = get_initialized_cell();
    int8_t val = collapse(&cell, all);
    // Test the cell is not collapsed
    cr_assert(not(is_collapsed(cell)));
    // Test the cell is 'all'.
    cr_assert(eq(u16, cell, entropy_masks[all]));
    // Test the return value is INT8_MAX.
    cr_assert(eq(i8, val, INT8_MAX));
}

Test(Collapse, test_collapse_cell_to_invalid) {
    uint16_t cell = entropy_masks[two];
    int8_t val = collapse(&cell, one);
    cr_assert(not(is_collapsed(cell)));
    cr_assert(eq(u16, cell, entropy_masks[two]));
    cr_assert(eq(i8, val, -1));
}

Test(Collapse, test_collapse_null_cell) {
    uint16_t *cell = NULL;
    int8_t val = collapse(cell, one);
    cr_assert(eq(i8, val, 0));
    cr_assert(eq(ptr, cell, NULL));
}
