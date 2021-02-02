#include <functional> // std::less
#include <stdint.h>	  // int32_t
#include <memory>	  // unique_ptr
#include "Node.h"

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
    using node = Node<value_type, key_type>;

	/** Pointer to the root node of the tree.*/
	std::unique_ptr<node> root;

	/** Number of nodes in the tree.*/
	std::size_t size;

	/** Class Iterator for the tree.
     * @tparam O The value type for the iterator.*/
	template <typename O>
	class Iterator;

public:

    /** Function object that compares nodes by comparing of the
     * key components of the nodes.*/
    OP node_key_compare{};

	/** Default constructor.*/
	bst() = default;

	/** Default destructor for the proper cleanup.*/
	~bst() = default;

	void insert(const std::pair<const key_type, value_type> &x);
};

#include "Iterator.h"
#include "bst.cpp" // TODO : https://stackoverflow.com/q/495021

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H
