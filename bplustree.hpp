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

#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <iostream>

template <typename k, typename v>
class BPTree;

template <typename k, typename v>
class BPTNode {
  // kyes values next&pre node
  k* keys_;
  v* values_;
  BPTNode<k, v>* next_;
  BPTNode<k, v>* pre_;
  // minimum degreee
  int degree_;
  // child pointers
  BPTNode<k, v>** child_;
  // number of keys
  int keycount_;
  // Is true when node is leaf
  bool isleaf_;
  // find the index of the key which is greater than or equal to key
  int findkey(k key);
  // remove the key which is in a leaf node
  void removefromleaf(int idx);
  // remove the key which is a non-leaf node
  void removefromnonleaf(int idx);
  // get the predecessor of the key
  BPTNode<k, v>* getpred(int idx);
  // get the successor of the key
  BPTNode<k, v>* getsucc(int idx);
  // fill up th child node
  void fill(int idx);
  // borrow a key from the child_[idx-1]th node and place it in child_[idx]
  void borrowfromprev(int idx);
  // borrow a key from the child_[idx+1]th node and place it in child_[idx]
  void borrowfromnext(int idx);
  // merge idx-th child with idx+1 child
  void merge(int idx);

 public:
  BPTNode(int degree, bool leaf);
  void insertnonfull(k key, v value);
  void splitchild(int index, BPTNode<k, v>* node);
  void display();
  BPTNode<k, v>* search(k key);
  void remove(k key);

  friend class BPTree<k, v>;
};

template <typename k, typename v>
class BPTree {
  BPTNode<k, v>* root_;
  BPTNode<k, v>* header_;
  int degree_;

 public:
  BPTree(int degree) : root_(nullptr), header_(nullptr), degree_(degree) {}
  void display();
  void displaybylink();
  BPTNode<k, v>* search(k key) {
    return (root_ != nullptr) ? root_->search(key) : nullptr;
  }
  void insert(k key, v value);
  void remove(k key);
};

template <typename k, typename v>
BPTNode<k, v>::BPTNode(int degree, bool leaf) {
  degree_ = degree;
  isleaf_ = leaf;
  keys_ = new k[2 * degree_ - 1];
  next_ = pre_ = nullptr;
  // only leafnode store data
  if (isleaf_) {
    values_ = new v[2 * degree_ - 1];
    child_ = nullptr;
  } else {
    child_ = new BPTNode<k, v>*[2 * degree_];
  }
  keycount_ = 0;
}

template <typename k, typename v>
int BPTNode<k, v>::findkey(k key) {
  int index = 0;
  while (index < keycount_ && keys_[index] < key) {
    ++index;
  }
  return index;
}

template <typename k, typename v>
void BPTNode<k, v>::removefromleaf(int idx) {
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
    values_[i - 1] = values_[i];
  }
  keycount_ -= 1;
}

template <typename k, typename v>
BPTNode<k, v>* BPTNode<k, v>::getpred(int idx) {
  BPTNode<k, v>* cur = child_[idx];
  while (!cur->isleaf_) {
    cur = cur->child_[cur->keycount_];
  }
  cur->keycount_--;
  return cur;
}

template <typename k, typename v>
BPTNode<k, v>* BPTNode<k, v>::getsucc(int idx) {
  BPTNode<k, v>* cur = child_[idx + 1];
  while (!cur->isleaf_) {
    cur = cur->child_[0];
  }
  return cur;
}

template <typename k, typename v>
void BPTNode<k, v>::removefromnonleaf(int idx) {
  k key = keys_[idx];
  // if the child[idx] has atleast degree keys, find the maximum key int child
  // and replace key by pred, then delete pred;
  if (child_[idx]->keycount_ >= degree_) {
    BPTNode<k, v>* pred = getpred(idx);
    keys_[idx] = pred->keys_[pred->keycount_];
    child_[idx + 1]->insertnonfull(pred->keys_[pred->keycount_],
                                   pred->values_[pred->keycount_]);
    child_[idx + 1]->remove(key);
  }
  // if the child[idx+1] has atleast degree keys, find the minimum key int child
  // and replace key by succ, then delete succ;
  else if (child_[idx + 1]->keycount_ >= degree_) {
    BPTNode<k, v>* succ = getsucc(idx);
    keys_[idx] = succ->keys_[1];
    child_[idx + 1]->remove(succ->keys_[0]);
  }
  // if both child[idx] ans child[idx+1] has less that degree keys,merge them
  // remove the key which is in child[idx] now
  else {
    merge(idx);
    child_[idx]->remove(key);
  }
}

template <typename k, typename v>
void BPTNode<k, v>::fill(int idx) {
  // borrow a key from prev or next
  if (idx != 0 && child_[idx - 1]->keycount_ >= degree_) {
    borrowfromprev(idx);
  } else if (idx != keycount_ && child_[idx + 1]->keycount_ >= degree_) {
    borrowfromnext(idx);
  } else {
    if (idx != keycount_) {
      merge(idx);
    } else {
      merge(idx - 1);
    }
  }
}

template <typename k, typename v>
void BPTNode<k, v>::borrowfromprev(int idx) {
  BPTNode<k, v>* child = child_[idx];
  BPTNode<k, v>* sibling = child_[idx - 1];
  // Move all keys in C[idx] one step backward
  for (int i = child->keycount_ - 1; i >= 0; --i) {
    child->keys_[i + 1] = child->keys_[i];
    if (child->isleaf_) {
      child->values_[i + 1] = child->values_[i];
    }
  }
  // If C[idx] is not a leaf, move all its child pointers one step backword
  if (!child->isleaf_) {
    for (int i = child->keycount_; i >= 0; --i) {
      child->child_[i + 1] = child->child_[i];
    }
  }
  child->keys_[0] = keys_[idx - 1];
  if (child->isleaf_) {
    child->values_[0] = sibling->values_[sibling->keycount_ - 1];
  }
  if (!child->isleaf_) {
    child->child_[0] = sibling->child_[sibling->keycount_];
  }
  keys_[idx - 1] = sibling->keys_[sibling->keycount_ - 1];
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename k, typename v>
void BPTNode<k, v>::borrowfromnext(int idx) {
  BPTNode<k, v>* child = child_[idx];
  BPTNode<k, v>* sibling = child_[idx + 1];
  child->keys_[child->keycount_] = keys_[idx];
  if (!child->isleaf_) {
    child->child_[child->keycount_ + 1] = sibling->child_[0];
  } else {
    child->values_[child->keycount_] = sibling->values_[0];
  }
  for (int i = 1; i < sibling->keycount_; ++i) {
    sibling->keys_[i - 1] = sibling->keys_[i];
    if (sibling->isleaf_) {
      sibling->values_[i - 1] = sibling->values_[i];
    }
  }
  if (!sibling->isleaf_) {
    for (int i = 1; i < sibling->keycount_; ++i) {
      sibling->child_[i - 1] = sibling->child_[i];
    }
  }
  keys_[idx] = sibling->keys_[0];
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename k, typename v>
void BPTNode<k, v>::remove(k key) {
  // find the index of the key which is greater than or equal to key
  int index = findkey(key);
  // the key in this node
  if (index < keycount_ && keys_[index] == key) {
    if (isleaf_) {
      // the node is a leaf
      removefromleaf(index);
    } else {
      // the node is not a leaf
      removefromnonleaf(index);
    }
  } else {
    if (isleaf_) {
      std::cout << "The key " << key << " is does not exist\n";
      return;
    }
    // The flag indicates whether the key is present in the sub-tree rooted
    // with the last child of this node
    bool flag = ((index == keycount_) ? true : false);
    // child[index] where the key is supposed to exist  has less that degree
    // keys, fill that child
    if (child_[index]->keycount_ < degree_) {
      fill(index);
    }
    // When has less degree keys in before and after child, the
    // merge will cause the current node keycount to decrease, then the key to
    // be removed appears in the child_[indx-1]
    if (flag && index > keycount_) {
      child_[index - 1]->remove(key);
    } else {
      child_[index]->remove(key);
    }
  }
}

template <typename k, typename v>
void BPTNode<k, v>::merge(int idx) {
  BPTNode<k, v>* child = child_[idx];
  BPTNode<k, v>* sibling = child_[idx + 1];
  if (!child->isleaf_) {
    for (int i = 0; i < sibling->keycount_; ++i) {
      child->child_[i + degree_] = sibling->child_[i];
    }
    child->keys_[degree_ - 1] = keys_[idx];
    for (int i = 0; i < sibling->keycount_; ++i) {
      child->keys_[i + degree_] = sibling->keys_[i];
    }
    child->keycount_ += sibling->keycount_ + 1;
  } else {
    for (int i = 0; i < sibling->keycount_; ++i) {
      child->keys_[i + degree_ - 1] = sibling->keys_[i];
      child->values_[i + degree_ - 1] = sibling->values_[i];
    }
    child->keycount_ += sibling->keycount_;
    if (sibling->next_ != nullptr) {
      child->next_ = sibling->next_;
      sibling->next_->pre_ = child;
    } else {
      child->next_ = nullptr;
    }
  }
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
  }
  for (int i = idx + 2; i <= keycount_; ++i) {
    child_[i - 1] = child_[i];
  }
  keycount_--;
  delete (sibling);
}

template <typename k, typename v>
void BPTNode<k, v>::display() {
  for (int i = 0; i < keycount_; ++i) {
    std::cout << "key:" << keys_[i] << " ";
    if (isleaf_) {
      std::cout << "value:" << values_[i] << " ";
    }
  }
  std::cout << "\n";
}
template <typename k, typename v>
void BPTNode<k, v>::insertnonfull(k key, v value) {
  int i = keycount_ - 1;
  if (isleaf_ == true) {
    // find the location of new key to be inserted
    // Move all the larger keys to one position behind
    while (i >= 0 && keys_[i] > key) {
      keys_[i + 1] = keys_[i];
      values_[i + 1] = values_[i];
      --i;
    }
    // insert the new key at found lacation
    keys_[i + 1] = key;
    values_[i + 1] = value;
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
    child_[i + 1]->insertnonfull(key, value);
  }
}

template <typename k, typename v>
void BPTNode<k, v>::splitchild(int index, BPTNode<k, v>* node) {
  // create a new node which is going to store last (degree-1) keys of node
  // node->kyes_ 0, degree-1, degree, 2degree-1
  //             | - - - - - |  -  | - - - - - |
  BPTNode<k, v>* temp = new BPTNode<k, v>(node->degree_, node->isleaf_);
  if (node->isleaf_) {
    temp->keycount_ = degree_;
    // copy the last degree keys of node to temp
    for (int i = 0; i < degree_; ++i) {
      temp->keys_[i] = node->keys_[i + degree_ - 1];
      temp->values_[i] = node->values_[i + degree_ - 1];
    }
    // link leaf nodes
    temp->next_ = node->next_;
    node->next_ = temp;
    temp->pre_ = node;
    if (temp->next_ != nullptr) temp->next_->pre_ = temp;
  } else {
    temp->keycount_ = degree_ - 1;
    // copy the last degree - 1 keys of node to temp
    for (int i = 0; i < degree_ - 1; ++i) {
      temp->keys_[i] = node->keys_[i + degree_];
    }
    // copy the last degree children of node to temp
    for (int i = 0; i < degree_; ++i) {
      temp->child_[i] = node->child_[i + degree_];
    }
  }
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

template <typename k, typename v>
BPTNode<k, v>* BPTNode<k, v>::search(k key) {
  int i = 0;
  while (i < keycount_ && key > keys_[i]) {
    ++i;
  }
  if (key == keys_[i] && isleaf_ == true) {
    std::cout << "find it! key: " << key << "value:" << values_[i] << std::endl;
    return this;
  }
  if (isleaf_ == true) {
    std::cout << "not find " << key << std::endl;
    return nullptr;
  }
  return child_[i]->search(key);
}

template <typename k, typename v>
void BPTree<k, v>::insert(k key, v value) {
  if (root_ == nullptr) {
    root_ = new BPTNode<k, v>(degree_, true);
    root_->keys_[0] = key;
    root_->values_[0] = value;
    root_->keycount_ = 1;
    header_ = root_;
  } else {
    if (root_->keycount_ == 2 * degree_ - 1) {
      BPTNode<k, v>* temp = new BPTNode<k, v>(degree_, false);
      temp->child_[0] = root_;
      temp->splitchild(0, root_);
      int i = 0;
      if (temp->keys_[0] < key) {
        i++;
      }
      temp->child_[i]->insertnonfull(key, value);
      root_ = temp;
    } else {
      root_->insertnonfull(key, value);
    }
  }
}

template <typename k, typename v>
void BPTree<k, v>::remove(k key) {
  if (root_ == nullptr) {
    std::cout << "The tree is empty\n";
    return;
  }
  root_->remove(key);
  if (root_->keycount_ == 0) {
    BPTNode<k, v>* temp = root_;
    if (root_->isleaf_) {
      root_ = nullptr;
    } else {
      root_ = root_->child_[0];
    }
    delete temp;
  }
}

template <typename k, typename v>
void BPTree<k, v>::display() {
  if (root_ != nullptr) {
    std::cout << "-------- Display BPTree ------\n";
    std::queue<BPTNode<k, v>*> q;
    int level = 1;
    q.push(root_);
    while (!q.empty()) {
      int len = q.size();
      std::cout << "------- level " << level++ << " ------\n";
      for (int i = 0; i < len; ++i) {
        BPTNode<k, v>* temp = q.front();
        q.pop();
        std::cout << " " << i << ":";
        temp->display();
        if (!temp->isleaf_)
          for (int j = 0; j <= temp->keycount_; ++j) {
            q.push(temp->child_[j]);
          }
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << "BPTree is empty!\n";
  }
}

template <typename k, typename v>
void BPTree<k, v>::displaybylink() {
  if (header_ != nullptr) {
    BPTNode<k, v>* temp = header_;
    std::cout << "-------- Display BPTree ------\n";
    while (temp != nullptr) {
      temp->display();
      temp = temp->next_;
    }
  }
}
#endif