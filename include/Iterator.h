#include "Node.h"
#include "bst.h" //TODO: needed?

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
#define ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H

/**
 * Class for the iterator of the binary search tree (BST).
 * This class is heavy coupled with the class bst.
 * See the class bst for the template description.
 * @tparam value_type
 * @tparam key_type
 * @tparam OP
 * @tparam O Type of the object pointed by an instance 
 *              of this class.
 */
template <typename value_t,
          typename key_type,
          typename OP>
template <typename O>
class bst<value_t, key_type, OP>::Iterator
{
    O *current_node;

    /** Given the pointer to a node this function returns its successor. */
    static O *get_successor(O *node)
    {
        if (node->get_right())
            return get_minimum_left_node_in_subtree(node->get_right());

        auto parent = node->get_parent();
        while (parent && node == parent->get_right())
        {
            node = parent;
            parent = node->get_parent();
        }

        return parent;
    }

public:
    using value_type = O;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(O *p) : current_node{p} {}

    reference operator*() const { return *current_node; }
    pointer operator->() const { return &**this; }

    // pre-increment
    Iterator &operator++()
    {
        current_node = get_successor(current_node);
        return *this;
    }

    // post-increment
    Iterator operator++(int)
    {
        auto tmp{*this};
        ++(*this);
        return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b)
    {
        return a.current_node == b.current_node;
    }

    friend bool operator!=(const Iterator &a, const Iterator &b)
    {
        return !(a == b);
    }
};

#endif //ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
