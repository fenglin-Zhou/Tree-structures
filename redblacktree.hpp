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
#include <queue>

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
  RBTNode<K, V>* insert(K, V);

 public:
  RBTNode() {}
  RBTNode(K key, V value, RBTcolor color, RBTNode<K, V>* parent,
          RBTNode<K, V>* left, RBTNode<K, V>* right);
  ~RBTNode() {}
  void remove(K);
  RBTNode<K, V>* search(K);
  void display();

  friend class RBTree<K, V>;
};

template <typename K, typename V>
class RBTree {
 private:
  RBTNode<K, V>* root_;
  void leftrotate(RBTNode<K, V>*& node);
  void rightrotate(RBTNode<K, V>*& node);
  void insertfix(RBTNode<K, V>*);

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
                       RBTNode<K, V>* left, RBTNode<K, V>* right)
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
void RBTNode<K, V>::display() {
  std::cout << "key:" << key_ << ", value:" << value_ << ", color:";
  if (color_ == RED)
    std::cout << "red \n";
  else
    std::cout << "black \n";
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::insert(K key, V value) {
  RBTNode<K, V>* temp = this;
  RBTNode<K, V>* parent = temp;
  RBTNode<K, V>* newnode =
      new RBTNode<K, V>(key, value, RED, nullptr, nullptr, nullptr);
  while (temp != nullptr) {
    parent = temp;
    if (temp->key_ < key) {
      temp = temp->right_;
    } else if (key < temp->key_) {
      temp = temp->left_;
    } else {
      delete newnode;
      std::cout << "Insert an existing key! Don't do this!\n";
      return nullptr;
    }
  }
  if (parent != nullptr) {
    if (parent->key_ < key)
      parent->right_ = newnode;
    else
      parent->left_ = newnode;
  }
  newnode->parent_ = parent;
  return newnode;
}

template <typename K, typename V>
void RBTree<K, V>::leftrotate(RBTNode<K, V>*& node) {
  RBTNode<K, V>* temp = node->right_;
  node->right_ = temp->left_;
  if (temp->left_ != nullptr) {
    temp->left_->parent_ = node;
  }
  temp->parent_ = node->parent_;
  if (node->parent_ != nullptr) {
    if (node == node->parent_->right_)
      node->parent_->right_ = temp;
    else
      node->parent_->left_ = temp;
  } else {
    root_ = temp;
  }
  temp->left_ = node;
  node->parent_ = temp;
}

template <typename K, typename V>
void RBTree<K, V>::rightrotate(RBTNode<K, V>*& node) {
  RBTNode<K, V>* temp = node->left_;
  node->left_ = temp->right_;
  if (temp->right_ != nullptr) {
    temp->right_->parent_ = node;
  }
  temp->parent_ = node->parent_;
  if (node->parent_ != nullptr) {
    if (node == node->parent_->right_)
      node->parent_->right_ = temp;
    else
      node->parent_->left_ = temp;
  } else {
    root_ = temp;
  }
  temp->right_ = node;
  node->parent_ = temp;
}

template <typename K, typename V>
RBTree<K, V>::RBTree(K key, V value) {
  root_ = new RBTNode<K, V>(key, value, BLACK, nullptr);
}

template <typename K, typename V>
RBTree<K, V>::RBTree() : root_(nullptr) {}

template <typename K, typename V>
RBTree<K, V>::~RBTree() {}

template <typename K, typename V>
void RBTree<K, V>::remove(K key) {
  if (root_ != nullptr) {
    root_->remove(key);
  }
}

template <typename K, typename V>
void RBTree<K, V>::insertfix(RBTNode<K, V>* node) {
  RBTNode<K, V>* parent;
  parent = node->parent_;
  while (node != root_ && parent->color_ == RED) {
    RBTNode<K, V>* gparent = parent->parent_;
    if (gparent->left_ == parent) {
      RBTNode<K, V>* uncle = gparent->right_;
      if (uncle != nullptr && uncle->color_ == RED) {
        parent->color_ = BLACK;
        uncle->color_ = BLACK;
        gparent->color_ = RED;
        node = gparent;
        parent = node->parent_;
      } else {
        if (node == parent->right_) {
          leftrotate(parent);
          std::swap(node, parent);
        }
        parent->color_ = BLACK;
        gparent->color_ = RED;
        rightrotate(gparent);
        break;
      }
    } else {
      RBTNode<K, V>* uncle = gparent->left_;
      if (uncle != nullptr && uncle->color_ == RED) {
        parent->color_ = BLACK;
        gparent->color_ = RED;
        node = gparent;
        parent = node->parent_;
      } else {
        if (node == parent->left_) {
          rightrotate(parent);
          std::swap(node, parent);
        }
        parent->color_ = BLACK;
        gparent->color_ = RED;
        leftrotate(gparent);
        break;
      }
    }
  }
  root_->color_ = BLACK;
}

template <typename K, typename V>
void RBTree<K, V>::insert(K key, V value) {
  if (root_ == nullptr) {
    root_ = new RBTNode<K, V>(key, value, BLACK, nullptr, nullptr, nullptr);
  } else {
    RBTNode<K, V>* temp = root_->insert(key, value);
    if (temp != nullptr) {
      insertfix(temp);
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
void RBTree<K, V>::display() {
  if (root_ != nullptr) {
    std::cout << "---------Red Black Tree------\n";
    std::queue<RBTNode<K, V>*> q;
    q.push(root_);
    int level = 1;
    int len = 0;
    RBTNode<K, V>* temp = nullptr;
    while (!q.empty()) {
      len = q.size();
      std::cout << "---------level" << level++ << "-------\n";
      for (int i = 0; i < len; ++i) {
        temp = q.front();
        q.pop();
        temp->display();
        if (temp->left_ != nullptr) {
          q.push(temp->left_);
        }
        if (temp->right_ != nullptr) {
          q.push(temp->right_);
        }
      }
      std::cout << std::endl;
    }
  }
}

#endif