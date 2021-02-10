#include "Node.h"
#include "bst.h"
#include "ap_error_ext.h"
#include "Iterator.h"

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_CPP
#define ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_CPP

template <typename value_t, typename key_type, typename OP>
template <typename O>
O* bst<value_t, key_type, OP>::Iterator<O>::
get_successor(O* node) noexcept
{
    if (!node)
        return nullptr;

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

template <typename value_t, typename key_type, typename OP>
template <typename O>
typename bst<value_t, key_type, OP>::template Iterator<O>::reference
bst<value_t, key_type, OP>::Iterator<O>::
operator*() const
{
    //TODO: stringa literal costly?
    AP_ERROR_IF_NULLPTR_WITH_MSG(current_node, "Nullptr cannot be de-reference", false);
    return *current_node;
}

template <typename value_t, typename key_type, typename OP>
template <typename O>
typename bst<value_t, key_type, OP>::template Iterator<O>::pointer
bst<value_t, key_type, OP>::Iterator<O>::
operator->() const
{
    AP_ERROR_IF_NULLPTR_WITH_MSG(current_node, "Nullptr cannot access any member", false);
    return &**this;
}

template <typename value_t, typename key_type, typename OP>
template <typename O>
typename bst<value_t, key_type, OP>::template Iterator<O>&
bst<value_t, key_type, OP>::Iterator<O>::
operator++() noexcept
{
    current_node = get_successor(current_node);
    return *this;
}

template <typename value_t, typename key_type, typename OP>
template <typename O>
typename bst<value_t, key_type, OP>::template Iterator<O>
bst<value_t, key_type, OP>::Iterator<O>::
operator++(int) noexcept
{
    auto tmp{ *this };
    ++(*this);
    return tmp;
}

#endif // ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_CPP