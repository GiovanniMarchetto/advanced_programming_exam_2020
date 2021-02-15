#include <iostream>
#include <utility>    // std::forward, move
#include <string>
#include <sstream>

#include "Node.h"
#include "Iterator.h"
#include "ap_error_ext.h"
#include "bst.h"

#ifndef ADVANCED_PROGRAMMING_EXAM_2020_BST_CPP
#define ADVANCED_PROGRAMMING_EXAM_2020_BST_CPP

template <typename value_type, typename key_type, typename OP>
Node<value_type, key_type, OP>* bst<value_type, key_type, OP>::
release_node(const Node<value_type, key_type, OP>* const node_to_release)
{
    AP_ERROR_IF_NULLPTR(node_to_release, "node_to_release", false);

    if (node_to_release->get_parent() == nullptr)
        return this->release_tree_root_node();

    if (node_to_release->get_parent()->get_left() == node_to_release)
        return node_to_release->get_parent()->release_left();

    return node_to_release->get_parent()->release_right();
}

template <typename value_type, typename key_type, typename OP>
Node<value_type, key_type, OP>* bst<value_type, key_type, OP>::
get_minimum_left_node_in_subtree(Node<value_type, key_type, OP>* node) noexcept
{
    while (node && node->get_left())
        node = node->get_left();

    return node;
}

template <typename value_type, typename key_type, typename OP>
bst<value_type, key_type, OP>::
bst(const bst& other) : size{ other.size }
{
    if (other.get_tree_root_node())
        set_tree_root_node(new node{ *(other.get_tree_root_node()) });
}

template <typename value_type, typename key_type, typename OP>
bst<value_type, key_type, OP>& bst<value_type, key_type, OP>::
operator=(const bst& other)
{
    auto tmp{ bst(other) };   // invoke the copy ctor
    *this = std::move(tmp); // move assignment
    return *this;
}

template <typename value_type, typename key_type, typename OP>
template <typename K, typename V>
std::pair<typename bst<value_type, key_type, OP>::iterator, bool> bst<value_type, key_type, OP>::
private_insert(K&& k, V&& v)
{
    AP_ERROR_IF_NULLPTR(k, "k", false);

    node* prev{ nullptr };
    node* curr = get_tree_root_node();

    while (curr)
    {
        prev = curr;
        if (node_key_compare(k, curr->key))
            curr = curr->get_left();
        else if (node_key_compare(curr->key, k))
            curr = curr->get_right();
        else        // key already present in the bst
            return std::make_pair<iterator, bool>(iterator{ prev }, false);
    }

    node* n = new node{ std::forward<K>(k), std::forward<V>(v) };
    n->set_parent(prev);

    if (!prev) // if the tree was empty
        set_tree_root_node(n);
    else if (*n < *prev)    // operator overloading of <
        prev->set_left(n);
    else
        prev->set_right(n);

    ++size;
    return std::make_pair<iterator, bool>(iterator{ n }, true);
}

template <typename value_type, typename key_type, typename OP>
inline std::pair<typename bst<value_type, key_type, OP>::iterator, bool> bst<value_type, key_type, OP>::
insert(const std::pair<key_type, value_type>& x)
{
    AP_ERROR_IF_NULLPTR(x.first, "x.first", true);
    return private_insert(x.first, x.second);
}

template <typename value_type, typename key_type, typename OP>
inline std::pair<typename bst<value_type, key_type, OP>::iterator, bool> bst<value_type, key_type, OP>::
insert(std::pair<key_type, value_type>&& x)
{
    AP_ERROR_IF_NULLPTR(x.first, "x.first", true);
    return private_insert(std::move(x.first), std::move(x.second));
}

template <typename value_type, typename key_type, typename OP>
template <class... Types>
inline std::pair<typename bst<value_type, key_type, OP>::iterator, bool> bst<value_type, key_type, OP>::
emplace(Types &&...args)
{
    return private_insert(std::forward<Types>(args)...);
}

template <typename value_type, typename key_type, typename OP>
inline void bst<value_type, key_type, OP>::
clear() noexcept
{
    set_tree_root_node();
    size = 0;
}

template <typename value_type, typename key_type, typename OP>
void bst<value_type, key_type, OP>::
unbalance()
{
    if (get_size())
    {
        bst tmp{};
        for (node& el : *this)
            tmp.emplace(std::move(el.key), std::move(el.value));
        *this = std::move(tmp);
    }
}

template <typename value_type, typename key_type, typename OP>
void bst<value_type, key_type, OP>::
recursive_balance(const size_t start, const size_t stop, bst<value_type, key_type, OP>& balanced, Node<value_type, key_type, OP>* const arr[])
{
    if (start != stop)
    {
        size_t mid{ (start + stop) / 2 };
        balanced.emplace(std::move(arr[mid]->key), std::move(arr[mid]->value));
        recursive_balance(start, mid, balanced, arr);
        recursive_balance(mid + 1, stop, balanced, arr);
    }
}

template <typename value_type, typename key_type, typename OP>
void bst<value_type, key_type, OP>::
balance()
{
    if (get_size())
    {
        bst tmp{};

        node* nodes[get_size()];
        size_t i{ 0 };
        for (auto& el : *this)
        {
            nodes[i] = &el;
            ++i;
        }
        recursive_balance(0, get_size(), tmp, nodes);
        *this = std::move(tmp);
    }
}

template <typename value_type, typename key_type, typename OP>
template <typename key_type_>
value_type& bst<value_type, key_type, OP>::
subscripting(key_type_&& x)
{
    AP_ERROR_IF_NULLPTR(x, "x", false);

    auto iter = find(std::forward<key_type_>(x));
    if (iter == end())
    {
        auto from_insert = emplace(std::forward<key_type_>(x), value_type{});
        return from_insert.first->value;   // from_insert.first is the iterator
    }
    return iter->value;
}

template <typename value_type, typename key_type, typename OP>
inline value_type& bst<value_type, key_type, OP>::
operator[](const key_type& x)
{
    AP_ERROR_IF_NULLPTR(x, "x", true);
    return subscripting(x);
}

template <typename value_type, typename key_type, typename OP>
inline value_type& bst<value_type, key_type, OP>::
operator[](key_type&& x)
{
    AP_ERROR_IF_NULLPTR(x, "x", true);
    return subscripting(std::move(x));
}

template <typename value_type, typename key_type, typename OP>
Node<value_type, key_type, OP>* bst<value_type, key_type, OP>::
private_find(const key_type& key_to_find) const
{
    AP_ERROR_IF_NULLPTR(key_to_find, "key_to_find", false);

    node* node{ get_tree_root_node() }; // search starts from the root

    while (node && key_to_find != node->key)
    {
        if (node_key_compare(key_to_find, node->key))
            node = node->get_left();
        else
            node = node->get_right();
    }

    return node;
}

template <typename value_type, typename key_type, typename OP>
typename bst<value_type, key_type, OP>::iterator bst<value_type, key_type, OP>::
find(const key_type& x)
{
    AP_ERROR_IF_NULLPTR(x, "x", true);

    node* node{ private_find(x) };
    if (node)
        return iterator{ node };
    return end();
}

template <typename value_type, typename key_type, typename OP>
typename bst<value_type, key_type, OP>::const_iterator bst<value_type, key_type, OP>::
find(const key_type& x) const
{
    AP_ERROR_IF_NULLPTR(x, "x", true);

    const node* node{ private_find(x) };
    if (node)
        return const_iterator{ node };
    return cend();
}

template <typename value_type, typename key_type, typename OP>
void bst<value_type, key_type, OP>::
erase(const key_type& x)
{
    AP_ERROR_IF_NULLPTR(x, "x", true);

    node* to_erase = private_find(x);

    if (to_erase == nullptr) // case 0: node to delete does not exist (wrong key provided)
        return;

    enum class to_erase_child_type { left, right, root };

    auto get_child_type = [this](node* child) {

        node* parent = child->get_parent();

        if (parent == nullptr) //                       if child hasn't parent node => child is the root
            return to_erase_child_type::root;

        else if (child == parent->get_left()) //        if child is the left child of its parent
            return to_erase_child_type::left;

        else //                                         if child is the right child of its parent
            return to_erase_child_type::right;
    };

    // Erases to_erase (and its children if it has any and they are managed smart pointers)
    auto transpose_subtree = [this](node* to_erase, to_erase_child_type ct, node* to_transplant) {
        node* parent = to_erase->get_parent();

        if (ct == to_erase_child_type::root) //        if to_erase is the root
            set_tree_root_node(to_transplant);

        else if (ct == to_erase_child_type::left) //   if to_erase is the left child of its parent
            parent->set_left(to_transplant);

        else //                                         if to_erase is the right child of its parent
            parent->set_right(to_transplant);

        if (to_transplant)
            to_transplant->set_parent(parent);
    };

    if (to_erase->get_left() == nullptr) // case 1: node to_erase has only the right child
        transpose_subtree(to_erase, get_child_type(to_erase), to_erase->release_right());

    else if (to_erase->get_right() == nullptr) // case 2: node to_erase has only the left child
        transpose_subtree(to_erase, get_child_type(to_erase), to_erase->release_left());

    else // case 3: node to_erase has both left and right children
    {
        node* min = get_minimum_left_node_in_subtree(to_erase->get_right());
        to_erase_child_type min_ct = get_child_type(min); // We need to know the type (of child) of the min node before releasing it 
        release_node(min);

        if (min->get_parent() != to_erase) // case 3ext: min is in the right subtree of node to_erase but is not its right child
        {
            transpose_subtree(min, min_ct, min->release_right());
            min->set_right(to_erase->release_right());
            min->get_right()->set_parent(min);
        }

        node* left_child_of_to_erase = to_erase->release_left(); // Save the left child because transpose_subtree is going to erase to_erase
        transpose_subtree(to_erase, get_child_type(to_erase), min);
        min->set_left(left_child_of_to_erase);
        min->get_left()->set_parent(min);
    }
    --size;
}

template <typename value_type, typename key_type, typename OP>
inline const std::string& bst<value_type, key_type, OP>::
tree_structure_to_string(std::string& str_buffer) const
{
    return subtree_structure_to_string(get_tree_root_node(), str_buffer);
}

template <typename value_type, typename key_type, typename OP>
std::string& bst<value_type, key_type, OP>::
subtree_structure_to_string(const Node<value_type, key_type, OP>* subtree_root_node,
    std::string& str_buffer)
{

    str_buffer.append("   "); // each level of the tree implies an indentation

    if (!subtree_root_node) // empty subtree
        return str_buffer.append("|--[]");

    std::string str_left_buff{ str_buffer },
        str_right_buff{ str_buffer };
    subtree_structure_to_string(subtree_root_node->get_left(), str_left_buff);
    subtree_structure_to_string(subtree_root_node->get_right(), str_right_buff);

    std::stringstream os{};
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


#endif //ADVANCED_PROGRAMMING_EXAM_2020_BST_CPP