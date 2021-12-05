// MIT License

// Copyright (c) 2021 FenglinZhou

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, Merge, publish, distribute, sublicense, and/or sell
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
#include <queue>

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
  // find the index of the key which is greater than or equal to key
  int Findkey(T key);
  // Remove the key which is in a leaf node
  void RemoveFromLeaf(int idx);
  // Remove the key which is a non-leaf node
  void RemoveFromNonLeaf(int idx);
  // get the predecessor of the key
  T GetPred(int idx);
  // get the successor of the key
  T GetSucc(int idx);
  // Fill up th child node
  void Fill(int idx);
  // borrow a key from the child_[idx-1]th node and place it in child_[idx]
  void BorrowFromPrev(int idx);
  // borrow a key from the child_[idx+1]th node and place it in child_[idx]
  void BorrowFromNext(int idx);
  // Merge idx-th child with idx+1 child
  void Merge(int idx);

  void SplitChild(int index, BTNode<T>* node);

 public:
  BTNode(int degree, bool leaf);
  ~BTNode() {}
  void Insertnonfull(T key);
  void Display();
  BTNode<T>* Search(T key);
  void Remove(T key);

  friend class BTree<T>;
};

template <typename T>
class BTree {
  BTNode<T>* root_;
  int degree_;

 public:
  BTree(int degree) : root_(nullptr), degree_(degree) {}
  ~BTree() {}
  void Display();
  BTNode<T>* Search(T key) {
    return (root_ != nullptr) ? root_->Search(key) : nullptr;
  }
  void Insert(T key);
  void Remove(T key);
};

template <typename T>
BTNode<T>::BTNode(int degree, bool leaf) {
  degree_ = degree;
  isleaf_ = leaf;
  keys_ = new T[2 * degree_ - 1];
  child_ = new BTNode<T>*[2 * degree_];
  keycount_ = 0;
}

template <typename T>
int BTNode<T>::Findkey(T key) {
  int index = 0;
  while (index < keycount_ && keys_[index] < key) {
    ++index;
  }
  return index;
}

template <typename T>
void BTNode<T>::RemoveFromLeaf(int idx) {
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
  }
  keycount_ -= 1;
}

template <typename T>
T BTNode<T>::GetPred(int idx) {
  BTNode<T>* cur = child_[idx];
  while (!cur->isleaf_) {
    cur = cur->child_[cur->keycount_];
  }
  return cur->keys_[cur->keycount_ - 1];
}

template <typename T>
T BTNode<T>::GetSucc(int idx) {
  BTNode<T>* cur = child_[idx + 1];
  while (!cur->isleaf_) {
    cur = cur->child_[0];
  }
  return cur->keys_[0];
}

template <typename T>
void BTNode<T>::RemoveFromNonLeaf(int idx) {
  T key = keys_[idx];
  // if the child[idx] has atleast degree keys, find the maximum key int child
  // and replace key by pred, then delete pred;
  if (child_[idx]->keycount_ >= degree_) {
    T pred = GetPred(idx);
    keys_[idx] = pred;
    child_[idx]->Remove(pred);
  }
  // if the child[idx+1] has atleast degree keys, find the minimum key int child
  // and replace key by succ, then delete succ;
  else if (child_[idx + 1]->keycount_ >= degree_) {
    T succ = GetSucc(idx);
    keys_[idx] = succ;
    child_[idx + 1]->Remove(succ);
  }
  // if both child[idx] ans child[idx+1] has less that degree keys,Merge them
  // Remove the key which is in child[idx] now
  else {
    Merge(idx);
    child_[idx]->Remove(key);
  }
}

template <typename T>
void BTNode<T>::Fill(int idx) {
  // borrow a key from prev or next
  if (idx != 0 && child_[idx - 1]->keycount_ >= degree_) {
    BorrowFromPrev(idx);
  } else if (idx != keycount_ && child_[idx + 1]->keycount_ >= degree_) {
    BorrowFromNext(idx);
  } else {
    if (idx != keycount_) {
      Merge(idx);
    } else {
      Merge(idx - 1);
    }
  }
}

template <typename T>
void BTNode<T>::BorrowFromPrev(int idx) {
  BTNode<T>* child = child_[idx];
  BTNode<T>* sibling = child_[idx - 1];
  // Move all keys in C[idx] one step backward
  for (int i = child->keycount_ - 1; i >= 0; --i) {
    child->keys_[i + 1] = child->keys_[i];
  }
  // If C[idx] is not a leaf, move all its child pointers one step backword
  if (!child->isleaf_) {
    for (int i = child->keycount_; i >= 0; --i) {
      child->child_[i + 1] = child->child_[i];
    }
  }
  child->keys_[0] = keys_[idx - 1];
  if (!child->isleaf_) {
    child->child_[0] = sibling->child_[sibling->keycount_];
  }
  keys_[idx - 1] = sibling->keys_[sibling->keycount_ - 1];
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename T>
void BTNode<T>::BorrowFromNext(int idx) {
  BTNode<T>* child = child_[idx];
  BTNode<T>* sibling = child_[idx + 1];
  child->keys_[child->keycount_] = keys_[idx];
  if (!child->isleaf_) {
    child->child_[child->keycount_ + 1] = sibling->child_[0];
  }
  keys_[idx] = sibling->keys_[0];
  for (int i = 1; i < sibling->keycount_; ++i) {
    sibling->keys_[i - 1] = sibling->keys_[i];
  }
  if (!sibling->isleaf_) {
    for (int i = 1; i < sibling->keycount_; ++i) {
      sibling->child_[i - 1] = sibling->child_[i];
    }
  }
  child->keycount_ += 1;
  sibling->keycount_ -= 1;
}

template <typename T>
void BTNode<T>::Remove(T key) {
  // find the index of the key which is greater than or equal to key
  int index = Findkey(key);
  // the key in this node
  if (index < keycount_ && keys_[index] == key) {
    if (isleaf_) {
      // the node is a leaf
      RemoveFromLeaf(index);
    } else {
      // the node is not a leaf
      RemoveFromNonLeaf(index);
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
    // keys, Fill that child
    if (child_[index]->keycount_ < degree_) {
      Fill(index);
    }
    // When has less degree keys in before and after child, the
    // Merge will cause the current node keycount to decrease, then the key to
    // be Removed appears in the child_[indx-1]
    if (flag && index > keycount_) {
      child_[index - 1]->Remove(key);
    } else {
      child_[index]->Remove(key);
    }
  }
}

template <typename T>
void BTNode<T>::Merge(int idx) {
  BTNode<T>* child = child_[idx];
  BTNode<T>* sibling = child_[idx + 1];
  child->keys_[degree_ - 1] = keys_[idx];
  for (int i = 0; i < sibling->keycount_; ++i) {
    child->keys_[i + degree_] = sibling->keys_[i];
  }
  if (!child->isleaf_) {
    for (int i = 0; i < sibling->keycount_; ++i) {
      child->child_[i + degree_] = sibling->child_[i];
    }
  }
  for (int i = idx + 1; i < keycount_; ++i) {
    keys_[i - 1] = keys_[i];
  }
  for (int i = idx + 2; i <= keycount_; ++i) {
    child_[i - 1] = child_[i];
  }
  child->keycount_ += sibling->keycount_ + 1;
  keycount_--;
  delete (sibling);
}

template <typename T>
void BTNode<T>::Display() {
  for (int i = 0; i < keycount_; ++i) {
    std::cout << keys_[i] << " ";
  }
  // std::cout << keycount_ << " ";
}
template <typename T>
void BTNode<T>::Insertnonfull(T key) {
  int i = keycount_ - 1;
  if (isleaf_ == true) {
    // find the location of new key to be Inserted
    // Move all the larger keys to one position behind
    while (i >= 0 && keys_[i] > key) {
      keys_[i + 1] = keys_[i];
      --i;
    }
    // Insert the new key at found lacation
    keys_[i + 1] = key;
    keycount_++;
  } else {
    while (i >= 0 && keys_[i] > key) {
      --i;
    }
    if (child_[i + 1]->keycount_ == 2 * degree_ - 1) {
      SplitChild(i + 1, child_[i + 1]);
      if (keys_[i + 1] < key) {
        i++;
      }
    }
    child_[i + 1]->Insertnonfull(key);
  }
}

template <typename T>
void BTNode<T>::SplitChild(int index, BTNode<T>* node) {
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
BTNode<T>* BTNode<T>::Search(T key) {
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
  return child_[i]->Search(key);
}

template <typename T>
void BTree<T>::Insert(T key) {
  if (root_ == nullptr) {
    root_ = new BTNode<T>(degree_, true);
    root_->keys_[0] = key;
    root_->keycount_ = 1;
  } else {
    if (root_->keycount_ == 2 * degree_ - 1) {
      BTNode<T>* temp = new BTNode<T>(degree_, false);
      temp->child_[0] = root_;
      temp->SplitChild(0, root_);
      int i = 0;
      if (temp->keys_[0] < key) {
        i++;
      }
      temp->child_[i]->Insertnonfull(key);
      root_ = temp;
    } else {
      root_->Insertnonfull(key);
    }
  }
}

template <typename T>
void BTree<T>::Remove(T key) {
  if (root_ == nullptr) {
    std::cout << "The tree is empty\n";
    return;
  }
  root_->Remove(key);
  if (root_->keycount_ == 0) {
    BTNode<T>* temp = root_;
    if (root_->isleaf_) {
      root_ = nullptr;
    } else {
      root_ = root_->child_[0];
    }
    delete temp;
  }
}

template <typename T>
void BTree<T>::Display() {
  if (root_ != nullptr) {
    std::cout << "-------- Display BTree ------\n";
    std::queue<BTNode<T>*> q;
    int level = 1;
    q.push(root_);
    while (!q.empty()) {
      int len = q.size();
      std::cout << "------- level " << level++ << " ------\n";
      for (int i = 0; i < len; ++i) {
        BTNode<T>* temp = q.front();
        q.pop();
        std::cout << " " << i << ":";
        temp->Display();
        if (!temp->isleaf_)
          for (int j = 0; j <= temp->keycount_; ++j) {
            q.push(temp->child_[j]);
          }
      }
      std::cout << std::endl;
    }
  } else {
    std::cout << "BTree is empty!\n";
  }
}

#endif