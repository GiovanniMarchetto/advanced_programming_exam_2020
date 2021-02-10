#include <functional> // std::less
#include <stdint.h>   // std::int32_t
#include <memory>     // std::unique_ptr
#include <utility>    // std::pair
#include <iostream>

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_NODE_H
#define ADVANCED_PROGRAMMING_EXAM_2020_NODE_H

/** Class for a node in a Binary search tree.
 * @tparam value_type Type of the value of each node of the tree.
 * @tparam key_type Type of the key of each node of the tree (not
 *               required to be specified).
 * @tparam OP Type of the comparison operator, which is used
 *            to compare two keys (not required to be specified).
 */
template <typename value_type,
    typename key_type = std::int32_t,
    typename OP = std::less<key_type>>
    class Node
{
    /** Left child of this node.*/
    std::unique_ptr<Node> left;

    /** Right child of this node.*/
    std::unique_ptr<Node> right;

    /** Parent of this node.*/
    Node* parent;

public:
    /** Key of the node.*/
    key_type key;

    /** Value of the node.*/
    value_type value;

    /** Getter for the left children. 
     *  @returns The (raw) pointer to the left child node.
    */
    Node* get_left() const noexcept { return left.get(); }

    /** Getter for the right children.
     *  @returns The (raw) pointer to the right child node.
    */
    Node* get_right() const noexcept { return right.get(); }

    /** Getter for the parent.
     *  @returns The (raw) pointer to the parent node.
    */
    Node* get_parent() const noexcept { return parent; }

    /** Release the left children. 
     *  @returns The (raw) pointer to the left child node and releases the ownership.
     *  WARNING: The returned pointer is unmanaged and therefore it should be called delete on it.
    */
    Node* release_left() noexcept { return left.release(); }

    /** Release the right children.
     *  @returns The (raw) pointer to the right child node and releases the ownership.
     *  WARNING: The returned pointer is unmanaged and therefore it should be called delete on it.
    */
    Node* release_right() noexcept { return right.release(); }

    /** Setter: the left child of this instance will be set with the given pointer.
     *  @param node The pointer to the node to set. Default value is nullptr.
    */
    void set_left(Node* const node = nullptr) noexcept { left.reset(node); }

    /** Setter: the right child of this instance will be set with the given pointer.
     *  @param node The pointer to the node to set. Default value is nullptr.
    */
    void set_right(Node* const node = nullptr) noexcept { right.reset(node); }

    /** Setter: the parent of this instance will be set with the given node.
     *  @param node The pointer to the node to set. Default value is nullptr.
    */
    void set_parent(Node* const node = nullptr) noexcept { parent = node; }

    /** Default constructor.*/
    Node() = default;

    /** Copy constructor (deep copy). The given node and all the children
     * linked by it are copied. This constructor recursively copies children nodes.
     * @param node   The node that will be copied.
     * @param parent The pointer to the parent node. Default value is nullptr.
    */
    Node(const Node& node, Node* parent = nullptr);

    /** Copy assignment. See the constructor #Node(const Node&, Node*).
     *  @param node The node that will be copied.
    */
    Node& operator=(const Node& node);

    /** Move constructor.*/
    Node(Node&&) noexcept = default;

    /** Move assignment.*/
    Node& operator=(Node&&) noexcept = default;

    /** Constructor. Given arguments are copied into the members of the
     * constructing instance.
     * @param key Key of this node.
     * @param value Value of this node.*/
    Node(const key_type& key, const value_type& value) : left{}, right{}, key{ key }, value{ value } {}

    /** Constructor. Given arguments are moved into the members of the
     * constructing instance.
     * @param key Key of this node.
     * @param value Value of this node.*/
    Node(key_type&& key, value_type&& value) : left{}, right{}, key{ std::move(key) }, value{ std::move(value) } {}

    /** Default destructor.*/
    ~Node() noexcept = default;

    /** Comparison function according to the given Comparator
     * applied to the keys.
     * @param other The node to compare.
     * @param comparator The operator to use for compare the keys.
     * @returns The result of the given comparator applied to the key
     *          of this instance (as first argument) and the key of the given
     *          instance (as second argument).*/
    template <typename Comparator>
    bool compare(const Node& other, const Comparator comparator) const noexcept { return comparator(key, other.key); }

    /** Overload of the < operator to define an order relation
     * between two instances according to their keys.
     * @param other The node to compare.
     * @returns True if the key of this instance is strictly less
     *          of the one given as parameter, false otherwise.*/
    bool operator<(const Node& other) const noexcept { return compare(other, OP{}); }

    /** Overloading of the << operator. This function provides a view
     * of the node.
     * @param os Reference to output stream.
     * @param node The node to be printed.
     * @returns The reference to the output stream.
    */
    template <typename value_type_, typename key_type_, typename OP_>
    friend std::ostream& operator<<(std::ostream& os, const Node& node) noexcept;
};

#include "Node.cpp"

#endif //ADVANCED_PROGRAMMING_EXAM_2020_NODE_H
