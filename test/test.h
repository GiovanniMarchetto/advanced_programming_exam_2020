#include <iostream>
#include <utility>
#include <string>
#include "Node.h"
#include "bst.h"
#include <vector>
#include <regex> // regex used for substring replacement

template <typename F>
void iteration_func(F &function);

void node_operations_test(int i);
void node_other_type_test();
void find_node_test(int i);
void bst_insertion_test(int i);
void subscripting_node_test(int i);
void erase_test(int i);
void tests();