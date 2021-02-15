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
- `make run` allows to run the program (if executable files are already built)
- `make valgrind` allows to run the program with *Valgrind*, in order to detect
  memory errors;
- `make all_with_valgrind` is the same as `make all`, but the program is executed
  with *Valgrind*.



## Documentation
Documentation was created with *Doxygen* and can be re-generated with the
command `make documentation`.
The output directory for the documentation can be specified from the *Makefile*
of this project.
There is also the command `build_with_documentation` that allows to clean, build and generate the documentation.

**Important**: *Doxygen* must be installed before be used. Generating a *PDF*
file requires to have installed *pdflatex*, too.



## Short report

### What we did
We used the 'Pair Programming' technique: one at a time was coding and the others
were checking. All together discussed about design choices.
We started creating the header files and deciding the structure of the project.
We thought a lot about best choices for the project.

### What we understood
We understood:
- how to create and use *Makefile*;
- concepts discussed during lectures;
- how to work in group (online).

### Benchmarked
The command `make all_with_charts` is the same as `make all` and then creates benhmark plots.

Parameters for benchmark tests can be configured from the source file
*test/benchmark_test.h*. Results are available as CSV files in
the directory specified as parameter in the source file *test/benchmark_test.h*.

#### Results
We performed benchmark tests in which we compared *std::map* with our implementation of the
*Binary Search Tree*, with 10000 nodes; we repeated the tests 20 times. The plots with
results are listed below.

![Balancing](https://github.com/GiovanniMarchetto/advanced_programming_exam_2020/blob/main/plots/Balancing.png?raw=true)
![Copy](https://github.com/GiovanniMarchetto/advanced_programming_exam_2020/blob/main/plots/Copy.png?raw=true)
![Find by key](https://github.com/GiovanniMarchetto/advanced_programming_exam_2020/blob/main/plots/Find_by_key.png?raw=true)
![Find by key after balancing](https://github.com/GiovanniMarchetto/advanced_programming_exam_2020/blob/main/plots/Find_by_key_after_balancing.png?raw=true)
![Insertion](https://github.com/GiovanniMarchetto/advanced_programming_exam_2020/blob/main/plots/Insertion.png?raw=true)