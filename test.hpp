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
#include "binary_Search_tree.hpp"
#include "bplustree.hpp"
#include "btree.hpp"
#include "redblacktree.hpp"

void test_bst() {
  BSTree<int, int> bst;
  int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                 25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  for (int i = 0; i < 23; ++i) {
    bst.Insert(num[i], i);
  }
  std::shared_ptr<Node<int, int> > node = bst.Search(1);
  bst.Remove(1);
  bst.Display();
}

void test_avlt() {
  AVLTree<int, int> avlt;
  int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                 25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  for (int i = 0; i < 23; ++i) {
    avlt.Insert(num[i], i);
  }
  std::shared_ptr<Node<int, int> > node = avlt.Search(1);
  avlt.Remove(1);
  avlt.Display();
}

void test_btree() {
  BTree<int> btree(3);
  int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                 25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  for (int i = 0; i < 23; ++i) {
    btree.Insert(num[i]);
  }
  BTNode<int>* node = btree.Search(6);
  btree.Display();
  btree.Remove(6);
  btree.Remove(13);
  btree.Remove(7);
  btree.Remove(4);
  btree.Remove(2);
  btree.Remove(16);
  btree.Display();
}

void test_bptree() {
  BPTree<int, int> bptree(3);
  int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                 25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  for (int i = 0; i < 23; ++i) {
    bptree.Insert(num[i], i);
  }
  bptree.Remove(16);
  bptree.Remove(18);
  bptree.Remove(17);
  bptree.Display();
  bptree.Displaybylink();
}

void test_rbtree() {
  RBTree<int, int> rbtree;
  int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
                 25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  for (int i = 0; i < 23; ++i) {
    rbtree.Insert(num[i], i);
  }
  rbtree.Remove(16);
  rbtree.Remove(18);
  rbtree.Remove(17);
  rbtree.Display();
}