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
    using node = Node<value_type, key_type, OP>;

    /** Pointer to the root node of the tree.*/
    std::unique_ptr<node> tree_root_node;

    /** Number of nodes in the tree.*/
    std::size_t size;

    /** Getter for the root node. Returns a raw pointer.*/
    node* get_tree_root_node() const noexcept { return tree_root_node.get(); }

    /** Release the root node. Returns a raw pointer and releases the ownership.*/
    node* release_tree_root_node() noexcept { return tree_root_node.release(); }

    /** Setter: the root node of this instance will be set with the given raw pointer.*/
    void set_tree_root_node(node* const node = nullptr) noexcept { tree_root_node.reset(node); }

    /** Releases the given node from its parent or from the tree if it's the root node.
     * Warning: it returns an unmanaged raw pointer to the released node. */
    node* release_node(const node* const node_to_release);

    /** Given a pointer to a node, this function return the leftmost
     * (minimum) node in the subtree whose root is the specified node.*/
    static node* get_minimum_left_node_in_subtree(node* node) noexcept;

public:
    /** Function object that compares nodes by comparing of the
     * key components of the nodes.*/
    OP node_key_compare{};

    /** Getter for the number of nodes in the tree. */
    size_t get_size() const noexcept { return size; }

    /** Class Iterator for the tree.*/
    template <typename O>
    class Iterator;
    using iterator = Iterator<node>;
    using const_iterator = Iterator<const node>;

    /** Begin function to use in a for-range loop. Returns an iterator.*/
    iterator begin() noexcept { return iterator{ get_minimum_left_node_in_subtree(get_tree_root_node()) }; }
    /** Cbegin function to use in a for-range loop. Returns a const iterator.*/
    const_iterator cbegin() const noexcept { return const_iterator{ get_minimum_left_node_in_subtree(get_tree_root_node()) }; }
    /** Begin function to use in a for-range loop. Returns a const iterator.*/
    const_iterator begin() const noexcept { return cbegin(); }

    /** End function to use in a for-range loop. Returns an iterator.*/
    iterator end() noexcept { return iterator{ nullptr }; }
    /** Cend function to use in a for-range loop. Returns a const iterator.*/
    const_iterator cend() const noexcept { return const_iterator{ nullptr }; }
    /** End function to use in a for-range loop. Returns a const iterator.*/
    const_iterator end() const noexcept { return cend(); }

    /** Default constructor.*/
    bst() = default;

    /** Default destructor for the proper cleanup.*/
    ~bst() noexcept = default;

    /** Copy constructor.*/
    bst(const bst& other) : size{ other.size }
    {
        if (other.get_tree_root_node())
            set_tree_root_node(new node{ *(other.get_tree_root_node()) });
    }

    /** Copy assignment.*/
    bst& operator=(const bst& other);

    /** Move constructor.*/
    bst(bst&&) noexcept = default;

    /** Move assignment.*/
    bst& operator=(bst&&) noexcept = default;

    /** Inserts a new element into the container given a pair with key and value
     * (which are copied) if there is no element with the key in the container. */
    std::pair<iterator, bool> insert(const std::pair<key_type, value_type>& x);

    /** Inserts a new element into the container given a pair with key and value
     * (which are moved) if there is no element with the key in the container. */
    std::pair<iterator, bool> insert(std::pair<key_type, value_type>&& x);

    /** Inserts a new element into the container constructed in-place with
     * the given args if there is no element with the key in the container.*/
    template <class... Types>
    std::pair<iterator, bool> emplace(Types &&...args);

    /**Clear the content of the tree.*/
    void clear() noexcept;

    /** Unbalance the tree to the worst case. */
    void unbalance();


    /** Balance the tree (recursively). */
    void balance();


    /** Returns a reference to the value that is mapped to a key equivalent to `x`,
     *  performing an insertion if such key does not already exist. */
    value_type& operator[](const key_type& x);

    /** Returns a reference to the value that is mapped to a key equivalent to `x`,
     *  performing an insertion if such key does not already exist. */
    value_type& operator[](key_type&& x);

    /** Function for finding a given key. If the key is present, it returns an
     * iterator to the proper node, end() otherwise.
     * @param x The key.*/
    iterator find(const key_type& x);

    /** Function for finding a given key. If the key is present, it returns a
     * constant iterator to the proper node, cend() otherwise.
     * @param x The key.*/
    const_iterator find(const key_type& x) const;

    /** Overloading of the << operator. This function provides a view
     * of the tree, iterating over its nodes.*/
    template <typename value_type_, typename key_type_, typename OP_>
    friend std::ostream& operator<<(std::ostream&, const bst<value_type_, key_type_, OP_>& _bst) noexcept;

    /** Removes the element (if one exists) with the key equivalent to key.*/
    void erase(const key_type& x);

    /** Function for printing the structure of the tree into a string
     * which is returned by reference.*/
    const std::string& tree_structure_to_string(std::string& str_buffer) const;

    /** Given a pointer to the root of a subtree, this function returns
     * a std::string containing a view of the subtree.*/
    static std::string& subtree_structure_to_string(const node* subtree_root_node,
        std::string& str_buffer);


private:

    /** Inserts a new element into the container given key and value (which
         * are forwarded) if there is no element with the key in the container.
         * @tparam K The type of the key.
         * @tparam V The type of the value.*/
    template <typename K, typename V>
    std::pair<iterator, bool> private_insert(K&& k, V&& v);

    /** Private function for finding a key.
     *  Returns a pointer to the node */
    node* private_find(const key_type& key_to_find) const;

    /** Balance recursive function.
     * TODO start included, stop excluded (documentazione)
    */
    void recursive_balance(const size_t start, const size_t stop, bst& balanced, node* const arr[]);

    template <typename key_type_>
    value_type& subscripting(key_type_&& x);
};

#include "Iterator.h"
#include "bst.cpp"

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H

