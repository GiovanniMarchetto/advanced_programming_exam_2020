#include <functional> // std::less
#include <stdint.h>   // int32_t
#include <memory>     // unique_ptr
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
    using node = Node<value_type, key_type, OP>;

    /** Pointer to the root node of the tree.*/
    std::unique_ptr<node> tree_root_node;

    /** Number of nodes in the tree.*/
    std::size_t size;

    /** Getter for the root node. Returns a raw pointer.*/
    node *get_tree_root_node() const { return tree_root_node.get(); }

    /** Setter: the root node of this instance will be set with the given raw pointer.*/
    void set_tree_root_node(node *const node = nullptr) { tree_root_node.reset(node); }

    /** Given a pointer to a node, this function return the leftmost
     * (minimum) node in the substree whose root is the specified node.*/
    static node *get_minimum_left_node_in_subtree(node *node) // TODO : const??
    {
        while (node && node->get_left())
            node = node->get_left();

        return node;
    }

public:
    /** Function object that compares nodes by comparing of the
     * key components of the nodes.*/
    OP node_key_compare{}; // TODO : do we need this member?

    /** Class Iterator for the tree.*/
    template <typename O>
    class Iterator;
    using iterator = Iterator<node>;
    using const_iterator = Iterator<const node>;

    /** Begin function to use in a for-range loop. Returns an iterator.*/
    iterator begin() noexcept // the iterator does not acquire resource, so noexcept (nothing will go wrong)
    {
        return iterator{get_minimum_left_node_in_subtree(get_tree_root_node())};
    }
    /** Begin function to use in a for-range loop. Returns a const iterator.*/
    const_iterator begin() const noexcept // const-iterator ensure to be protected when invoking a const function
    {
        return const_iterator{get_minimum_left_node_in_subtree(get_tree_root_node())};
    }

    /** End function to use in a for-range loop. Returns an iterator.*/
    auto end() noexcept { return iterator{nullptr}; }
    /** End function to use in a for-range loop. Returns a const iterator.*/
    auto end() const noexcept { return const_iterator{nullptr}; }

    /** Default constructor.*/
    bst() = default;

    /** Default destructor for the proper cleanup.*/
    ~bst() = default;

    /** Copy constructor.*/
    bst(const bst &other) : size{other.size}
    {
        if (other.get_tree_root_node())
            set_tree_root_node(new node{*other.get_tree_root_node()});
    }

    /** Copy assignment.*/
    bst &operator=(const bst &other)
    {
        set_tree_root_node();
        auto tmp{bst(other)};
        *this = std::move(tmp);
        return *this;
    }

    /** Move constructor.*/
    bst(bst &&) = default;

    /** Move assignment.*/
    bst &operator=(bst &&) = default;

    /** Insertion of a new node. This function inserts the given
	 * node, but children are discarded.*/
    std::pair<iterator, bool> insert(const std::pair<key_type, value_type> &x) // TODO : noexcept ??
    {
        using Node = bst<value_type, key_type, OP>::node;
        Node *node = new Node{x};

        Node *prev{nullptr};
        Node *curr = get_tree_root_node();

        while (curr)
        {
            prev = curr;
            if (*node < *curr)
                curr = curr->get_left();
            else
                curr = curr->get_right();
        }

        if (prev && !(*node < *prev || *prev < *node)) // node == prev
        {
            delete node;
            return std::make_pair<iterator, bool>(iterator{prev}, false);
        }

        node->set_parent(prev);

        if (!prev) // if the tree was empty
            set_tree_root_node(node);
        else if (*node < *prev)
            prev->set_left(node);
        else
            prev->set_right(node);

        ++size;
        return std::make_pair<iterator, bool>(iterator{node}, true); // TODO : r-value? move assignment??
    }

    /** Overloading of the << operator. This function provides a view
     * of the tree, iterating over its nodes.*/
    friend std::ostream &operator<<(std::ostream &os, const bst &_bst)
    {
        os << "[size=" << _bst.size << "] { ";
        for (const auto &el : _bst)
            os << el << " ";
        os << "}";
        return os;
    }
};

#include "Iterator.h"
#include "bst.cpp" // TODO : https://stackoverflow.com/q/495021

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H
