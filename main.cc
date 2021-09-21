#include <iostream>
#include "binary_search_tree.hpp"

void test_bst() {
    BSTNode<int, int> bst;
    bst.insertElem(2,2);
    bst.insertElem(4,4);
    bst.insertElem(3,3);
    bst.insertElem(1,1);
    std::shared_ptr<Node<int, int> > node= bst.searchElem(1);
    bst.deleteElem(1);
    bst.display();
}

int main(){
    test_bst();
    return 0;
}