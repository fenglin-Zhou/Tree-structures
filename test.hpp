// MIT License

// Copyright (c) 2021 FenglinZhou

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>

#include "avl_tree.hpp"
#include "binary_search_tree.hpp"
#include "btree.hpp"

void test_bst() {
  BSTree<int, int> bst;
  bst.insertElem(2, 2);
  bst.insertElem(4, 4);
  bst.insertElem(7, 7);
  bst.insertElem(9, 9);
  bst.insertElem(5, 5);
  bst.insertElem(3, 3);
  bst.insertElem(1, 1);
  std::shared_ptr<Node<int, int> > node = bst.searchElem(1);
  bst.deleteElem(1);
  bst.display();
}

void test_avlt() {
  AVLTree<int, int> avlt;
  avlt.insertElem(2, 2);
  avlt.insertElem(4, 4);
  avlt.insertElem(7, 7);
  avlt.insertElem(9, 9);
  avlt.insertElem(5, 5);
  avlt.insertElem(3, 3);
  avlt.insertElem(1, 1);
  std::shared_ptr<Node<int, int> > node = avlt.searchElem(1);
  avlt.deleteElem(1);
  avlt.display();
}

void test_btree() {
  BTree<int> btree(3);
  btree.insert(1);
  btree.insert(2);
  btree.insert(3);
  btree.insert(4);
  btree.insert(5);
  btree.insert(6);
  btree.insert(7);
  btree.insert(8);
  btree.insert(9);
  BTNode<int>* node = btree.search(6);
  btree.traverse();
}