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

#include "avl_tree.h"
#include "binary_search_tree.h"
#include "bplustree.h"
#include "btree.h"
#include "redblacktree.h"

void Test_BST() {
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

void Test_AVLT() {
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

void Test_BTree() {
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

void Test_BPTree() {
  // BPTree<int, int> bptree(3);
  // int num[23] = {1,  3,  7,  10, 11, 13, 14, 15, 18, 16, 19, 24,
  //                25, 26, 21, 4,  5,  20, 22, 2,  17, 12, 6};
  // for (int i = 0; i < 23; ++i) {
  //   bptree.Insert(num[i], i);
  // }
  // bptree.Remove(16);
  // bptree.Remove(18);
  // bptree.Remove(17);
  // bptree.Display();
  // bptree.Displaybylink();
  BPTree<int, int> bpt(2);
  for (int i = 0; i < 10; ++i) {
    bpt.Insert(i * 2, i);
  }
  bpt.Insert(-1, -1);
  bpt.Insert(1, 1);
  bpt.Display();
  bpt.Remove(6);
  bpt.Display();
  bpt.Displaybylink();
}

void Test_RBTree() {
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