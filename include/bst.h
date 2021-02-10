#include <functional> // std::less
#include <stdint.h>   // int32_t
#include <memory>     // unique_ptr
#include <utility>    // std::pair
#include <string>

#include "Node.h"


#ifndef ADVANCED_PROGRAMMING_EXAM_2020_BST_H
#define ADVANCED_PROGRAMMING_EXAM_2020_BST_H

/**
 * Class for the binary search tree (BST).
 * The BST is sorted according to the keys.
 * @tparam value_type Type of the value of each node of the tree.
 * @tparam key_type Type of the key of each node of the tree (not
 *               required to be specified).
 * @tparam OP Type of the comparison operator, which is used
 *            to compare two keys (not required to be specified).
 */
template <typename value_type,
    typename key_type = std::int32_t,
    typename OP = std::less<key_type>>
    class bst
{
    /** Alias for a templated node. */
    using node = Node<value_type, key_type, OP>;

    /** Pointer to the root node of the tree.*/
    std::unique_ptr<node> tree_root_node;

    /** Number of nodes in the tree.*/
    std::size_t size;

    /** Getter for the root node.
     * @returns The (raw) pointer to the root node.
     */
    node* get_tree_root_node() const noexcept { return tree_root_node.get(); }

    /** Release the root node.
     *  @returns The (raw) pointer to the root node and releases the ownership.
     *  WARNING: The returned pointer is unmanaged and therefore it should be called delete on it.
    */
    node* release_tree_root_node() noexcept { return tree_root_node.release(); }

    /** Setter: the root node of this instance will be set with the given pointer.
     *  @param node The pointer to the node to set. Default value is nullptr.
    */
    void set_tree_root_node(node* const node = nullptr) noexcept { tree_root_node.reset(node); }

    /** Releases the given node from its parent or from the tree if it's the root node.
     * @param node_to_release The pointer to the node to realese.
     * @returns The (raw) pointer to the released node.
     * WARNING: The returned pointer is unmanaged and therefore it should be called delete on it. */
    node* release_node(const node* const node_to_release);

    /** Given a pointer to a node, this function return the leftmost
     * (minimum) node in the subtree whose root is the specified node.
     * @param node The root node of the subtree.
     * @returns The minimum left child node in the subtree.*/
    static node* get_minimum_left_node_in_subtree(node* node) noexcept;

public:
    /** Function object that compares nodes by comparing of the
     * key components of the nodes.*/
    OP node_key_compare{};

    /** Getter for the number of nodes in the tree.
     * @returns The number of nodes in the tree.
    */
    size_t get_size() const noexcept { return size; }

    /** Class Iterator for the tree.*/
    template <typename O>
    class Iterator;
    using iterator = Iterator<node>;
    using const_iterator = Iterator<const node>;

    /** Begin function to use in a for-range loop. @returns An iterator.*/
    iterator begin() noexcept { return iterator{ get_minimum_left_node_in_subtree(get_tree_root_node()) }; }
    /** Cbegin function to use in a for-range loop. @returns a const iterator.*/
    const_iterator cbegin() const noexcept { return const_iterator{ get_minimum_left_node_in_subtree(get_tree_root_node()) }; }
    /** Begin function to use in a for-range loop. @returns a const iterator.*/
    const_iterator begin() const noexcept { return cbegin(); }

    /** End function to use in a for-range loop. @returns an iterator.*/
    iterator end() noexcept { return iterator{ nullptr }; }
    /** Cend function to use in a for-range loop. @returns a const iterator.*/
    const_iterator cend() const noexcept { return const_iterator{ nullptr }; }
    /** End function to use in a for-range loop. @returns a const iterator.*/
    const_iterator end() const noexcept { return cend(); }

    /** Default constructor.*/
    bst() = default;

    /** Default destructor for the proper cleanup.*/
    ~bst() noexcept = default;

    /** Copy constructor. Deep copy is performed.
     * @param other The bst that will be copied.
    */
    bst(const bst& other);

    /** Copy assignment.*/
    bst& operator=(const bst& other);

    /** Move constructor.*/
    bst(bst&&) noexcept = default;

    /** Move assignment.*/
    bst& operator=(bst&&) noexcept = default;

    /** Inserts a new element into the container given a pair with key and value
     * (which are copied) if there is no element with the key in the container.
     * @param x The pair of key and value for the node that should be inserted.
     * @returns The pair of the iterator and the boolean flag (true if the node is inserted).
     */
    std::pair<iterator, bool> insert(const std::pair<key_type, value_type>& x);

    /** Inserts a new element into the container given a pair with key and value
     * (which are moved) if there is no element with the key in the container.
     * @param x The pair of key and value for the node that should be inserted.
     * @returns The pair of the iterator and the boolean flag (true if the node is inserted).
     */
    std::pair<iterator, bool> insert(std::pair<key_type, value_type>&& x);

    /** Inserts a new element into the container constructed in-place with
     * the given args if there is no element with the key in the container.
     * @tparam Types The types for the parameters.
     * @param args The pack corresponding to key and value to insert.
     * @returns The pair of the iterator and the boolean flag (true if the node is inserted).
     */
    template <class... Types>
    std::pair<iterator, bool> emplace(Types &&...args);

    /**Clear the content of the tree.*/
    void clear() noexcept;

    /** Unbalance the tree to the worst case. */
    void unbalance();

    /** Balance the tree (recursively).*/
    void balance();

    /** Returns a reference to the value that is mapped to a key equivalent to `x`,
     *  performing an insertion if such key does not already exist.
     * @param x The key.
     * @returns The reference to the value.
     */
    value_type& operator[](const key_type& x);

    /** Returns a reference to the value that is mapped to a key equivalent to `x`,
     *  performing an insertion if such key does not already exist.
     * @param x The key.
     * @returns The reference to the value.
     */
    value_type& operator[](key_type&& x);

    /** Function for finding a given key. If the key is present, it returns an
     * iterator to the proper node, end() otherwise.
     * @param x The key to find.
     * @returns The iterator pointing to the node.
     */
    iterator find(const key_type& x);

    /** Function for finding a given key. If the key is present, it returns a
     * constant iterator to the proper node, cend() otherwise.
     * @param x The key to find.
     * @returns The const iterator pointing to the node.
     */
    const_iterator find(const key_type& x) const;

    /** Overloading of the << operator. This function provides a view
     * of the tree, iterating over its nodes.
     * @param os Reference to output stream.
     * @param _bst The bst to be printed.
     * @returns The reference to the output stream.
     */
    template <typename value_type_, typename key_type_, typename OP_>
    friend std::ostream& operator<<(std::ostream& os, const bst<value_type_, key_type_, OP_>& _bst) noexcept;

    /** Removes the element (if one exists) with the key equivalent to key.
     * @param x The key of node to erase.
     */
    void erase(const key_type& x);

    /** Function for printing the structure of the tree into a string
     * which is returned by reference.
     * @param str_buffer The reference of the string where to write.
     * @returns The reference to the string where this function wrote.
     */
    const std::string& tree_structure_to_string(std::string& str_buffer) const;

    /** Given a pointer to the root of a subtree, this function returns
     * a std::string containing a view of the subtree.
     * @param subtree_root_node The pointer to the subtree root node
     * for which the structure will be print.
     * @param str_buffer The reference of the string where to write.
     * @returns The reference to the string where this function wrote.
     */
    static std::string& subtree_structure_to_string(const node* subtree_root_node,
        std::string& str_buffer);


private:

    /** Inserts a new element into the container given key and value (which
     * are forwarded) if there is no element with the key in the container.
     * @tparam K The type of the key.
     * @tparam V The type of the value.
     * @param k The key.
     * @param v The value.
     * @returns The pair of the iterator and the boolean flag (true if the node is inserted).
     */
    template <typename K, typename V>
    std::pair<iterator, bool> private_insert(K&& k, V&& v);

    /** Function for finding a key.
     * @param key_to_find The key of the node to find.
     * @returns The pointer to the node.
     */
    node* private_find(const key_type& key_to_find) const;

    /** Recursive function for balancing the tree.
     * Given a sorted array (in ascending order) of pointers to nodes in the tree,
     * this function takes the median (rounded down) element of the array
     * and inserts the corresponding element in the tree.
     * Then, this function recursively calls itself twice:
     * one time with the part of the array starting at its first element up to
     * the one previous the median (corresponding to the inserting element),
     * another time with the latter part of the array (median excluded).
     * @param start The starting index of the array (included).
     * @param stop The stop index of the array to be considered (excluded).
     * @param balanced The reference to the balanced tree which
     * to insert the median node as described above.
     * @param arr The array cointaining the pointers to the nodes of the tree.
    */
    void recursive_balance(const size_t start, const size_t stop, bst& balanced, node* const arr[]);

    /** Returns a reference to the value that is mapped to a key equivalent to `x`,
     *  performing an insertion if such key does not already exist.
     * @param x The key.
     * @returns The reference to the value.
     */
    template <typename key_type_>
    value_type& subscripting(key_type_&& x);
};

#include "Iterator.h"
#include "bst.cpp"

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H

