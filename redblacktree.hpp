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

#ifndef _REDBLACKTREE_H_
#define _REDBLACKTREE_H_

#include <iostream>

template <typename K, typename V>
class RBTree;

enum RBTcolor { RED, BLACK };

template <typename K, typename V>
class RBTNode {
 private:
  RBTcolor color_;
  K key_;
  V value_;
  RBTNode<K, V>* parent_;
  RBTNode<K, V>* left_;
  RBTNode<K, V>* right_;

  RBTNode<K, V>* findmin();
  RBTNode<K, V>* findmax();
  void insert(RBTNode<K, V>*);

 public:
  explicit RBTNode(K, V, RBTcolor, RBTNode<K, V>* parent, RBTNode<K, V>* left,
                   RBTNode<K, V>* right);
  ~RBTNode();
  void remove(K);
  RBTNode<K, V>* search(K);

  friend class RBTree<K, V>;
};

template <typename K, typename V>
class RBTree {
 private:
  RBTNode<K, V>* root_;
  void leftrotate(RBTNode<K, V>*& node);
  void rightrotate(RBTNode<K, V>*& node);

 public:
  RBTree();
  RBTree(K, V);
  ~RBTree();
  void remove(K);
  void insert(K, V);
  void display();
  RBTNode<K, V>* search(K);
};

template <typename K, typename V>
RBTNode<K, V>::RBTNode(K key, V value, RBTcolor color, RBTNode<K, V>* parent,
                       RBTNode<K, V>* left = nullptr,
                       RBTNode<K, V>* right = nullptr)
    : key_(key),
      value_(value),
      color_(color),
      parent_(parent),
      left_(left),
      right_(right) {}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::search(K key) {
  if (key_ == key) {
    return this;
  } else if (key_ < key && right_ != nullptr) {
    return right_->search(key);
  } else if (key < key_ && left_ != nullptr) {
    return left_->search(key);
  } else {
    std::cout << "Not found!\n";
    return nullptr;
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::findmin() {
  RBTNode<K, V>* temp = this;
  while (temp->left_ != nullptr) {
    temp = temp->left_;
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::findmax() {
  RBTNode<K, V>* temp = this;
  while (temp->right_ != nullptr) {
    temp = temp->right_;
  }
}

template <typename K, typename V>
void RBTNode<K, V>::insert(RBTNode<K, V>* node) {
  RBTNode<K, V>* temp = this;
  RBTNode<K, V>* parent = temp;
  while (temp != nullptr) {
    parent = temp;
    if (temp->key_ < key) {
      temp = temp->right_;
    } else if (key < temp->key_) {
      temp = temp->left_;
    } else {
      std::cout << "Insert an existing key! try again!\n";
      return;
    }
  }
  node->parent_ = parent;
  temp = node;
}

template <typename K, typename V>
void RBTree<K, V>::leftrotate(RBTNode<K, V>*& node) {
  RBTNode<K, V>* temp = node->right_;
  node->right_ = temp->left_;
  if (temp->left_ != nullptr) {
    temp->left_->parent_ = node;
  }
  temp->parent_ = node->parent_;
  temp->left_ = node;
  node = temp;
}

template <typename K, typename V>
void RBTree<K, V>::rightrotate(RBTNode<K, V>*& node) {
  RBTNode<K, V>* temp = node->left_;
  node->left_ = temp;
  if (temp->right_ != nullptr) {
    temp->right_->parent_ = node;
  }
  temp->parent_ = node->parent_;
  temp->right_ = node;
  node = temp;
}

template <typename K, typename V>
RBTree<K, V>::RBTree(K key, V value) {
  root_ = new RBTNode<k, V>(key, value, BLACK, nullptr);
}

template <typename K, typename V>
RBTree<K, V>::RBTree() : root_(nullptr) {}

template <typename K, typename V>
RBTree<K, V>::~RBTree() {}

template <typename K, typename V>
void RBTree<K, V>::remove(K) {
  if (root_ != nullptr) {
    root_->remove(K);
  }
}

template <typename K, typename V>
void RBTree<K, V>::insert(K key, V value) {
  if (root_ == nullptr) {
    root_ = new RBTNode<K, V>(key, value, BLACK, nullptr);
  } else {
    RBTNode<K, V>* node = new RBTNode<K, V>(key, value, RED, nullptr);
    root_->insert(node);
    if (node->parent_->color_ == RED) {
    }
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTree<K, V>::search(K key) {
  if (root_ != nullptr) {
    return root_->search(key);
  }
  return nullptr;
}

template <typename K, typename V>
void RBTree<K, V>::display() {}

#endif