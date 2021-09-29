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

#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <memory>
#include <queue>

template <typename K, typename V>
class Node {
  K key_;
  V value_;

 public:
  std::shared_ptr<Node<K, V> > left_;
  std::shared_ptr<Node<K, V> > right_;
  Node(K, V);
  Node() {}
  ~Node() {}
  K getKey() const;
  V getValue() const;
  void setValue(V);
  void setKey(K);
  void display();
  int height();
  int leftHeight();
  int rightHeight();
};

template <typename K, typename V>
Node<K, V>::Node(const K key, const V value) {
  key_ = key;
  value_ = value;
  left_ = nullptr;
  right_ = nullptr;
}

template <typename K, typename V>
int Node<K, V>::leftHeight() {
  if (left_ == nullptr) {
    return 0;
  }
  return left_->height();
}

template <typename K, typename V>
int Node<K, V>::rightHeight() {
  if (right_ == nullptr) {
    return 0;
  }
  return right_->height();
}

template <typename K, typename V>
int Node<K, V>::height() {
  return std::max(left_ == nullptr ? 0 : left_->height(),
                  right_ == nullptr ? 0 : right_->height()) +
         1;
}

template <typename K, typename V>
K Node<K, V>::getKey() const {
  return key_;
}

template <typename K, typename V>
V Node<K, V>::getValue() const {
  return value_;
}

template <typename K, typename V>
void Node<K, V>::setValue(V value) {
  value_ = value;
}

template <typename K, typename V>
void Node<K, V>::setKey(K key) {
  key_ = key;
}

template <typename K, typename V>
void Node<K, V>::display() {
  std::cout << "key:" << key_ << " value:" << value_ << std::endl;
}

#endif