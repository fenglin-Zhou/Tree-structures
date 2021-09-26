// MIT License

// Copyright (c) 2021 FenglinZhou

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

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

template<typename K, typename V>
class AVLTree {
    std::shared_ptr<Node<K, V> > root_;
    void insert(std::shared_ptr<Node<K, V> >&, K, V);
    void deleteKey(std::shared_ptr<Node<K, V> >&, K);
    void leftRotate(std::shared_ptr<Node<K, V> >&);
    void rightRotate(std::shared_ptr<Node<K, V> >&);
    void balance(std::shared_ptr<Node<K, V> >&);
    std::shared_ptr<Node<K, V> > findMax(std::shared_ptr<Node<K, V> >);
    std::shared_ptr<Node<K, V> > findMin(std::shared_ptr<Node<K, V> >);
public:
    AVLTree();
    AVLTree(K, V);
    ~AVLTree(){}
    void insertElem(K, V);
    void deleteElem(K);
    std::shared_ptr<Node<K, V> > searchElem(K);
    void display();
};

template<typename K, typename V>
AVLTree<K, V>::AVLTree() {
    root_ = nullptr;
}

template<typename K, typename V>
AVLTree<K, V>::AVLTree(K key, V value) {
    root_ = std::make_shared<Node<K, V> >(key, value);
}

template<typename K, typename V>
void AVLTree<K, V>::leftRotate(std::shared_ptr<Node<K, V> >& node) {
    std::shared_ptr<Node<K, V> > temp = node->right_;
    node->right_ = temp->left_;
    temp->left_ = node;
    node = temp;
}

template<typename K, typename V>
void AVLTree<K, V>::rightRotate(std::shared_ptr<Node<K, V> >& node) {
    std::shared_ptr<Node<K, V> > temp = node->left_;
    node->left_ = temp->right_;
    temp->right_ = node;
    node = temp;
}

template<typename K, typename V>
void AVLTree<K, V>::balance(std::shared_ptr<Node<K, V> >& node) {
    if(node == nullptr) {
        return;
    }
    if(node->leftHeight() - node->rightHeight() > 1) {
        if(node->left_->rightHeight() > node->left_->leftHeight()) {
            leftRotate(node->left_);
        }
        rightRotate(node);
    } else if(node->rightHeight() - node->leftHeight() > 1) {
        if(node->right_->leftHeight() > node->right_->rightHeight()) {
            rightRotate(node->right_);
        }
        leftRotate(node);
    }
}

template<typename K, typename V>
void AVLTree<K, V>::insertElem(K key, V value) {
    if(root_ == nullptr) {
        root_ = std::make_shared<Node<K, V> >(key, value);
        return;
    }
    insert(root_, key, value);
}

template<typename K, typename V>
void AVLTree<K, V>::insert(std::shared_ptr<Node<K, V> > &node, K key, V value) {
    if(node == nullptr) {
        node = std::make_shared<Node<K, V> >(key, value);
        return;
    }
    if(node->getKey() < key) {
        insert(node->right_, key, value);
    } else if(key < node->getKey()) {
        insert(node->left_, key, value);
    } else {
        std::cout << "newkey:" << key << "has existed\n" << std::endl;
        std::cout << "You are trying to insert an existing element, please try again!\n";
    }
    balance(node);
}

template<typename K, typename V>
void AVLTree<K, V>::deleteElem(K key) {
    deleteKey(root_, key);
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::findMax(std::shared_ptr<Node<K, V> > node) {
    if(node->right_ != nullptr) {
        node = node->right_;
    }
    return node;
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::findMin(std::shared_ptr<Node<K, V> > node) {
    if(node->left_ != nullptr) {
        node = node->left_;
    }
    return node;
}

template<typename K, typename V>
void AVLTree<K, V>::deleteKey(std::shared_ptr<Node<K, V> > &node, K key) {
    if(node == nullptr) {
        return;
    }
    if(node->getKey() < key) {
        deleteKey(node->right_, key);
    } else if(key < node->getKey()) {
        deleteKey(node->left_, key);
    } else if(key == node->getKey()) {
        if(node->left_ == nullptr && node->right_ == nullptr) {
            std::cout << "remove: ";
            node->display();
            node = nullptr;
            return;
        } else if(node->left_ != nullptr) {
            std::shared_ptr<Node<K, V> > pre = findMax(node->left_);
            node->setKey(pre->getKey());
            node->setValue(pre->getValue());
            deleteKey(node->left_, pre->getKey());
        } else {
            std::shared_ptr<Node<K, V> > next = findMin(node->right_);
            node->setKey(next->getKey());
            node->setValue(next->getValue());
            deleteKey(node->right_, next->getKey());
        }
    }
    balance(node);
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > AVLTree<K, V>::searchElem(K key) {
    if(root_ == nullptr){
        std::cout << "Not found!\n";
        return nullptr;
    }
    std::shared_ptr<Node<K, V> > node = root_;
    while(node != nullptr) {
        if(node->getKey() == key) {
            std::cout << "Found it! key:" << key << " value:" 
                << node->getValue() << std::endl;
            return node;
        } else if(node->getKey() < key) {
            node = node->right_;
        } else if(key < node->getKey()) {
            node = node->left_;
        }
    }
    std::cout << "Not found!\n";
    return nullptr;
}

template<typename K, typename V>
void AVLTree<K, V>::display() {
    std::queue<std::shared_ptr<Node<K, V> > > q;
    std::cout << "------- Display AVLTree ----------\n";
    if (root_ == nullptr) {
        std::cout << " No elements !\n";
    }
    int level = 1;
    q.push(root_);
    while(!q.empty()) {
        int len = q.size();
        std::cout << "------- level  " << level++ << " -------\n";
        for(int i = 0; i < len; ++i) {
            std::shared_ptr<Node<K, V> > temp = q.front();
            q.pop();
            temp->display();
            if(temp->left_ != nullptr) {
                q.push(temp->left_);
            }
            if(temp->right_ != nullptr) {
                q.push(temp->right_);
            }
        }
    }
}

#endif