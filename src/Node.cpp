#include <utility>    // std::pair, std::move
#include <iostream>

#include "Node.h"

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_NODE_CPP
#define ADVANCED_PROGRAMMING_EXAM_2020_NODE_CPP

template <typename value_type, typename key_type, typename OP>
Node<value_type, key_type, OP>::
Node(const Node<value_type, key_type, OP>& node,
    Node<value_type, key_type, OP>* parent)
    : key{ node.key }, value{ node.value }
{
    auto tmp_left = node.get_left();
    auto tmp_right = node.get_right();

    set_parent(parent);

    if (tmp_left)
        set_left(new Node(*tmp_left, this));
    if (tmp_right)
        set_right(new Node(*tmp_right, this));
}

template <typename value_type, typename key_type, typename OP>
Node<value_type, key_type, OP>& Node<value_type, key_type, OP>::
operator=(const Node<value_type, key_type, OP>& node)
{
    auto tmp{ Node(node) };   // invoke the copy ctor
    *this = std::move(tmp); // move assignment
    return *this;
}

template <typename value_type, typename key_type, typename OP>
std::ostream&
operator<<(std::ostream& os, const Node<value_type, key_type, OP>& node) noexcept
{
    os << "[" << node.key << " => " << node.value << "]";
    return os;
}

#endif // ADVANCED_PROGRAMMING_EXAM_2020_NODE_CPP