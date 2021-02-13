#include <iostream>
#include <utility>
#include <string>
#include <vector>

#include "random_generator.h"
#include "test.h"
#include "Node.h"
#include "bst.h"
#include "print_and_format.h"

/** Principal bst for tests.*/
bst<char> bst_{};

/** Receives a function as argument and executes it iteratively
 * for sup_limit times, that is the second argument.
 * @param function The function to iterate.
 * @param sup_limit The number of iteration.
 *  Default value of 0 (no iteration without a value).
 */
template <typename F>
void iteration_func(const F& function, const int sup_limit = 0)
{
    for (int i{ 0 }; i < sup_limit; ++i)
        function(i + 1);
}

/** Node test with the different types of constructors,
 * assignments and comparison.
 * @param int The index of iteration.
*/
void node_operations_test(const int i = 1)
{
    Node<char> first_node(random_int(MAX_NUMBER_OF_KEY), random_char());
    Node<char> cpy_node{ first_node };
    Node<char> second_node(random_int(MAX_NUMBER_OF_KEY), random_char());
    if (i < NUMBER_OF_PRINT)
    {
        std::cout << "---[Index " << i << " ]" << std::endl;
        std::cout << "Address " << &first_node
            << " - Node ctr:                       " << first_node << std::endl;
        std::cout << "Address " << &cpy_node
            << " - Node copy ctr:                  " << cpy_node << std::endl;
        std::cout << "Address " << &second_node
            << " - Node copy assignment - before: " << second_node << std::endl;
    }

    second_node = first_node;
    if (i < NUMBER_OF_PRINT)
        std::cout << "Address " << &second_node
        << " - Node copy assignment - after:  " << second_node << std::endl;

    std::string not_a_word;
    const char a{ random_char() };
    const char b{ random_char() };
    not_a_word.append(2, a);
    not_a_word.append(3, b);

    Node<int, std::string> third_node(not_a_word, random_int(MAX_NUMBER_OF_KEY));

    first_node = Node<char>(random_int(MAX_NUMBER_OF_KEY), random_char());

    if (i < NUMBER_OF_PRINT)
    {
        std::cout << "Address " << &third_node
            << " - Node move ctr:                  " << third_node << std::endl;
        std::cout << "Address " << &first_node
            << " - Node move assignment:           " << first_node << std::endl;
        std::cout << "Comparison:         " << first_node << " < " << second_node
            << " ----> result " << (first_node < second_node) << std::endl;
        std::cout << "Inverse comparison: " << second_node << " < " << first_node
            << " ----> result " << (second_node < first_node) << std::endl;
    }
}

/** Node test with various types and classes.*/
void node_other_type_test()
{
    std::string k = "key_lv";
    std::string v = "value_lv";
    Node<std::string, std::string> sn(k, v);
    std::cout << "Copy - node of string:                       " << sn << std::endl;

    Node<std::string, std::string> sn2("key_rv", "value_rv");
    std::cout << "Move - node of string:                       " << sn2 << std::endl;

    Node<int, int> sn3(random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY));
    std::cout << "Move (but built-in types are copied anyway): " << sn3 << std::endl;

    std::vector<int> vec = { random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY) };
    Node<std::vector<int>> vnode(12, vec);
    std::cout << "Copy - node of vector<int>:                  " << vnode << std::endl;

    std::vector<int> vec2 = { random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY), random_int(MAX_NUMBER_OF_KEY) };
    Node<std::vector<int>> vnode2(random_int(MAX_NUMBER_OF_KEY), std::move(vec2));
    std::cout << "Move - node of vector<int>:                  " << vnode2 << std::endl;

    Node<bst<char>, int> n_bst(1, bst_);
    std::cout << "Copy - node of bst<char>:                     " << n_bst << std::endl;
}

/** Test function with different types of insertion
 *  (insert, emplace both in move and copy mode).
 * @param int The index of iteration.
*/
void bst_insertion_test(const int i = 1)
{

    std::pair<int, char> paio = std::pair<int, char>(random_int(MAX_NUMBER_OF_KEY), random_char());
    bst_.insert(paio);
    if (i == 0)
    {
        std::cout << formatting_title("## Copy insert ") << std::endl;
        std::string str{};
        std::cout << bst_.tree_structure_to_string(str) << std::endl;
    }

    bst_.insert(std::pair<int, char>(random_int(MAX_NUMBER_OF_KEY), random_char()));
    if (i == 0)
    {
        std::cout << formatting_title("## Move insert ") << std::endl;
        std::string str{};
        std::cout << bst_.tree_structure_to_string(str) << std::endl;
    }

    bst_.emplace(random_int(MAX_NUMBER_OF_KEY), random_char());
    if (i == 0)
    {
        std::cout << formatting_title("## Variadic emplace ") << std::endl;
        std::string str{};
        std::cout << bst_.tree_structure_to_string(str) << std::endl;
    }

    bst_.insert({ random_int(MAX_NUMBER_OF_KEY), random_char() });
    if (i == 0)
    {
        std::cout << formatting_title("## Move insert") << std::endl;
        std::cout << formatting_title("(implicit conversion from initializer list to std::pair) ") << std::endl;
        std::string str{};
        std::cout << bst_.tree_structure_to_string(str) << std::endl;
    }
}

/** Test function for the find.
 * It execute two times the find function and print the resulting
 * subtree starting from the key to find.
 * @param int The index of iteration.
*/
void find_node_test(const int i = 1)
{
    if (i < NUMBER_OF_PRINT)
    {
        std::cout << "---[Index " << i << " ]" << std::endl;
        int key_to_find{ random_int(MAX_NUMBER_OF_KEY) };
        std::cout << "Key to find: " << key_to_find << std::endl;
        bst<char>::iterator it{ bst_.find(key_to_find) };
        std::cout << "Print tree by using the iterator starting from the node marked with the key to find: " << std::endl;
        print_tree_from_iterator(it, bst_);
        const bst<char> bst_4{ bst_ };
        bst<char>::const_iterator cit{ bst_4.find(key_to_find) };
        std::cout << "Second try to find the key (start from a copy of the original tree): " << std::endl;
        print_tree_from_iterator(cit, bst_);
    }
    if (i == NUMBER_OF_PRINT)
    {
        std::cout << "---[Index " << i << " ]" << std::endl;
        std::cout << "Key to find: " << MAX_NUMBER_OF_KEY + 1 << std::endl;
        bst<char>::iterator it2{ bst_.find(MAX_NUMBER_OF_KEY + 1) };
        print_tree_from_iterator(it2, bst_);
    }
}

/** Test function for the subscripting operator.
 * It tries to overwrite, using the subscripting operator
 * passing first a r-value and then a l-value.
 * @param int The index of iteration.
*/
void subscripting_node_test(const int i = 1)
{
    int key_to_subscript = random_int(MAX_NUMBER_OF_KEY);
    char value_to_subscript = random_char();
    int key_to_subscript_2 = random_int(MAX_NUMBER_OF_KEY);
    char value_to_subscript_2 = random_char();

    if (i == NUMBER_OF_PRINT) {
        key_to_subscript = key_to_subscript + MAX_NUMBER_OF_KEY;
        key_to_subscript_2 = key_to_subscript_2 + MAX_NUMBER_OF_KEY;
    }

    if (i <= NUMBER_OF_PRINT)
    {
        std::cout << "---[Index " << i << " ]" << std::endl;

        std::cout << "Subscripting node (l-value) - "
            << "[" << key_to_subscript << " => "
            << value_to_subscript << "], "
            << "Subscripting node (r-value) - "
            << "[" << key_to_subscript_2 << " => "
            << value_to_subscript_2 << "]" << std::endl;

        std::cout << "Original tree:                  " << bst_ << std::endl;
    }

    bst_[key_to_subscript] = value_to_subscript;
    if (i <= NUMBER_OF_PRINT)
        std::cout << "Resulting tree (after l-value): " << bst_
        << std::endl;

    bst_[key_to_subscript_2] = std::move(value_to_subscript_2); //std::move not needed with char type but here we force it for testing
    if (i <= NUMBER_OF_PRINT)
        std::cout << "Resulting tree (after r-value): " << bst_
        << std::endl;

}

/** Test function for the erase.
 * Erase a random node from the bst.
 * @param int The index of iteration.
*/
void erase_test(const int i = 1)
{
    int key_erase_node = random_int(MAX_NUMBER_OF_KEY);
    bst_.erase(key_erase_node);

    if (i < NUMBER_OF_PRINT)
    {
        std::cout << "---[Index " << i << " ]" << std::endl;
        std::cout << "Erase node - "
            << "[" << key_erase_node << "]" << std::endl;
        std::cout << "Resulting tree: " << bst_
            << std::endl;
        std::string str2{};
        std::cout << bst_.tree_structure_to_string(str2) << std::endl;
    }
}

void tests()
{

    std::cout << formatting_title() << std::endl;
    std::cout << formatting_title("##### NODE TESTS #####", true) << std::endl;
    std::cout << formatting_title() << std::endl
        << std::endl;

    std::cout << formatting_title("## Nodes operations ##", true) << std::endl;
    iteration_func(node_operations_test, NUMBER_OF_NODES);
    std::cout << formatting_title("# Results #", true) << std::endl;
    std::cout << NUMBER_OF_NODES << " repetitions of nodes operations - done" << std::endl;
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Copy and move semantic of the value of a node during its construction") << std::endl;
    node_other_type_test();
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title() << std::endl;
    std::cout << formatting_title("##### BST TESTS #####", true) << std::endl;
    std::cout << formatting_title() << std::endl
        << std::endl;

    std::cout << formatting_title("## BST construction ##", true) << std::endl;
    iteration_func(bst_insertion_test, NUMBER_OF_NODES_BST);
    std::cout << formatting_title("# Final tree (traversal) #", true) << std::endl;
    std::cout << bst_ << std::endl;
    std::cout << formatting_title("# Final tree (tree shape) #", true) << std::endl;
    std::string str{};
    std::cout << bst_.tree_structure_to_string(str) << std::endl;
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## BST copy semantics") << std::endl;
    bst<char> bst_2{ bst_ };
    std::cout << bst_2 << std::endl;
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Balance") << std::endl;
    bst_.balance();
    std::cout << "Traversal: " << bst_ << std::endl
        << std::endl;
    str = "";
    std::cout << bst_.tree_structure_to_string(str) << std::endl;
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Finding a node") << std::endl;
    iteration_func(find_node_test, NUMBER_OF_NODES);
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Subscripting operator") << std::endl;
    iteration_func(subscripting_node_test, NUMBER_OF_NODES);
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Erase") << std::endl;
    std::cout << "Original tree:  " << bst_ << std::endl;
    iteration_func(erase_test, NUMBER_OF_NODES);
    std::cout << formatting_title("# Final tree (after all erase repetitions) #", true) << std::endl;
    std::cout << bst_ << std::endl;
    std::string str2{};
    std::cout << bst_.tree_structure_to_string(str2) << std::endl;
    std::cout << std::endl
        << std::endl;

    std::cout << formatting_title("## Clear") << std::endl;
    std::cout << "Original tree (the copy): " << bst_2 << std::endl;
    bst_2.clear();
    std::cout << "Clear tree:    " << bst_2 << std::endl;
    std::cout << std::endl
        << std::endl;
}
