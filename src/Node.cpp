#include "bst.h"

template <typename value_type, typename key_type, typename OP>
inline bst<value_type, key_type, OP>::Node::Node(const key_type key, const value_type value) : key{key}, value{value} {}

template <typename value_type, typename key_type, typename OP>
inline bool bst<value_type, key_type, OP>::Node::operator<(const typename bst<value_type, key_type, OP>::Node &other) const
{
    // TODO: move op construction elsewhere to avoid its allocation every time this operator is called
    OP op{};
    return op(key, other.key);
}
