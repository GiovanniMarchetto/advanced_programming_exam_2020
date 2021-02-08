#include <iostream>
#include <string>
#include <vector>
#include "bst.h"

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec);
void print_tree_from_iterator(bst<char>::iterator &iterator, bst<char> &bst);
void print_tree_from_iterator(bst<char>::const_iterator &iterator, const bst<char> &bst);

/** Simple utility function to properly format the titles of tests in an
 * uniform way.*/
std::string formatting_title(std::string title = "", bool center = false);