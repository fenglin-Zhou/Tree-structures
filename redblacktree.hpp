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

  RBTNode<K, V>* FindMin();
  RBTNode<K, V>* FindMax();
  RBTNode<K, V>* Insert(K, V);

 public:
  RBTNode() {}
  RBTNode(K key, V value, RBTcolor color, RBTNode<K, V>* parent,
          RBTNode<K, V>* left, RBTNode<K, V>* right);
  ~RBTNode() {}
  void Remove(K);
  RBTNode<K, V>* Search(K);
  void Display();

  friend class RBTree<K, V>;
};

template <typename K, typename V>
class RBTree {
 private:
  RBTNode<K, V>* root_;
  void LeftRotate(RBTNode<K, V>*& node);
  void RightRotate(RBTNode<K, V>*& node);
  void InsertFix(RBTNode<K, V>*);
  void Remove(RBTNode<K, V>*);
  RBTNode<K, V>* FindMin(RBTNode<K, V>*);
  void RemoveNoLeaf(RBTNode<K, V>*);
  void RemoveTwoLeaf(RBTNode<K, V>*);
  void RemoveFixup(RBTNode<K, V>* child, RBTNode<K, V>* parent);

 public:
  RBTree();
  RBTree(K, V);
  ~RBTree();
  void Remove(K);
  void Insert(K, V);
  void Display();
  RBTNode<K, V>* Search(K);
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
RBTNode<K, V>* RBTNode<K, V>::Search(K key) {
  if (key_ == key) {
    return this;
  } else if (key_ < key && right_ != nullptr) {
    return right_->Search(key);
  } else if (key < key_ && left_ != nullptr) {
    return left_->Search(key);
  } else {
    std::cout << "Not found!\n";
    return nullptr;
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::FindMin() {
  RBTNode<K, V>* temp = this;
  while (temp->left_ != nullptr) {
    temp = temp->left_;
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::FindMax() {
  RBTNode<K, V>* temp = this;
  while (temp->right_ != nullptr) {
    temp = temp->right_;
  }
}

template <typename K, typename V>
void RBTNode<K, V>::Display() {
  std::cout << "key:" << key_ << ", value:" << value_ << ", color:";
  if (color_ == RED)
    std::cout << "red \n";
  else
    std::cout << "black \n";
}

template <typename K, typename V>
RBTNode<K, V>* RBTNode<K, V>::Insert(K key, V value) {
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
void RBTree<K, V>::LeftRotate(RBTNode<K, V>*& node) {
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
  node = temp;
}

template <typename K, typename V>
void RBTree<K, V>::RightRotate(RBTNode<K, V>*& node) {
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
  node = temp;
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
void RBTree<K, V>::Remove(K key) {
  if (root_ != nullptr) {
    RBTNode<K, V>* Removenode = Search(key);
    if (Removenode != nullptr) {
      Remove(Removenode);
    }
  }
}

template <typename K, typename V>
void RBTree<K, V>::RemoveFixup(RBTNode<K, V>* child, RBTNode<K, V>* parent) {
  RBTNode<K, V>* bro = nullptr;
  while ((!child) || child->color_ == BLACK && child != root_) {
    if (child == parent->left_) {
      bro = parent->right_;
      if (bro->color_ == RED) {
        bro->color_ = BLACK;
        parent->color_ = RED;
        LeftRotate(parent);
        bro = parent->right_;
      }
      if ((bro->right_ == nullptr || bro->right_->color_ == BLACK) &&
          (bro->left_ == nullptr || bro->left_->color_ == BLACK)) {
        bro->color_ = RED;
        child = parent;
        parent = child->parent_;
      } else {
        if ((bro->right_ == nullptr || bro->right_->color_ == BLACK) &&
            (bro->left_ == nullptr || bro->left_->color_ == RED)) {
          bro->color_ = RED;
          bro->left_->color_ = BLACK;
          RightRotate(bro);
          bro = parent->right_;
        }
        bro->color_ = parent->color_;
        parent->color_ = BLACK;
        bro->right_->color_ = BLACK;
        LeftRotate(parent);
        child = root_;
        break;
      }
    } else {
      bro = parent->left_;
      if (bro->color_ == RED) {
        bro->color_ = BLACK;
        parent->color_ = RED;
        RightRotate(parent);
      }
      if ((bro->right_ == nullptr || bro->right_->color_ == BLACK) &&
          (bro->left_ == nullptr || bro->left_->color_ == BLACK)) {
        bro->color_ = RED;
        child = parent;
        parent = child->parent_;
      } else {
        if ((bro->left_ == nullptr || bro->left_->color_ == BLACK) &&
            (bro->right_ == nullptr || bro->right_->color_ == RED)) {
          bro->color_ = RED;
          bro->right_->color_ = BLACK;
          LeftRotate(bro);
          bro = parent->left_;
        }
        bro->color_ = parent->color_;
        parent->color_ = BLACK;
        bro->left_->color_ = BLACK;
        RightRotate(parent);
        child = root_;
        break;
      }
    }
  }
  if (child != nullptr) {
    child->color_ = BLACK;
  }
}

template <typename K, typename V>
void RBTree<K, V>::Remove(RBTNode<K, V>* node) {
  RBTNode<K, V>* child = nullptr;
  RBTNode<K, V>* parent = node->parent_;
  RBTcolor color;
  if (node->left_ == nullptr && node->right_ == nullptr) {
    RemoveNoLeaf(node);
    return;
  } else if (node->left_ != nullptr && node->right_ != nullptr) {
    RemoveTwoLeaf(node);
    return;
  } else if (node->left_ != nullptr) {
    child = node->left_;
  } else {
    child = node->right_;
  }
  parent = node->parent_;
  color = node->color_;
  if (child != nullptr) {
    child->parent_ = parent;
  }
  if (parent != nullptr) {
    if (parent->right_ == node)
      parent->right_ = child;
    else
      parent->left_ = child;
  } else {
    root_ = child;
  }
  if (color == BLACK) {
    RemoveFixup(child, parent);
  }
  delete node;
  node = nullptr;
}

template <typename K, typename V>
void RBTree<K, V>::RemoveTwoLeaf(RBTNode<K, V>* node) {
  RBTNode<K, V>* replace = FindMin(node);
  RBTNode<K, V>* child = nullptr;
  RBTNode<K, V>* parent = nullptr;
  RBTcolor color;
  if (node->parent_ != nullptr) {
    if (node->parent_->left_ == node)
      node->parent_->left_ = replace;
    else
      node->parent_->right_ = replace;
  } else {
    root_ = replace;
  }
  child = replace->right_;
  parent = replace->parent_;
  color = replace->color_;
  if (parent == node) {
    parent = replace;
  } else {
    if (child != nullptr) {
      child->parent_ = parent;
    }
    parent->left_ = child;
    replace->right_ = node->right_;
    node->right_->parent_ = replace;
  }
  replace->parent_ = node->parent_;
  replace->color_ = node->color_;
  replace->left_ = node->left_;
  node->left_->parent_ = replace;
  if (color == BLACK) {
    RemoveFixup(child, parent);
  }
  delete node;
  node = nullptr;
}

template <typename K, typename V>
void RBTree<K, V>::RemoveNoLeaf(RBTNode<K, V>* node) {
  RBTNode<K, V>* parent = node->parent_;
  if (parent != nullptr) {
    if (parent->right_ == node)
      parent->right_ = nullptr;
    else
      parent->left_ = nullptr;
    delete node;
  } else {
    delete node;
    root_ = node = nullptr;
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTree<K, V>::FindMin(RBTNode<K, V>* node) {
  RBTNode<K, V>* minnode = node->right_;
  while (minnode->left_ != nullptr) {
    minnode = minnode->left_;
  }
  return minnode;
}

template <typename K, typename V>
void RBTree<K, V>::InsertFix(RBTNode<K, V>* node) {
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
          LeftRotate(parent);
          std::swap(node, parent);
        }
        parent->color_ = BLACK;
        gparent->color_ = RED;
        RightRotate(gparent);
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
          RightRotate(parent);
          std::swap(node, parent);
        }
        parent->color_ = BLACK;
        gparent->color_ = RED;
        LeftRotate(gparent);
        break;
      }
    }
  }
  root_->color_ = BLACK;
}

template <typename K, typename V>
void RBTree<K, V>::Insert(K key, V value) {
  if (root_ == nullptr) {
    root_ = new RBTNode<K, V>(key, value, BLACK, nullptr, nullptr, nullptr);
  } else {
    RBTNode<K, V>* temp = root_->Insert(key, value);
    if (temp != nullptr) {
      InsertFix(temp);
    }
  }
}

template <typename K, typename V>
RBTNode<K, V>* RBTree<K, V>::Search(K key) {
  if (root_ != nullptr) {
    return root_->Search(key);
  }
  return nullptr;
}

template <typename K, typename V>
void RBTree<K, V>::Display() {
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
        temp->Display();
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