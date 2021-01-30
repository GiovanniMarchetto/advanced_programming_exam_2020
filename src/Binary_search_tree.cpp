#include "../include/Binary_search_tree.h"

Binary_search_tree::Node {

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

    /** Copy assignment (deep copy).*/
    Node& operator = (const Node&);

    /** Move assignment  (shadow copy).*/
    Node& operator = (Node&&);

    /** Default constructor.*/
    Binary_search_tree() = default;

    /** Default destructor for the proper cleanup.*/
    ~Binary_search_tree() = default;

    /** Default constructor.*/
    Node() = default;

    /** Default destructor.*/
    ~Node() = default;

};