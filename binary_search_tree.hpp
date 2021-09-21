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



#include <iostream>
#include <queue>
#include <memory>

template<typename K, typename V>
class Node{
    K key_;
    V value_;
public:
    std::shared_ptr<Node<K, V> > left_;
    std::shared_ptr<Node<K, V> > right_;
    Node(K, V);
    Node(){}
    ~Node(){}
    K getKey() const;
    V getValue() const;
    void setValue(V);
    void setKey(K);
};

template<typename K, typename V>
Node<K, V>::Node(const K key, const V value){
    key_ = key;
    value_ = value;
    left_ = nullptr;
    right_ = nullptr;
}

template<typename K, typename V>
K Node<K, V>::getKey() const { return key_; }

template<typename K, typename V>
V Node<K, V>::getValue() const { return value_; }

template<typename K, typename V>
void Node<K, V>::setValue(V value) {
    value_ = value;
}

template<typename K, typename V>
void Node<K, V>::setKey(K key) {
    key_ = key;
}

template<typename K, typename V>
class BSTNode {
    std::shared_ptr<Node<K, V> > root_;
    void insert(std::shared_ptr<Node<K, V> >&, K, V);
    void deleteKey(std::shared_ptr<Node<K, V> >&, K);
    std::shared_ptr<Node<K, V> > findMax(std::shared_ptr<Node<K, V> >);
    std::shared_ptr<Node<K, V> > findMin(std::shared_ptr<Node<K, V> >);
public:
    BSTNode();
    BSTNode(K, V);
    ~BSTNode(){}
    void insertElem(K, V);
    void deleteElem(K);
    std::shared_ptr<Node<K, V> > searchElem(K);
    void display();
};

template<typename K, typename V>
BSTNode<K, V>::BSTNode() {
    root_ = nullptr;
}

template<typename K, typename V>
BSTNode<K, V>::BSTNode(K key, V value) {
    root_ = std::make_shared<Node<K, V> >(key, value);
}

template<typename K, typename V>
void BSTNode<K, V>::insertElem(K key, V value) {
    if(root_ == nullptr) {
        root_ = std::make_shared<Node<K, V> >(key, value);
        return;
    }
    insert(root_, key, value);
}

template<typename K, typename V>
void BSTNode<K, V>::insert(std::shared_ptr<Node<K, V> > &node, K key, V value) {
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
}

template<typename K, typename V>
void BSTNode<K, V>::deleteElem(K key) {
    deleteKey(root_, key);
}

template<typename K, typename V>
void BSTNode<K, V>::deleteKey(std::shared_ptr<Node<K, V> > &node, K key) {
    if(node == nullptr) {
        return;
    }
    if(node->getKey() < key) {
        deleteKey(node->right_, key);
    } else if(key < node->getKey()) {
        deleteKey(node->left_, key);
    } else if(key == node->getKey()) {
        if(node->left_ == nullptr && node->right_ == nullptr) {
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
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > BSTNode<K, V>::findMax(std::shared_ptr<Node<K, V> > node) {
    if(node->right_ != nullptr) {
        node = node->right_;
    }
    return node;
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > BSTNode<K, V>::findMin(std::shared_ptr<Node<K, V> > node) {
    if(node->left_ != nullptr) {
        node = node->left_;
    }
    return node;
}

template<typename K, typename V>
std::shared_ptr<Node<K, V> > BSTNode<K, V>::searchElem(K key) {
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
void BSTNode<K, V>::display() {
    std::queue<std::shared_ptr<Node<K, V> > > q;
    std::cout << "------- display BST tree ----------\n";
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
            std::cout << " key:" << temp->getKey() << " value:"
                    << temp->getValue();
            if(temp->left_ != nullptr) {
                q.push(temp->left_);
            }
            if(temp->right_ != nullptr) {
                q.push(temp->right_);
            }
        }
        std::cout << std::endl;
    }
}