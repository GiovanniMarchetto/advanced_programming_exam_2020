#include <functional> // std::less
#include <stdint.h>	  // int32_t
#include <memory>	  // unique_ptr
// #include <iostream>

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_BST_H
#define ADVANCED_PROGRAMMING_EXAM_2020_BST_H

/**
 * Class for the binary search tree (BST).
 * The BST is sorted according to the keys.
 * @tparam value_type Type of the value of each node of the tree.
 * @tparam key_type Type of the key of each node of the tree (not
 *               required to be specified).
 * @tparam OP The type of the comparison operator, which is used
 *            to compare two keys (not required to be specified).
 */
template <typename value_type,
		  typename key_type = std::int32_t,
		  typename OP = std::less<key_type>>
class bst
{
};

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H
