#include "Node.h"
#include "bst.h" //TODO: needed?

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
#define ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H

template <typename value_t,
          typename key_type,
          typename OP>
template <typename O>
class bst<value_t, key_type, OP>::Iterator
{
    using node = Node<value_t, key_type, OP>;
    node *current_node;

    static node *get_successor(node *node) // TODO : const??
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

    explicit Iterator(node *p) : current_node{p} {}

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

    //getter and setter
    node *get_current() const { return current_node; }
    void set_current(node *p) { current_node = p; }
};

#endif //ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
