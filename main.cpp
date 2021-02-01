#include <iostream>
#include <utility>
#include <string>
#include <iostream>
#include "Node.h"
#include "bst.h"

int main()
{
    std::cout << "Hello World!" << std::endl;

    Node<char> D(4,'D');
    Node<char> C(3,'C');
    Node<char> B(2,'B');
    Node<char> A(1,'A');

    Node<char> A_copia{A};
    Node<char> C_copia{C};

    D = A;

    std::cout << (B < C) << std::endl;  // expected: 1
    std::cout << (C < B) << std::endl;  // expected: 0

    return 0;

}