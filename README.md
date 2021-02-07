# Binary search tree 
Repository for the Advanced Programming course's project (A.Y. 2020-2021)

## How to compile and run
In order to produce the final executable file, a *Makefile* is provided
and the following commands can be run in the shell from the root directory
of the project:
- `make all` allows to clean, build and execute the program;
- `make clean` allows to remove (if present) the directory with all the
executable files;
- `make build` allows to build the executable files for the program;
- `make run` allows to run the program (if executable files are already built);
- `make valgrind` allows to run the program with *Valgrind*, in order to detect
  memory errors;
- `make all_with_valgrind` is the same as `make all`, but the program is executed
  with *Valgrind*;
- `make allv` is a shortcut for `make all_with_valgrind`.

Benchmark tests are available: parameters can be configured from the source file
*test/benchmark_test.h*. Benchmark results are available as CSV files in
the directory specified as parameter in the source file *test/benchmark_test.h* . 

## Short report
### What we did
We used the 'Pair Programming' technique: one at a time was coding and the others
were checking. All together discussed about design choices.
We started creating the header files and deciding the structure of the project.
We thought a lot about best choices for the project.
### What we understood
We understood:
- how to create and use *Makefile*;
- how objects are stored in memory (when in the heap, when in the stack).
### Benchmarked
