#include <iostream>
#include <utility>
#include <string>
#include <iostream>
#include "Node.h"
#include "bst.h"
#include <vector>

// For random generation
#include <cstdlib> // srand, rand
#include <ctime>   // time

// For benchmark timing test and comparison
#include <map>
#include <chrono>

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec) // TODO : to be deleted (was used just for testing copy/move ctr in Node.h (the ones which take key and value))
{
    os << "\n";
    for (size_t i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    os << std::endl;
    return os;
}
void print_tree_from_iterator(bst<char>::iterator &iterator, bst<char> &bst)
{
    while (iterator != bst.end())
    {
        std::cout << *iterator << " ";
        ++iterator;
    }
    std::cout << std::endl;
};
void print_tree_from_iterator(bst<char>::const_iterator &iterator, const bst<char> &bst)
{
    while (iterator != bst.cend())
    {
        std::cout << *iterator << " ";
        ++iterator;
    }
    std::cout << std::endl;
};

// void create_vector_of_nodes (const std::size_t& number_of_nodes_to_create, std::vector<std::pair<int, char>>& vec) {

//     auto find_pair_by_key_in_vector = [](const auto& key, const auto& vector){
//         for (const auto& el : vector )
//             if (el.first == key) return true;
//         return false;
//     };

//     while( vec.size() < number_of_nodes_to_create )
//     {
//         // Random generation of a pair
//         int random_key{rand()};
//         char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
//         if ( ! find_pair_by_key_in_vector(random_key, vec) )                 // if not already present
//             vec.push_back(std::pair<int, char>(random_key, random_val));// move insert
//     }

// };

int main()
{
    std::cout << "--------------------------------" << std::endl;
    std::cout << "----------Hello World!----------" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    // TEST: nodes construction
    std::cout << "--------------TEST: nodes construction-----------------------" << std::endl;
    Node<char> D(4, 'D');
    Node<char> C(3, 'C');
    Node<char> B(2, 'B');
    Node<char> A(1, 'A');

    // TEST: copy ctr of nodes  // TODO : check this
    std::cout << "--------------TEST: copy ctr of nodes------------------------" << std::endl;
    Node<char> A_copia{A};
    Node<char> C_copia{C};

    // TEST: copy assignment of nodes
    std::cout << "--------------TEST: copy assignment of nodes-----------------" << std::endl;
    D = A;

    // TEST: move constructor of nodes
    std::cout << "--------------TEST: move constructor of nodes----------------" << std::endl;
    Node<int, std::string> F("rr", 5);

    // TEST: move assignment of nodes
    std::cout << "--------------TEST: move assignment of nodes-----------------" << std::endl;
    A = Node<char>(8, 'h');

    // TEST: comparison of nodes
    std::cout << "--------------TEST: comparison of nodes----------------------" << std::endl;
    std::cout << (B < C) << " "; // expected: 1
    std::cout << (C < B) << " "; // expected: 0
    //std::cout << (F < D) << std::endl;                      // expected: exception (?)   // TODO : test well comparison functions
    std::cout << C.compare(B) << " ";       // expected: 0
    std::cout << B.compare(C) << " ";       // expected: 1
    std::cout << C.compare(B) << std::endl; // expected: 0

    // TEST: insertion of a node
    std::cout << "--------------TEST: insertion of a node----------------------" << std::endl;
    bst<char> bst_{};
    auto paio = std::pair<int, char>(2, 'y');
    bst_.insert(paio);                         //expected: copy insert
    bst_.insert(std::pair<int, char>(0, 'z')); //expected: move insert
    bst_.insert(std::pair<int, char>(1, 'x')); //expected: move insert
    bst_.insert(std::pair<int, char>(3, 'w')); //expected: move insert
    bst_.emplace(4, 'c');                      //expected: variadic emplace
    bst_.insert({5, 'f'});                     //expected: move insert (implicit conversion from initializer list to std::pair)
    std::cout << bst_ << std::endl;

    // // TEST: to try the copy semantics of bst
    std::cout << "--------------TEST: to try the copy semantics of bst---------" << std::endl;
    bst<char> bst_2{bst_};
    std::cout << bst_2 << std::endl;

    // TEST: copy and move semantic of the value of a node during its construction
    std::cout << "--------------TEST: copy and move semantic of the value------" << std::endl;
    std::cout << "--------------        of a node during its construction------" << std::endl;
    std::string k = "key_lv";
    std::string v = "value_lv";
    Node<std::string, std::string> sn2(k, v);                // value is copied
    Node<std::string, std::string> sn("key_rv", "value_rv"); // value is moved
    Node<int, int> sn3(4, 88);                               // value is moved (but built-in types are copied anyway)
    std::vector<int> vec = {7, 5, 16, 8};
    std::vector<int> vec2 = {10, 11, 12, 25};
    Node<std::vector<int>> vnode(12, vec);              // value is copied
    Node<std::vector<int>> vnode2(12, std::move(vec2)); // value is moved
    //Node<std::vector<int>> vnode3(12, std::move(*(new std::vector<int>{1, 2, 3}))); // value is moved -> memory leak due to the user
    Node<bst<char>, int> n_bst(1, bst_); // value is copied
    std::cout << n_bst << std::endl;

    // TEST: Insertion of a number of random generated nodes and printing
    std::cout << "\n--------------TEST: Insertion of a number of random---------\n"
                 "--------------        generated nodes and printing:---------"
              << std::endl;
    int NUMBER_OF_NODES{50};
    int MAX_NUMBER_OF_KEY{15}; // if MAX_NUMBER_OF_KEY<NUMBER_OF_NODES test behaviour with duplicated keys
    bst<char> bst_3{};
    //std::srand(std::time(NULL)); // random seed initialization
    std::srand(123); // fixed seed for reproducible tests
    for (int i{0}; i < NUMBER_OF_NODES; ++i)
    {
        int random_key{rand() % MAX_NUMBER_OF_KEY};                              // between 0 and MAX_NUMBER_OF_KEY-1
        char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
        bst_3.insert(std::pair<int, char>(random_key, random_val));              // move insert
    }
    //bst_3.unbalance();
    bst_3.balance();
    // Print the tree structure
    std::string str{};
    std::cout << bst_3.print_tree(str) << std::endl;
    // Print tree traversal
    std::cout << bst_3 << std::endl;

    // TEST: Finding a node
    std::cout << "--------------TEST: finding a node by key -------------------" << std::endl;
    int key_to_find{5};
    std::cout << "Key to find: " << key_to_find << std::endl;
    bst<char>::iterator it{bst_3.find(key_to_find)};
    const bst<char> bst_4{bst_3};
    bst<char>::const_iterator cit{bst_4.find(key_to_find)};
    // Print the tree by using the iterator starting from the node marked with the key to find.
    print_tree_from_iterator(it, bst_3);
    print_tree_from_iterator(cit, bst_3);

    // TEST: Subscripting operator
    std::cout << "--------------TEST: subscripting operator -------------------" << std::endl;
    int key_to_subscript = 3;
    int key_to_subscript_noexist = 7;
    std::cout << "Original bst:                  " << bst_ << std::endl;
    bst_[key_to_subscript] = 'p';
    bst_[key_to_subscript_noexist] = 'i';
    std::cout << "Subscripting bst with l-value: " << bst_ << std::endl;
    bst_[1] = 'g';
    bst_[8] = 'o';
    std::cout << "Subscripting bst with r-value: " << bst_ << std::endl;

    return 0;








    // TEST (BENCHMARK): Insertion time
    std::cout << "\n--------------TEST (BENCHMARK): Insertion time: -------------" << std::endl;
    std::size_t NUMBER_OF_NODES_INSERTION_BENCHMARK{10000};
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
            int random_key{rand()};
            char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
            if (!find_pair_by_key_in_vector(random_key, vec))                        // if not already present
                vec.push_back(std::pair<int, char>(random_key, random_val));         // move insert
        }
    };
    std::vector<std::pair<int, char>> vector_of_nodes_to_insert{};
    create_vector_of_nodes(NUMBER_OF_NODES_INSERTION_BENCHMARK, vector_of_nodes_to_insert);

    bst<char, int> bst_benchmark_insertion{};
    std::map<int, char> std_map_benchmark_insertion{};
    long int duration_insertion_in_our_tree{},
        duration_insertion_in_std_map{};
    for (std::size_t i{0}; i < NUMBER_OF_NODES_INSERTION_BENCHMARK; ++i)
    {
        std::pair<int, char> pair_to_insert{vector_of_nodes_to_insert.at(i)}; // the pair is temporarely saved here just before being inserted

        // Insertion in our tree
        auto t1 = std::chrono::high_resolution_clock::now();
        bst_benchmark_insertion.insert(pair_to_insert); // pair is copied
        auto t2 = std::chrono::high_resolution_clock::now();
        duration_insertion_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        // Insertion in std::map
        t1 = std::chrono::high_resolution_clock::now();
        std_map_benchmark_insertion.insert(pair_to_insert); // pair is copied
        t2 = std::chrono::high_resolution_clock::now();
        duration_insertion_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    std::cout << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into the tree in " << duration_insertion_in_our_tree << " us. ";
    std::cout << " Average insertion time: " << (duration_insertion_in_our_tree / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us.\n";
    std::cout << NUMBER_OF_NODES_INSERTION_BENCHMARK << " nodes have been inserted into an std::map in " << duration_insertion_in_std_map << " us. ";
    std::cout << " Average insertion time: " << (duration_insertion_in_std_map / (0.0 + NUMBER_OF_NODES_INSERTION_BENCHMARK)) << " us." << std::endl;

    // TEST (BENCHMARK): Search time
    std::cout << "\n--------------TEST (BENCHMARK): Search time: ----------------" << std::endl;
    int KEY_TO_FIND{0}; // anything
    long int duration_search_in_our_tree{},
        duration_search_in_std_map{};
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        bst_benchmark_insertion.find(KEY_TO_FIND);
        auto t2 = std::chrono::high_resolution_clock::now();
        duration_search_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        t1 = std::chrono::high_resolution_clock::now();
        std_map_benchmark_insertion.find(KEY_TO_FIND);
        t2 = std::chrono::high_resolution_clock::now();
        duration_search_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    std::cout << "Time for finding a key (" << bst_benchmark_insertion.get_size() << " nodes in the tree): " << duration_search_in_our_tree << " us.\n";
    std::cout << "Time for finding a key (" << bst_benchmark_insertion.get_size() << " nodes in the std::map): " << duration_search_in_std_map << " us."
              << std::endl;

    // TEST (BENCHMARK): Time for balancing the tree
    std::cout << "\n--------------TEST (BENCHMARK): Balancing the tree: ---------" << std::endl;
    long int duration_balancing_our_tree{};
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        bst_benchmark_insertion.balance();
        auto t2 = std::chrono::high_resolution_clock::now();
        duration_balancing_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    std::cout << "Time for balancing the tree (" << bst_benchmark_insertion.get_size() << " nodes in the tree): " << duration_balancing_our_tree << " us." << std::endl;

    // TEST (BENCHMARK): Search time
    std::cout << "\n--------------TEST (BENCHMARK): Search time after -----------"
              << "\n--------------                    balancing the tree: ------- " << std::endl;
    duration_search_in_our_tree = 0,
    duration_search_in_std_map = 0;
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        bst_benchmark_insertion.find(KEY_TO_FIND);
        auto t2 = std::chrono::high_resolution_clock::now();
        duration_search_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        // For std::map should be same as before (nothing changed)
        t1 = std::chrono::high_resolution_clock::now();
        std_map_benchmark_insertion.find(KEY_TO_FIND);
        t2 = std::chrono::high_resolution_clock::now();
        duration_search_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    std::cout << "Time for finding a key (" << bst_benchmark_insertion.get_size() << " nodes in the tree): " << duration_search_in_our_tree << " us.\n";
    std::cout << "Time for finding a key (" << bst_benchmark_insertion.get_size() << " nodes in the std::map): " << duration_search_in_std_map << " us."
              << std::endl;

    // TEST (BENCHMARK): Copy time
    std::cout << "\n--------------TEST (BENCHMARK): Copy time: ----------------" << std::endl;
    long int duration_copy_in_our_tree{},
        duration_copy_in_std_map{};
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        bst<char> bst_copy{bst_benchmark_insertion}; // TODO : copy ctr
        auto t2 = std::chrono::high_resolution_clock::now();
        duration_copy_in_our_tree += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        t1 = std::chrono::high_resolution_clock::now();
        std::map<int, char> map_copy{std_map_benchmark_insertion};
        t2 = std::chrono::high_resolution_clock::now();
        duration_copy_in_std_map += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    }

    std::cout << "Time for deep copy of our tree (" << bst_benchmark_insertion.get_size() << " nodes in the tree): " << duration_copy_in_our_tree << " us.\n";
    std::cout << "Time for deep copy of std::map (" << bst_benchmark_insertion.get_size() << " nodes in the std::map): " << duration_copy_in_std_map << " us.\n\n"
              << std::endl;

    return 0;
}