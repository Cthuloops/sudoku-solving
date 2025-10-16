# wfc-sudoku-generator
wfc-sudoku-generator is an attempt at generating sudoku boards using wave
function collapse.

# Overview
Wave Function Collapse is a constraint solving algorithm and Sudoku is a
constraint solving game.  So it seemed like a match made in heaven. This project
aims to use the wfc algorithm to create valid sudoku boards.

# Sudoku
"In classic [Sudoku],  the objective is to fill a 9 × 9 grid with digits so that
each column, each row, and each of the nine 3 × 3 subgrids that compose the
grid (also called "boxes", "blocks", or "regions") contains all of the digits
from 1 to 9."

# Wave Function Collapse
[Wave Function Collapse Wikipedia]
[Wave Function Collapse Github]

# Parts
## Cell
A cell exists in the superposition of all it's available values, entropy, until
it gets collapsed. Once a cell is collapsed, it has a singular, stable value.

## Grid
The grid is the sudoku board that contains all of the cells and some additional
convenience pointers.

# Build
## Dependencies
Make
C
GCC

## Docs
I have doxygen comments throughout the codebase but I haven't set up any build
for them yet. So if you have doxygen installed you should be able to build
them.

## Test
### Dependencies
Criterion

### Test build
Run `make test` in the root directory. The test executables will be build/test_*

## How to build
To build, clone the repo.
cd into the repo directory and run `make`. It will produce a `sudoku` executable
in the root directory.


[Sudoku]: https://en.wikipedia.org/wiki/Sudoku
[Wave Function Collapse Wikipedia]: https://en.wikipedia.org/wiki/Wave_function_collapse
[Wave Function Collapse Github]: https://github.com/mxgmn/WaveFunctionCollapse