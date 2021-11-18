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

#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <iostream>
#include <memory>

#include "node.hpp"

template <typename K, typename V>
class AVLTree {
  std::shared_ptr<Node<K, V> > root_;
  void Insert(std::shared_ptr<Node<K, V> >&, K, V);
  void DeleteKey(std::shared_ptr<Node<K, V> >&, K);
  void LeftRotate(std::shared_ptr<Node<K, V> >&);
  void RightRotate(std::shared_ptr<Node<K, V> >&);
  void Balance(std::shared_ptr<Node<K, V> >&);
  std::shared_ptr<Node<K, V> > FindMax(std::shared_ptr<Node<K, V> >);
  std::shared_ptr<Node<K, V> > FindMin(std::shared_ptr<Node<K, V> >);

 public:
  AVLTree();
  AVLTree(K, V);
  ~AVLTree() {}
  void Insert(K, V);
  void Remove(K);
  std::shared_ptr<Node<K, V> > Search(K);
  void Display();
};

template <typename K, typename V>
AVLTree<K, V>::AVLTree() {
  root_ = nullptr;
}

template <typename K, typename V>
AVLTree<K, V>::AVLTree(K key, V value) {
  root_ = std::make_shared<Node<K, V> >(key, value);
}

template <typename K, typename V>
void AVLTree<K, V>::LeftRotate(std::shared_ptr<Node<K, V> >& node) {
  std::shared_ptr<Node<K, V> > temp = node->right_;
  node->right_ = temp->left_;
  temp->left_ = node;
  node = temp;
}

template <typename K, typename V>
void AVLTree<K, V>::RightRotate(std::shared_ptr<Node<K, V> >& node) {
  std::shared_ptr<Node<K, V> > temp = node->left_;
  node->left_ = temp->right_;
  temp->right_ = node;
  node = temp;
}

template <typename K, typename V>
void AVLTree<K, V>::Balance(std::shared_ptr<Node<K, V> >& node) {
  if (node == nullptr) {
    return;
  }
  if (node->leftHeight() - node->rightHeight() > 1) {
    if (node->left_->rightHeight() > node->left_->leftHeight()) {
      LeftRotate(node->left_);
    }
    RightRotate(node);
  } else if (node->rightHeight() - node->leftHeight() > 1) {
    if (node->right_->leftHeight() > node->right_->rightHeight()) {
      RightRotate(node->right_);
    }
    LeftRotate(node);
  }
}

template <typename K, typename V>
void AVLTree<K, V>::Insert(K key, V value) {
  if (root_ == nullptr) {
    root_ = std::make_shared<Node<K, V> >(key, value);
    return;
  }
  Insert(root_, key, value);
}

template <typename K, typename V>
void AVLTree<K, V>::Insert(std::shared_ptr<Node<K, V> >& node, K key, V value) {
  if (node == nullptr) {
    node = std::make_shared<Node<K, V> >(key, value);
    return;
  }
  if (node->getKey() < key) {
    Insert(node->right_, key, value);
  } else if (key < node->getKey()) {
    Insert(node->left_, key, value);
  } else {
    std::cout << "newkey:" << key << "has existed\n" << std::endl;
    std::cout
        << "You are trying to Insert an existing element, please try again!\n";
  }
  Balance(node);
}

template <typename K, typename V>
void AVLTree<K, V>::Remove(K key) {
  DeleteKey(root_, key);
}

template <typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::FindMax(
    std::shared_ptr<Node<K, V> > node) {
  if (node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::FindMin(
    std::shared_ptr<Node<K, V> > node) {
  if (node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template <typename K, typename V>
void AVLTree<K, V>::DeleteKey(std::shared_ptr<Node<K, V> >& node, K key) {
  if (node == nullptr) {
    return;
  }
  if (node->getKey() < key) {
    DeleteKey(node->right_, key);
  } else if (key < node->getKey()) {
    DeleteKey(node->left_, key);
  } else if (key == node->getKey()) {
    if (node->left_ == nullptr && node->right_ == nullptr) {
      std::cout << "Remove: ";
      node->Display();
      node = nullptr;
      return;
    } else if (node->left_ != nullptr) {
      std::shared_ptr<Node<K, V> > pre = FindMax(node->left_);
      node->setKey(pre->getKey());
      node->setValue(pre->getValue());
      DeleteKey(node->left_, pre->getKey());
    } else {
      std::shared_ptr<Node<K, V> > next = FindMin(node->right_);
      node->setKey(next->getKey());
      node->setValue(next->getValue());
      DeleteKey(node->right_, next->getKey());
    }
  }
  Balance(node);
}

template <typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::Search(K key) {
  if (root_ == nullptr) {
    std::cout << "Not found!\n";
    return nullptr;
  }
  std::shared_ptr<Node<K, V> > node = root_;
  while (node != nullptr) {
    if (node->getKey() == key) {
      std::cout << "Found it! key:" << key << " value:" << node->getValue()
                << std::endl;
      return node;
    } else if (node->getKey() < key) {
      node = node->right_;
    } else if (key < node->getKey()) {
      node = node->left_;
    }
  }
  std::cout << "Not found!\n";
  return nullptr;
}

template <typename K, typename V>
void AVLTree<K, V>::Display() {
  std::queue<std::shared_ptr<Node<K, V> > > q;
  std::cout << "------- Display AVLTree ----------\n";
  if (root_ == nullptr) {
    std::cout << " No elements !\n";
  }
  int level = 1;
  q.push(root_);
  while (!q.empty()) {
    int len = q.size();
    std::cout << "------- level  " << level++ << " -------\n";
    for (int i = 0; i < len; ++i) {
      std::shared_ptr<Node<K, V> > temp = q.front();
      q.pop();
      temp->Display();
      if (temp->left_ != nullptr) {
        q.push(temp->left_);
      }
      if (temp->right_ != nullptr) {
        q.push(temp->right_);
      }
    }
  }
}

#endif