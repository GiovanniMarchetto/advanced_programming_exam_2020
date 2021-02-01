#include "bst.h"

template <typename value_type, typename key_type, typename OP>
void bst<value_type, key_type, OP>::insert(const std::pair<const key_type, value_type> &x)
{
    auto n = new Node{x.first, x.second};

    //TODO : https://stackoverflow.com/q/495021
}