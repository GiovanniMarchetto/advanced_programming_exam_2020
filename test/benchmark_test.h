// TODO: check which of the following includes are really required
#include <iostream>
#include <fstream>     // for writing to file
#include <filesystem>  // create_directory
#include <utility>
#include <string>
#include <regex> // regex used for substring replacement
// #include "Node.h"
#include "../include/bst.h"
#include <vector>

#include <type_traits> // For std::is_same<T,U>

// For random generation
#include <cstdlib> // srand, rand
#include <ctime>   // time

// For benchmark timing test and comparison
#include <map>
#include <chrono>


// --------  CONFIGURABLE PARAMETERS FOR TESTS         --------

/** Number of iterations to run a code fragment when
 * statistical information are required.*/
constexpr static unsigned int NUMBER_OF_ITERATIONS{5};  // TODO : set a reasonable big value

/** Default value for the number of nodes of the BST to be used in a test.*/
constexpr static size_t DEFAULT_NUMBER_OF_NODES_FOR_TEST{5};// TODO : set a reasonable big value

/** Default value for a key to find in the BST.*/
constexpr static size_t DEFAULT_KEY_FOR_TEST{10};

/** Path of the directory where to save files with the test results.*/
const static std::string OUTPUT_RESULTS_DIR{"./benchmark_results"};

// --------  END OF CONFIGURABLE PARAMETERS FOR TESTS  --------
// --------  DO NOT MODIFY UNDER THIS LINE             --------




/** Struct containing all benchmark tests. Note: each test is
 * run separately from others and new instances are used for
 * each test. Templates are specified in the class of the BST.*/
template <typename value_type,
          typename key_type = std::int32_t,
          typename OP = std::less<key_type>>
class Benchmark_test
{
    /** Header used in printed results*/
    std::string HEADER_FOR_RESULTS{"n,bst [µs],std::map [µs]"};
    
    /** Instance of BST for tests.*/
    bst<value_type, key_type, OP> bst_;

    /** Instance of std::map used for comparisons.*/
    std::map<key_type, value_type> map_;

public:

    /** Constructor. Sets up the environment for a test.*/
    Benchmark_test() : bst_{} {};

    // TODO : set random seed? Not required? Let the user to set before initializing the test?

    /** Benchmark test for the time of inserting a node in the BST.
     * @param os Output stream where to print test results.
     * @param NUMBER_OF_NODES_INSERTION_BENCHMARK Number of nodes to be inserted in
     *                                            this test. If not specified, the
     *                                            value specifed as member of this
     *                                            class will be used.
     */
    std::ostream &insertion_test(std::ostream &os,
                                 const size_t NUMBER_OF_NODES_INSERTION_BENCHMARK = DEFAULT_NUMBER_OF_NODES_FOR_TEST);

    /** Benchmark test for the time of finding a node in the BST, given the key.
     * @param os Output stream where to print test results.
     * @param key_to_find The key of the node to find.
     */
    std::ostream &find_test(std::ostream &os, const key_t key_to_find = DEFAULT_KEY_FOR_TEST);

    /** Benchmark test for the time of balancing the BST.
     * @param os Output stream where to print test results.
     */
    std::ostream &balancing_test(std::ostream &os);

    /** Benchmark test for the time of copying the BST.
     * @param os Output stream where to print test results.
     */
    std::ostream &copy_bst_test(std::ostream &os);
};

/** Performs all benchmark tests and prints results into an output stream,
 * according to the boolean flag given as argument: if the flag is true, then
 * a file with results is created, otherwise the std:.out will be used.
 * The flag is set to true as default.*/
void benchmark_test(bool to_file=true);