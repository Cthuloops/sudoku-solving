#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "../include/grid.h"

TestSuite(InitializeGrid);
Test(InitializeGrid, test_grid_struct) {
    struct Grid grid;
    cr_expect([9]());
}
