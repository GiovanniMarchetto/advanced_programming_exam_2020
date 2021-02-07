#include <iostream>
#include <utility>
#include <string>
#include <iostream> // TODO : remove useless includes
#include "Node.h"
#include "bst.h"
#include <vector>

#include "test.h"
#include "print_and_format.h"
#include "benchmark_test.h"


// For random generation
#include <cstdlib> // srand, rand
#include <ctime>   // time

// For benchmark timing test and comparison
#include <map>
#include <chrono>

int main()
{
    std::cout << formatting_title() << std::endl;
    std::cout << formatting_title("  ___                  ___  ", true) << std::endl;
    std::cout << formatting_title(" (o o)                (o o) ", true) << std::endl;
    std::cout << formatting_title("(  V  ) Hello World! (  V  )", true) << std::endl;
    std::cout << formatting_title("--m-m------------------m-m--", true) << std::endl;
    std::cout << formatting_title() << std::endl;

    tests();

    benchmark_test();

    return 0;
}