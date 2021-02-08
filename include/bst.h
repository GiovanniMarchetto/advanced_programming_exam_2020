#include <functional> // std::less
#include <stdint.h>   // int32_t
#include <memory>     // unique_ptr
#include <utility>    //std::forward, move
#include "Node.h"

#include <string>
#include <sstream>

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
    node *get_tree_root_node() const { return tree_root_node.get(); }

    /** Release the root node. Returns a raw pointer and releases the ownership.*/
    node *release_tree_root_node() { return tree_root_node.release(); }

    /** Setter: the root node of this instance will be set with the given raw pointer.*/
    void set_tree_root_node(node *const node = nullptr) { tree_root_node.reset(node); }

    /** Releases the given node from its parent 
     * or from the tree if it's the root node. 
     * Warning: it returns an unmanaged raw pointer 
     * to the released node. */
    node *release_node(node *node_to_release)
    {
        if (node_to_release->get_parent() == nullptr)
            return this->release_tree_root_node();

        if (node_to_release->get_parent()->get_left() == node_to_release)
            return node_to_release->get_parent()->release_left();

        return node_to_release->get_parent()->release_right();
    }

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
    OP node_key_compare{};

    /** Getter for the number of nodes in the tree. */
    size_t get_size() const { return size; }

    /** Class Iterator for the tree.*/
    template <typename O>
    class Iterator;
    using iterator = Iterator<node>;
    using const_iterator = Iterator<const node>;

    /** Begin function to use in a for-range loop. Returns an iterator.*/
    iterator begin() noexcept { return iterator{get_minimum_left_node_in_subtree(get_tree_root_node())}; }
    // TODO: mantenere? --  the iterator does not acquire resource, so noexcept (nothing will go wrong)
    /** Cbegin function to use in a for-range loop. Returns a const iterator.*/
    const_iterator cbegin() const noexcept { return const_iterator{get_minimum_left_node_in_subtree(get_tree_root_node())}; }
    // TODO: mantenere? -- const-iterator ensure to be protected when invoking a const function
    /** Begin function to use in a for-range loop. Returns a const iterator.*/
    const_iterator begin() const noexcept { return cbegin(); }

    //TODO: riorganization code for cbegin and cend?

    /** End function to use in a for-range loop. Returns an iterator.*/
    iterator end() noexcept { return iterator{nullptr}; }
    /** Cend function to use in a for-range loop. Returns a const iterator.*/
    const_iterator cend() const noexcept { return const_iterator{nullptr}; };
    /** End function to use in a for-range loop. Returns a const iterator.*/
    const_iterator end() const noexcept { return cend(); }

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

    /** Inserts a new element into the container given key and value (which
     * are forwarded) if there is no element with the key in the container. */
    template <typename K, typename V>
    std::pair<iterator, bool> private_insert(K &&k, V &&v) //TODO: DOVREBBE ESSERE PRIVATO, insert o emplace?
    {
        using Node = bst<value_type, key_type, OP>::node; // TODO: inutile per ora, se si sposta implementazione su altro file diventa utile

        Node *prev{nullptr};
        Node *curr = get_tree_root_node();

        while (curr)
        {
            prev = curr;
            if (k < curr->key)
                curr = curr->get_left();
            else if (k > curr->key)
                curr = curr->get_right();
            else
            {
                return std::make_pair<iterator, bool>(iterator{prev}, false);
            }
        }

        Node *node = new Node{std::forward<K>(k), std::forward<V>(v)};
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

    /** Inserts a new element into the container given a pair with key and value
     * (which are copied) if there is no element with the key in the container. */
    std::pair<iterator, bool> insert(const std::pair<key_type, value_type> &x) // TODO : noexcept ??
    {
        return private_insert(x.first, x.second);
    }

    /** Inserts a new element into the container given a pair with key and value
     * (which are moved) if there is no element with the key in the container. */
    std::pair<iterator, bool> insert(std::pair<key_type, value_type> &&x)
    {
        return private_insert(std::move(x.first), std::move(x.second));
    }

    /** Inserts a new element into the container constructed in-place with 
     * the given args if there is no element with the key in the container. */
    template <class... Types>
    std::pair<iterator, bool> emplace(Types &&...args)
    {
        return private_insert(std::forward<Types>(args)...);
    }

    /**Clear the content of the tree.*/
    void clear()
    {
        set_tree_root_node();
        size = 0;
    }

    /** Unbalance the tree to the worst case. */
    void unbalance()
    {
        if (get_size())
        {
            bst tmp{};
            for (node &el : *this)
                tmp.emplace(std::move(el.key), std::move(el.value));
            *this = std::move(tmp);
        }
    }

    /** Balance recursive function. */
    void recursive_balance(size_t start, size_t stop, bst &balanced, node *arr[]) //TODO: DOVREBBE ESSERE PRIVATO
    {
        if (start != stop)
        {
            size_t mid{(start + stop) / 2};
            balanced.emplace(std::move(arr[mid]->key), std::move(arr[mid]->value));
            recursive_balance(start, mid, balanced, arr);
            recursive_balance(mid + 1, stop, balanced, arr);
        }
    }

    /** Balance the tree (recursively). */
    void balance()
    {
        if (get_size())
        {
            bst tmp{};

            node *nodes[get_size()];
            size_t i{0};
            for (auto &el : *this)
            {
                nodes[i] = &el;
                ++i;
            }
            recursive_balance(0, get_size(), tmp, nodes);
            *this = std::move(tmp);
        }
    }

    template <typename key_type_>
    value_type &subscripting(key_type_ &&x)
    {
        auto iter = find(std::forward<key_type_>(x));
        if (iter == end())
        {
            auto from_insert = insert({std::forward<key_type_>(x), value_type{}});
            return from_insert.first->value;
        }
        return iter->value;
    }

    /** Returns a reference to the value that is mapped to a key equivalent to `x`, 
     *  performing an insertion if such key does not already exist. */
    value_type &operator[](const key_type &x) { return subscripting(x); }

    /** Returns a reference to the value that is mapped to a key equivalent to `x`, 
     *  performing an insertion if such key does not already exist. */
    value_type &operator[](key_type &&x) { return subscripting(std::move(x)); }

    /** Private function for finding a key.
     *  Returns a pointer to the node */
    node *private_find(const key_type &key_to_find) const // TODO : move this in private part
    {
        node *node{get_tree_root_node()}; // search starts from the root

        while (node && key_to_find != node->key)
        {
            if (node_key_compare(key_to_find, node->key))
                node = node->get_left();
            else
                node = node->get_right();
        }

        return node;
    }

    /** Function for finding a given key. If the key is present, it returns an 
     * iterator to the proper node, end() otherwise.
     * @param x The key.*/
    iterator find(const key_type &x)
    {
        node *node{private_find(x)};
        if (node)
            return iterator{node};
        return end();
    }

    /** Function for finding a given key. If the key is present, it returns a 
     * costant iterator to the proper node, cend() otherwise.
     * @param x The key.*/
    const_iterator find(const key_type &x) const
    {
        const node *node{private_find(x)};
        if (node)
            return const_iterator{node};
        return cend();
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

    /** Removes the element (if one exists) with the key equivalent to key.*/
    void erase(const key_type &x)
    {
        node *to_erase = private_find(x);

        if (to_erase == nullptr) // case 0: node to delete does not exist (wrong key provided)
            return;

        // Erases to_remove (and its children if it has any and they are managed smart pointers)
        auto transpose_subtree = [this](node *to_remove, node *to_transplant) {
            node *parent = to_remove->get_parent();

            if (parent == nullptr) //                       if to_remove hasn't parent node => to_remove is the root
                set_tree_root_node(to_transplant);

            else if (to_remove == parent->get_left()) //    if to_remove is the left child of its parent
                parent->set_left(to_transplant);

            else if (to_remove == parent->get_right()) //   if to_remove is the right child of its parent
                parent->set_right(to_transplant);

            if (to_transplant)
                to_transplant->set_parent(parent);
        };

        if (to_erase->get_left() == nullptr) // case 1: node to_erase has only the right child
            transpose_subtree(to_erase, to_erase->release_right());

        else if (to_erase->get_right() == nullptr) // case 2: node to_erase has only the left child
            transpose_subtree(to_erase, to_erase->release_left());

        else // case 3: node to_erase has both left and right children
        {
            node *min = release_node(get_minimum_left_node_in_subtree(to_erase->get_right()));

            if (min->get_parent() != to_erase) // case 3ext: min is in the right subtree of node to_erase but is not its right child
            {
                transpose_subtree(min, min->release_right());
                min->set_right(to_erase->release_right());
                min->get_right()->set_parent(min);
            }

            node *left_child_of_to_erase = to_erase->release_left(); // Save the left child because transpose_subtree is going to erase to_erase
            transpose_subtree(to_erase, min);
            min->set_left(left_child_of_to_erase);
            min->get_left()->set_parent(min);
        }
        --size;
    }

    /** Function for printing the structrure of the tree into a string
     * which is returned by reference.*/
    const std::string &print_tree(std::string &str_buffer) const
    {
        return print_subtree(tree_root_node.get(), str_buffer); // TODO : this should use getter instead of std::unique_ptr<??>::get
    }

    /** Given a pointer to the root of a subtree, this function returns
     * a std::string containing a view of the subtree.*/
    template <typename value_type_, typename key_type_, typename OP_>
    static std::string &
    print_subtree(const Node<value_type_, key_type_, OP_> *subtree_root_node,
                  std::string &str_buffer)
    {

        str_buffer.append("   "); // each level of the tree implies an indentation

        // TODO : to be optimized (does std::string use move-ctr? Better to use char*? Should use string initialization std::string{} instead of literal "")
        if (!subtree_root_node) // empty subtree
            return str_buffer.append("|--[]");

        std::string str_left_buff{str_buffer},
            str_right_buff{str_buffer};
        print_subtree(subtree_root_node->get_left(), str_left_buff);
        print_subtree(subtree_root_node->get_right(), str_right_buff);

        std::stringstream os{};            // TODO : improve efficiency (this is used here only for using the overloading of << in Node)
        os << "|--" << *subtree_root_node; //save info of current node into the stream

        // Buffer to return
        str_buffer.append(os.str()); // append info of current node
        if (subtree_root_node->get_left() ||
            subtree_root_node->get_right()) // if at least one child is defined, then print
        {
            str_buffer.append("\n")
                .append(str_left_buff) // append info of left child node
                .append("\n")
                .append(str_right_buff); // append info of right child node
        }

        return str_buffer;
    }
};

#include "Iterator.h"
#include "bst.cpp" // TODO : https://stackoverflow.com/q/495021

#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_H
