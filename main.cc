#include <iostream>
#include "binary_search_tree.hpp"
int main(){
    BSTNode<int, int> bst;
    bool flag = bst.insertElem(1,1);
    flag = bst.insertElem(2,2);
    flag = bst.insertElem(3,3);
    flag = bst.insertElem(4,4);
    return 0;
}