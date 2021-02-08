#include <iostream>
#include <fstream>    // for writing to file
#include <filesystem> // create_directory

#include <map>
#include <vector>

#include <type_traits> // std::is_same<T,U>
#include <chrono>
#include <utility>

// For random generation
#include <cstdlib> // srand, rand
#include <ctime>   // time

#include "print_and_format.h"
#include "benchmark_test.h"

void benchmark_test(bool to_file)
{

    if (to_file) // results will be print to a file
    {
        // Remove the directory containing old results (if it exists)
        std::filesystem::remove_all(OUTPUT_RESULTS_DIR);

        // Create dir for results
        std::filesystem::create_directory(OUTPUT_RESULTS_DIR);
    }

    std::ofstream f; // fstream just declared in this scope

    /** Returns the output stream: if to_file is true, then the output stream
     * to the file with the name given as argument will be set. If the file
     * does not exist, it will be created, otherwise it will be replaced.
     * @param filename The string with the name of the file wher to save the
     *                  results, withou neither the full path nor the file
     *                  extension.*/
    auto get_ostream = [to_file, &f](const std::string &filename) -> std::ostream & {
        if (!to_file)
            return std::cout;

        // Otherwise, write to file
        std::ofstream tmp{OUTPUT_RESULTS_DIR + "/" + filename + ".csv"}; //previous content will be overwritten
        f = std::move(tmp);
        // Note: fstream will be closed by the dctor
        return f;
    };

    Benchmark_test<char> benchmark_test_{};
    std::cout << std::endl;

    std::cout << formatting_title() << "\n";
    std::cout << formatting_title("##### BENCHMARK TESTS ####", true) << "\n";
    std::cout << formatting_title() << "\n";
    std::cout << NUMBER_OF_ITERATIONS << " iterations will be executed for each test.\n";
    if (to_file)
        std::cout << "Results will be written to files in the directory " +
                         OUTPUT_RESULTS_DIR + "\n";

    std::cout << formatting_title("## Insertion  ") << "\n";
    benchmark_test_.insertion_test(get_ostream("insertion"));
    std::cout << "\n";

    std::cout << formatting_title("## Finding a node by key  ") << "\n";
    benchmark_test_.find_test(get_ostream("find_by_key"));
    std::cout << "\n";

    std::cout << formatting_title("## Balancing the tree  ") << "\n";
    benchmark_test_.balancing_test(get_ostream("balancing"));
    std::cout << "\n";

    std::cout << formatting_title("## Finding a node by key, after balancing the BST  ") << "\n";
    benchmark_test_.balance_and_find_test(get_ostream("find_by_key_after_balancing"));
    std::cout << std::endl;

    std::cout << formatting_title("## Copying the BST  ") << "\n";
    benchmark_test_.copy_bst_test(get_ostream("copy"));
    std::cout << std::endl;
}

/** Receives a lambda function as argument and executes it iteratively
 * for NUMBER_OF_ITERATIONS times.*/
template <typename F>
void iterate(F &lambda_fun)
{
    for (size_t i{0}; i < NUMBER_OF_ITERATIONS; ++i)
        lambda_fun();
}

/** Generates and returns an std::pair<int, char> with random generated
 * key and value.*/
std::pair<int, char> generate_pair_random()
{
    int random_key{rand()};
    char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
    return std::pair<int, char>(random_key, random_val);
}

/** Function to clear the given bst and map and then repopulate
 * with the given number of nodes, randomly generated.
 * Note: very expensive function.
 * If the flag only_bst is set to true, operations will affect only
 * the given BST.*/
template <typename value_type,
          typename key_type,
          typename OP>
void create_random_bst_and_map(bst<value_type, key_type, OP> &bst_,
                               std::map<key_type, value_type> &map_,
                               size_t N,
                               bool only_bst = false)
{
    bst_.clear();
    if (!only_bst)
        map_.clear();

    while (bst_.get_size() < N) // enforce the size (duplicates won't be inserted)
    {
        std::pair<int, char> pair{generate_pair_random()};
        bst_.insert(pair);
        if (!only_bst)
            map_.insert(pair);
    }
};

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
    insertion_test(std::ostream &os,
                   const size_t NUMBER_OF_NODES_INSERTION_BENCHMARK)
{

    if (!(std::is_same<key_type, int>::value &&
          std::is_same<value_type, char>::value)) // Check if right template types
    {
        os << "This test is currenty available only for integer key type and char value type";
        return os;
    }

    // Random generation of pairs with distinct keys (temporarily saved in a
    // vector to time only the insertion time)
    auto create_vector_of_nodes = [](const std::size_t &number_of_nodes_to_create, std::vector<std::pair<int, char>> &vec) {
        std::srand(std::time(NULL)); // random seed initialization

        auto find_pair_by_key_in_vector = [](const auto &key, const auto &vector) {
            for (const auto &el : vector)
                if (el.first == key)
                    return true;
            return false;
        };

        while (vec.size() < number_of_nodes_to_create)
        {
            // Random generation of a pair
            std::pair<int, char> pair{generate_pair_random()};
            if (!find_pair_by_key_in_vector(pair.first, vec))                 // if not already present
                vec.push_back(std::pair<int, char>(pair.first, pair.second)); // move insert
        }
    };
    std::vector<std::pair<int, char>> vector_of_nodes_to_insert{};
    create_vector_of_nodes(NUMBER_OF_NODES_INSERTION_BENCHMARK, vector_of_nodes_to_insert);

    os << HEADER_FOR_RESULTS;

    auto actual_test = [this, NUMBER_OF_NODES_INSERTION_BENCHMARK, &vector_of_nodes_to_insert, &os]() {
        bst_.clear(); // Clear the bst before inserting

        long int duration_insertion_in_our_tree{},
            duration_insertion_in_std_map{};
        for (std::size_t i{0}; i < NUMBER_OF_NODES_INSERTION_BENCHMARK; ++i)
        {
            std::pair<int, char> pair_to_insert{vector_of_nodes_to_insert.at(i)}; // the pair is temporarely saved here just before being inserted

            // Insertion in our tree
            auto t1 = std::chrono::high_resolution_clock::now();
            bst_.insert(pair_to_insert); // pair is copied
            auto t2 = std::chrono::high_resolution_clock::now();
            duration_insertion_in_our_tree += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            // Insertion in std::map
            t1 = std::chrono::high_resolution_clock::now();
            map_.insert(pair_to_insert); // pair is copied
            t2 = std::chrono::high_resolution_clock::now();
            duration_insertion_in_std_map += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            // Printing results for this iteration
            os << "\n"
               << i + 1 << "," << duration_insertion_in_our_tree << "," << duration_insertion_in_std_map;
        }
    };

    iterate(actual_test);

    // os << "\n";
    // os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into the tree in " << duration_insertion_in_our_tree << " us. ";
    // os << " Average insertion time: " << (duration_insertion_in_our_tree / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.\n";
    // os << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into an std::map in " << duration_insertion_in_std_map << " us. ";
    // os << " Average insertion time: " << (duration_insertion_in_std_map / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.";

    return os;
}

/** Finding test support function. This function performs the finding test
 * allowing to specify if balancing the tree before running the test: it can
 * be specified thanks to the parameter.*/
template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
    private_find_test(std::ostream &os, bool balance_before_test)
{

    os << HEADER_FOR_RESULTS;

    auto actual_test = [&os, balance_before_test]() {
        // bst and map that will be used in the test
        bst<char, int> bst_{};
        std::map<int, char> map_{};

        long int duration_search_in_our_tree{},
            duration_search_in_std_map{};
        for (std::size_t i{0}; i < DEFAULT_NUMBER_OF_NODES_FOR_TEST; ++i)
        {
            create_random_bst_and_map(bst_, map_, i);
            int random_key{rand()}; // key to find

            if (balance_before_test)
                bst_.balance();

            auto t1 = std::chrono::high_resolution_clock::now();
            bst_.find(random_key);
            auto t2 = std::chrono::high_resolution_clock::now();
            duration_search_in_our_tree += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            t1 = std::chrono::high_resolution_clock::now();
            bst_.find(random_key);
            t2 = std::chrono::high_resolution_clock::now();
            duration_search_in_std_map += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            os << "\n"
               << i + 1 << "," << duration_search_in_our_tree << "," << duration_search_in_std_map;
        }
    };

    iterate(actual_test);

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
    find_test(std::ostream &os)
{
    return private_find_test(os);
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
    balance_and_find_test(std::ostream &os)
{
    return private_find_test(os, false);
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::
    balancing_test(std::ostream &os)
{
    os << HEADER_FOR_RESULTS;

    auto actual_test = [&os]() {
        bst<char, int> bst_{};
        std::map<int, char> map_{};

        long int duration_balancing_our_tree{};
        for (std::size_t i{0}; i < DEFAULT_NUMBER_OF_NODES_FOR_TEST; ++i)
        {
            create_random_bst_and_map(bst_, map_, i, true);
            auto t1 = std::chrono::high_resolution_clock::now();
            bst_.balance();
            auto t2 = std::chrono::high_resolution_clock::now();

            duration_balancing_our_tree += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            // os << "Time for balancing the tree (" << bst_.get_size() << " nodes in the tree): " << duration_balancing_our_tree << " us." << std::endl;

            os << "\n"
               << i + 1 << "," << duration_balancing_our_tree;
        }
    };

    iterate(actual_test);

    return os;
}

template <typename value_type,
          typename key_type,
          typename OP>
std::ostream &Benchmark_test<value_type, key_type, OP>::copy_bst_test(std::ostream &os)
{

    os << HEADER_FOR_RESULTS;

    auto actual_test = [&os]() {
        // bst and map that will be used in the test
        bst<char, int> bst_{};
        std::map<int, char> map_{};

        long int duration_copy_in_our_tree{},
            duration_copy_in_std_map{};

        for (std::size_t i{0}; i < DEFAULT_NUMBER_OF_NODES_FOR_TEST; ++i)
        {
            create_random_bst_and_map(bst_, map_, i);

            auto t1 = std::chrono::high_resolution_clock::now();
            bst<value_type, key_type, OP> bst_copy{bst_};
            auto t2 = std::chrono::high_resolution_clock::now();
            duration_copy_in_our_tree += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            t1 = std::chrono::high_resolution_clock::now();
            std::map<key_type, value_type> map_copy{map_};
            t2 = std::chrono::high_resolution_clock::now();
            duration_copy_in_std_map += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            os << "\n"
               << i + 1 << "," << duration_copy_in_our_tree << "," << duration_copy_in_std_map;
        }
    };

    iterate(actual_test);

    return os;
}