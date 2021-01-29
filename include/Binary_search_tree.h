#ifndef ADVANCED_PROGRAMMING_EXAM_2020_BINARY_SEARCH_TREE_H
#define ADVANCED_PROGRAMMING_EXAM_2020_BINARY_SEARCH_TREE_H

/**
 * Class for the binary search tree (BST).
 * The BST is sorted according to the keys.
 * @tparam value_type Type of the value of each node of the tree.
 * @tparam key_type Type of the key of each node of the tree (not
 *               required to be specified).
 * @tparam OP The type of the comparison operator, which is used
 *            to compare two keys (not required to be specified).
 */
template<typename value_type,
         typename key_type = std::int32_t,
         typename OP = std::less <key_type>>
class Binary_search_tree {

    /** Class for a node in the Binary search tree.*/
    class Node {

        /** Key of the node.*/
        key_type key;

        /** Value of the node.*/
        value_type value;

        /** Left child of this node.*/
        std::unique_ptr<Node> left;

        /** Right child of this node.*/
        std::unique_ptr<Node> right;

    public:
        /** Create a new node.
         * @param key Key of this node.
         * @param value Value of this node.*/
        Node(const key_type key, const value_type value);

        /** Given a value, a new node with that value is created.
         * The key for the node will be automatically generated.
         * @param value Value of this node.*/
        Node(const value_type value);

        /** Copy constructor.*/
        Node(const Node &node);

        /** Move constructor.*/
        Node(Node &&node);

        /** Default constructor.*/
        Node() = default;

        /** Default destructor.*/
        ~Node() = default;

    };

    /** Pointer to the root node of the tree.*/
    std::unique_ptr<Node> rootNode;

    /** Number of nodes in the tree.*/
    size_t size;


public:

    /** Copy constructor.*/
    Binary_search_tree(const Binary_search_tree &binarySearchTree);

    /** Move constructor.*/
    Binary_search_tree(Binary_search_tree &&binarySearchTree);

    /** Default constructor.*/
    Binary_search_tree() = default;

    /** Default destructor for the proper cleanup.*/
    ~Binary_search_tree() = default;

    /** Insert. Function for inserting a node. The function returns a pair of
     * an iterator (pointing to the node) and a bool. The bool is true
     * if a new node has been allocated, false otherwise (i.e., the key
     * was already present in the tree).
     * @param x The pair (key, value).*/
    std::pair<iterator, bool> insert(const std::pair<const key_type, value_type> x);
    /** Insert. Function for inserting a node. The function returns a pair of
     * an iterator (pointing to the node) and a bool. The bool is true
     * if a new node has been allocated, false otherwise (i.e., the key
     * was already present in the tree).
     * @param x The pair (key, value).*/
    std::pair<iterator, bool> insert(std::pair<const key_type, value_type>&& x);

    /** Emplace. Inserts a new element into the container constructed in-place with
     * the given args if there is no element with the key in the container.*/
    template< class... Types >
    std::pair<iterator,bool> emplace(Types&&... args);

    /** Clear the content of the tree.*/
    void clear();

    /** Begin. Return an iterator to the left-most node (which, likely,
     * is not the root node).*/
    iterator begin();
    /** Begin. Return an iterator to the left-most node (which, likely,
     * is not the root node).*/
    const_iterator begin() const;
    /** Begin. Return an iterator to the left-most node (which, likely,
     * is not the root node).*/
    const_iterator cbegin() const;

    /** End. Return an iterator to one-past the last element.*/
    iterator end();
    /** End. Return an iterator to one-past the last element.*/
    const_iterator end() const;
    /** End. Return an iterator to one-past the last element.*/
    const_iterator cend() const;

    /** Find a given key. If the key is present, returns
     * an iterator to the proper node, end() otherwise.*/
    iterator find(const key_type& x);
    /** Find a given key. If the key is present, returns
     * an iterator to the proper node, end() otherwise.*/
    const_iterator find(const key_type& x) const;

    /** Balance the tree.*/
    void balance();

    /** Subscripting operator.
     * Returns a reference to the value that is mapped to
     * a key equivalent to x, performing an insertion if such
     * key does not already exist.*/
    value_type& operator[](const key_type& x);
    /** Subscripting operator.
     * Returns a reference to the value that is mapped to
     * a key equivalent to x, performing an insertion if such
     * key does not already exist.*/
    value_type& operator[](key_type&& x);

    /** Put-to operator.
     * Implement the friend function inside the class, such
     * that you do not have to specify the templates for bst.*/
    friend
    std::ostream& operator<<(std::ostream& os, const bst& x);

    /** Erase.
     * Removes the element (if one exists) with the key equivalent to key.*/
    void erase(const key_type& x);

};


#endif //ADVANCED_PROGRAMMING_EXAM_2020_BINARY_SEARCH_TREE_H
