#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "cell_entropy.h"

TestSuite(EntropyInitializedCell);
Test(EntropyInitializedCell, test_cell_is_initialized) {
    uint16_t cell = get_initialized_cell();
    cr_assert(eq(cell, (entropies[all] | (9 << 9))));
}

//////////////////////////////////////////////////
TestSuite(EntropyCount);
Test(EntropyCount, test_initialized_count) {
    uint16_t cell = get_initialized_cell();
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 9));
}

Test(EntropyCount, test_recalculate_full_count) {
    uint16_t cell = get_initialized_cell();
    size_t initial_count = calculate_entropy_count(cell);
    recalculate_entropy_count(&cell);
    size_t post_count = calculate_entropy_count(cell);
    cr_assert(eq(initial_count, 9));
    cr_assert(eq(initial_count, post_count));
}

Test(EntropyCount, test_no_count) {
    uint16_t cell = 0;
    size_t count = calculate_entropy_count(cell);
    cr_assert(eq(count, 0));
}

Test(EntropyCount, test_get_entropy_count_full) {
    uint16_t cell = get_initialized_cell();
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 9));
}

Test(EntropyCount, test_get_entropy_count_empty) {
    uint16_t cell = 0;
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 0));
}

Test(EntropyCount, test_partial_entropy_count) {
    uint16_t cell = 0;
    cell |= entropies[one] | entropies[four] | entropies[five] |
            entropies[two] | entropies[nine];
    recalculate_entropy_count(&cell);
    size_t count = calculate_entropy_count(cell);
    cr_assert(eq(count, 5), "Expected 5, got %d", count);
    // cr_assert(eq(count, 5));
}

Test(EntropyCount, test_adding_all_values_iterative) {
    uint16_t cell = 0;
    size_t count;
    enum Entropy values[9] = { one, two, three, four, five,
                               six, seven, eight, nine };
    for (size_t i = 0; i < 9; i++) {
        cell |= entropies[values[i]];
        recalculate_entropy_count(&cell);
        count = get_entropy_count(cell);
        cr_assert(eq(count, i + 1));
    }
}

//////////////////////////////////////////////////
TestSuite(EntropyRemove);
Test(EntropyRemove, test_remove_value_1) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, one);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 8));
    cr_assert(not(cell & entropies[one]));
}

Test(EntropyRemove, test_remove_value_5) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, five);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 8));
    cr_assert(not(cell & entropies[five]));
}

Test(EntropyRemove, test_remove_value_9) {
    uint16_t cell = get_initialized_cell();
    remove_entropy_value(&cell, nine);
    size_t count = get_entropy_count(cell);
    cr_assert(eq(count, 8));
    cr_assert(not(cell & entropies[nine]));
}

Test(EntropyRemove, test_remove_all_values_iterative) {
    uint16_t cell = get_initialized_cell();
    size_t count;
    enum Entropy values[9] = { one, two, three, four, five,
                               six, seven, eight, nine };
    for (size_t i = 0; i < 9; i++) {
        remove_entropy_value(&cell, values[i]);
        count = get_entropy_count(cell);
        cr_assert(eq(count, (9 - (i + 1))));
        cr_assert(not(cell & entropies[values[i]]));
    }
}
