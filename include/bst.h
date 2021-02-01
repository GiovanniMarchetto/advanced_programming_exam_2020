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
	/** Instantiation of a comparison operator object.*/
    OP op{};

	/** Class for a node in the Binary search tree.*/
	class Node
	{
		/** Key of the node.*/
		key_type key;

		/** Value of the node.*/
		value_type value;

		/** Left child of this node.*/
		std::unique_ptr<Node> left;

		/** Right child of this node.*/
		std::unique_ptr<Node> right;

	public:
		/** Create a new node.
		 * @param key Key of this node.
		 * @param value Value of this node.*/
		Node(const key_type key, const value_type value);

		/** Overload of the < operator to define an order relation
		 * between nodes according to the class template */
		//friend bool operator<(const Node& lhs, const Node& rhs);
		bool operator<(const Node &other) const;
	};

	/** Pointer to the root node of the tree.*/
	std::unique_ptr<Node> root;

	/** Number of nodes in the tree.*/
	std::size_t size;

public:
	/** Default constructor.*/
	bst() = default;

	/** Default destructor for the proper cleanup.*/
	~bst() = default;

	void insert(const std::pair<const key_type, value_type> &x);
};

#include "bst.cpp"
#include "Node.cpp"
#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H
