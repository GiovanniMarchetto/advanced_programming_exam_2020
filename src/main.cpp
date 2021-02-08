#include <iostream>

#include "test.h"
#include "print_and_format.h"
#include "benchmark_test.h"

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