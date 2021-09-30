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

#ifndef _BTREE_H_
#define _BTREE_H_

#include <iostream>
#include <vector>

template <typename T>
class BTree;

template <typename T>
class BTNode {
  // kyes
  T* keys_;
  // minimum degreee
  int degree_;
  // child pointers
  BTNode<T>** child_;
  // number of keys
  int keycount_;
  // Is true when node is leaf
  bool isleaf_;

 public:
  BTNode(int degree, bool leaf);
  void insertnonfull(T key);
  void splitchild(int index, BTNode* node);
  void traverse();
  BTNode<T>* search(T key);

  friend class BTree<T>;
};

template <typename T>
class BTree {
  BTNode<T>* root_;
  int degree_;

 public:
  BTree(int degree) : root_(nullptr), degree_(degree) {}
  void traverse() {
    if (root_ != nullptr) {
      std::cout << "------- Display BTree ----------\n";
      root_->traverse();
    }
  }
  BTNode<T>* search(T key) {
    return (root_ != nullptr) ? root_->search(key) : nullptr;
  }

  void insert(T key);
};

template <typename T>
BTNode<T>::BTNode(int degree, bool leaf) {
  degree_ = degree;
  isleaf_ = leaf;
  keys_ = new T[2 * degree_ - 1];
  child_ = new BTNode*[2 * degree_];
  keycount_ = 0;
}

template <typename T>
void BTNode<T>::traverse() {
  int i = 0;
  for (i = 0; i < keycount_; ++i) {
    if (isleaf_ == false) {
      child_[i]->traverse();
    }
    std::cout << " " << keys_[i];
  }
  std::cout << std::endl;
  // the last child_, You can also modify the for loop implement it
  if (isleaf_ == false) {
    child_[i]->traverse();
  }
}
template <typename T>
void BTNode<T>::insertnonfull(T key) {
  int i = keycount_ - 1;
  if (isleaf_ == true) {
    // find the location of new key to be inserted
    // Move all the larger keys to one position behind
    while (i >= 0 && keys_[i] > key) {
      keys_[i + 1] = keys_[i];
      --i;
    }
    // insert the new key at found lacation
    keys_[i + 1] = key;
    keycount_++;
  } else {
    while (i >= 0 && keys_[i] > key) {
      --i;
    }
    if (child_[i + 1]->keycount_ == 2 * degree_ - 1) {
      splitchild(i + 1, child_[i + 1]);
      if (keys_[i + 1] < key) {
        i++;
      }
    }
    child_[i + 1]->insertnonfull(key);
  }
}

template <typename T>
void BTNode<T>::splitchild(int index, BTNode* node) {
  // create a new node which is going to store last (degree-1) keys of node
  // node->kyes_ 0, degree-1, degree, 2degree-1
  //             | - - - - - |  -  | - - - - - |
  BTNode<T>* temp = new BTNode<T>(node->degree_, node->isleaf_);
  temp->keycount_ = degree_ - 1;
  // copy the last degree - 1 keys of node to temp
  for (int i = 0; i < degree_ - 1; ++i) {
    temp->keys_[i] = node->keys_[i + degree_];
  }
  // copy the last degree children of node to temp
  if (node->isleaf_ == false) {
    for (int i = 0; i < degree_; ++i) {
      temp->child_[i] = node->child_[i + degree_];
    }
  }
  // reduce the keycount
  node->keycount_ = degree_ - 1;
  // Move all the larger keys to one position behind
  for (int i = keycount_; i >= index + 1; --i) {
    child_[i + 1] = child_[i];
  }
  // Insert the new child in the reserved position
  child_[index + 1] = temp;
  // Move all the larger keys to one position behind
  for (int i = keycount_ - 1; i >= index; --i) {
    keys_[i + 1] = keys_[i];
  }
  // copy the middle key of node to this node
  keys_[index] = node->keys_[degree_ - 1];
  // increment keycount
  keycount_ = keycount_ + 1;
}

template <typename T>
BTNode<T>* BTNode<T>::search(T key) {
  int i = 0;
  while (i < keycount_ && key > keys_[i]) {
    ++i;
  }
  if (key == keys_[i]) {
    std::cout << "find it! key: " << key << std::endl;
    return this;
  }
  if (isleaf_ == true) {
    std::cout << "not find " << key << std::endl;
    return nullptr;
  }
  return child_[i]->search(key);
}

template <typename T>
void BTree<T>::insert(T key) {
  if (root_ == nullptr) {
    root_ = new BTNode<T>(degree_, true);
    root_->keys_[0] = key;
    root_->keycount_ = 1;
  } else {
    if (root_->keycount_ == 2 * degree_ - 1) {
      BTNode<T>* temp = new BTNode<T>(degree_, false);
      temp->child_[0] = root_;
      temp->splitchild(0, root_);
      int i = 0;
      if (temp->keys_[0] < key) {
        i++;
      }
      temp->child_[i]->insertnonfull(key);
      root_ = temp;
    } else {
      root_->insertnonfull(key);
    }
  }
}

#endif