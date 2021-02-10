#include "Node.h"
#include "bst.h"

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
    O* current_node;

    /** Given the pointer to a node this function returns its successor.
     *  If the given node is null, nullptr is returned.
    */
    static O* get_successor(O* node) noexcept;

public:
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(O* p) : current_node{ p } {}

    reference operator*() const;

    pointer operator->() const;

    /** Pre-increment operator. When the end of the container is
     * reached (hence, when the current node is nullptr), this
     * method can still be invoked and it returns nullptr.
     */
    Iterator& operator++() noexcept;

    /** Post-increment operator. When the end of the container is
     * reached (hence, when the current node is nullptr), this
     * method can still be invoked and it returns nullptr.
     */
    Iterator operator++(int) noexcept;

    friend bool operator==(const Iterator& a, const Iterator& b) noexcept { return a.current_node == b.current_node; }

    friend bool operator!=(const Iterator& a, const Iterator& b) noexcept { return !(a == b); }
};

#include "Iterator.cpp"

#endif //ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
