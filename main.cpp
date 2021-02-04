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
    std::cout << "\nTEST: Insertion of a number of random generated nodes and printing:" << std::endl;
    int NUMBER_OF_NODES{50};
    int MAX_NUMBER_OF_KEY{15}; // if MAX_NUMBER_OF_KEY<NUMBER_OF_NODES test behaviour with duplicated keys
    bst<char> bst_3{};
    std::srand(std::time(NULL)); // random seed initialization
    //std::srand(123); // fixed seed for reproducible tests
    for (int i{0}; i < NUMBER_OF_NODES; ++i)
    {
        int random_key{rand() % MAX_NUMBER_OF_KEY};                              // between 0 and MAX_NUMBER_OF_KEY-1
        char random_val{static_cast<char>(rand() % 26 + static_cast<int>('a'))}; // between 'a' and 'z'
        bst_3.insert(std::pair<int, char>(random_key, random_val));              // move insert
    }

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

    return 0;
}