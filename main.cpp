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

    Node<int,std::string> F("rr",5);
    // std::cout << (F < D) << std::endl;  // expected: exception   // TODO : test well comparison functions

    std::cout << C.compare(B) << std::endl;  // expected: 0

    std::cout << B.compare(C) << std::endl;  // expected: 1
    std::cout << C.compare(B) << std::endl;  // expected: 0
    


    // Insertion of a node
    bst<char> bst_{};
    bst_.insert(std::pair<int, char>(2,'y'));
    bst_.insert(std::pair<int, char>(0,'z'));
    bst_.insert(std::pair<int, char>(1,'x'));
    bst_.insert(std::pair<int, char>(3,'w'));
    bst_.insert(std::pair<int, char>(4,'c'));

    std::cout << bst_ << std::endl;

    return 0;

}