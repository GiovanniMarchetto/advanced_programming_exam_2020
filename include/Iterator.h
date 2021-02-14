#include "Node.h"
#include "bst.h"

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
#define ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H

/**
 * Class for the iterator of the binary search tree (BST).
 * This class is heavy coupled with the class bst.
 * See the class bst for the template description.
 * @tparam value_type Type of the value of each node of the tree.
 * @tparam key_type Type of the key of each node of the tree (not
 *               required to be specified).
 * @tparam OP Type of the comparison operator, which is used
 *            to compare two keys (not required to be specified).
 * @tparam O Type of the object pointed by an instance
 *              of this class.
 */
template <typename value_t,
    typename key_type,
    typename OP>
    template <typename O>
/*!
*  \authors    Massimiliano Cristarella
*  \authors    Matteo Ferfoglia
*  \authors    Giovanni Marchetto
*/
class bst<value_t, key_type, OP>::Iterator
{
    O* current_node;

    /** Given the pointer to a node this function returns its successor.
     *  If the given node is null, nullptr is returned.
     *  @param node The node of which to get the successor.
     *  @returns The successor of the given node.
    */
    static O* get_successor(O* node) noexcept;

public:
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    /** Constructor of an iterator pointing to the given node.
     *  @param p The pointer of the node.
    */
    explicit Iterator(O* p) : current_node{ p } {}

    /** Dereference operator.
     * @returns The reference to the currently pointed node.
    */
    reference operator*() const;

    /** Structure dereference operator.
     * @returns The pointer to the currently pointed node.
    */
    pointer operator->() const;

    /** Pre-increment operator. When the end of the container is
     * reached (hence, when the current node is nullptr), this
     * method can still be invoked and it returns nullptr.
     * @returns The reference to the pre-incremented iterator.
     */
    Iterator& operator++() noexcept;

    /** Post-increment operator. When the end of the container is
     * reached (hence, when the current node is nullptr), this
     * method can still be invoked and it returns nullptr.
     * @returns The iterator before being incremented.
     */
    Iterator operator++(int) noexcept;

    /** Tests if the iterator object on the left side of the operator
     * is equal to the iterator object on the right side.
     * The equality is evaluated true if the given iterators
     * are pointing to the same node.
     * @param a The left side iterator object.
     * @param b The right side iterator object.
     * @returns True if the iterator objects are equal;
     * false if the iterator objects are not equal.
     */
    friend bool operator==(const Iterator& a, const Iterator& b) noexcept { return a.current_node == b.current_node; }

    /** Tests if the iterator object on the left side of the operator
     * is not equal to the iterator object on the right side.
     * The equality is evaluated true if the given iterators
     * are not pointing to the same node.
     * @param a The left side iterator object.
     * @param b The right side iterator object.
     * @returns True if the iterators are not equal;
     * false if the iterators are equal.
     */
    friend bool operator!=(const Iterator& a, const Iterator& b) noexcept { return !(a == b); }
};

#include "Iterator.cpp"

#endif //ADVANCED_PROGRAMMING_EXAM_2020_ITERATOR_H
