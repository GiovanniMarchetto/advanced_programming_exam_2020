#include <iostream>

#include <cstdlib> // srand
#include <ctime>   // time for setting the random seed

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

    //std::srand(123); // fixed seed for reproducible tests
    std::srand(std::time(NULL)); // random seed initialization

    tests();

    benchmark_test();

    std::cout << formatting_title() << std::endl;
    std::cout << formatting_title("### END OF PROGRAM ###", true) << std::endl;
    std::cout << formatting_title() << std::endl;

    return 0;
}