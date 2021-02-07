#include <iostream>
#include <utility>
#include <string>
#include "Node.h"
#include "bst.h"
#include <vector>
#include <regex> // regex used for substring replacement

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec); // TODO : to be deleted ...
void print_tree_from_iterator(bst<char>::iterator &iterator, bst<char> &bst);
void print_tree_from_iterator(bst<char>::const_iterator &iterator, const bst<char> &bst);

/** Simple utitilty function to properly format the titles of tests in an
 * uniform way.*/
std::string formatting_title(std::string title = "", bool center = false);