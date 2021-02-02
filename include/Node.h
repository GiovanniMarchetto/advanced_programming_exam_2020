#include <functional> // less
#include <stdint.h>   // int32_t
#include <memory>     // unique_ptr
#include <utility>    // pair

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_NODE_H
#define ADVANCED_PROGRAMMING_EXAM_2020_NODE_H

/** Class for a node in a Binary search tree.
 * @tparam value_type The type of the value of an instance of this class.
 * @tparam key_type The type of the key of an instance of this class.
 */
template <typename value_type,
          typename key_type = std::int32_t>
class Node
{
    /** Left child of this node.*/
    std::unique_ptr<Node> left;

    /** Right child of this node.*/
    std::unique_ptr<Node> right;

    /** Getter for the left children. Returns a raw pointer to the left child node.*/
    const Node* get_left() const { return left.get(); }

    /** Getter for the right children. Returns a raw pointer to the left child node.*/
    const Node* get_right() const { return right.get(); }

    /** Setter: the left child of this instance will be set with the given pointer.*/
    void set_left(Node* const node = nullptr) { left.reset(node); }

    /** Setter: the right child of this instance will be set with the given pointer.*/
    void set_right(Node* const node = nullptr) { right.reset(node); }

public:
    /** Key of the node.*/
    key_type key;

    /** Value of the node.*/
    value_type value;


    // ---START--- Compatibility with std::pair

    /** Constructor: creates a new instance of this class starting from an std::pair.*/
    Node(std::pair<key_type, value_type> pair) : Node{pair.first, pair.second} {};

    /** Returns a pointer to a (copy of the) representation of this instance as an std::pair.*/
    std::pair<key_type, value_type>* get_pair() const
    {
        return new std::pair(key_type{key}, value_type{value});
    }

    // ---END--- Compatibility with std::pair


    /** Constructor.
     * @param key Key of this node.
     * @param value Value of this node.*/
    Node(const key_type key, const value_type value) : left{}, right{}, key{key}, value{value} {}

    /** Default constructor.*/
    Node() = default;

    /** Default destructor.*/
    ~Node()
    {
        left.release();
        right.release();
    };

    /** Copy constructor (deep copy). The given node and all the children
     * linked by it are copied.*/
    explicit Node(const Node &node) : key{node.key}, value{node.value}
    {

        auto tmp_left = node.get_left();
        auto tmp_right = node.get_right();

        if ( tmp_left )
            set_left( new Node(*tmp_left) );

        if ( tmp_right )
            set_right( new Node(*tmp_right) );

    }

    /** Copy assignment. Deep copy is performed.*/
    Node &operator=(const Node &node)
    {
        set_left();
        set_right();

        auto tmp{Node(node)};
        *this = std::move(tmp);

        return *this;
    }

    /** Move constructor.*/
    Node(Node &&) = default;

    /** Move assignment.*/
    Node &operator=(Node &&) = default;

    /** Comparison function according to the given Comparator
     * applied to the keys.
     * @returns the result of the given comparator applied to the key
     * of this instance (as first argument) and the key of the given
     *  instance (as second argument).*/
    template<typename Comparator>
    bool compare(const Node& other, const Comparator comparator = std::less<key_type>()) const {
        return comparator(this->key, other.key);
    }
    /** See the same overloaded function for details. This comparison
     * function always uses std::less .*/
    bool compare(const Node& other) const {
        return compare(other, std::less<key_type>{});
    }

    /** Overload of the < operator to define an order relation
     * between two instances according to their key.
     * @returns true if the key of this instance is strictly less
     *          of the one given as parameter, false otherwise.*/
    bool operator<(const Node &other) const {
        return key < other.key;
    }
};

#endif //ADVANCED_PROGRAMMING_EXAM_2020_NODE_H
