//
// Created by Lddyss on 2025/1/30.
//
#include "rb_tree.h"
#include <algorithm>

// 构造函数
template <typename Key, class Comparator>
RBTree<Key, Comparator>::RBTree() : root_(nullptr) {}

// 析构函数
template <typename Key, class Comparator>
RBTree<Key, Comparator>::~RBTree() {
    destroy(root_);
}

// 递归销毁树
template <typename Key, class Comparator>
void RBTree<Key, Comparator>::destroy(Node* node) {
    if (node != nullptr) {
        destroy(node->left_);
        destroy(node->right_);
        delete node;
    }
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::findNode(const Key &k, RBTree::Node** res) const {
    Node* p = root_;
    while (p != nullptr) {
        if (res != nullptr) {
            *res = p;
        }
        if (compare_(k, p->k_)) {
            p = p->left_;
        } else if (compare_(p->k_, k)) {
            p = p->right_;
        } else {
            if (res != nullptr) {
                *res = p;
            }
            return true;
        }
    }
    return false;
}


template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::insertNode(const Key &k, Node* &n_node) {
    Node* p = nullptr;
    if (findNode(k, &p)) {
        return false;
    }

    n_node = new Node(k, RED, nullptr, nullptr, p);
    if (p == nullptr) {
        root_ = n_node;
    } else if (compare_(k, p->k_)) {
        p->left_ = n_node;
    } else {
        p->right_ = n_node;
    }
    insertFixup(n_node);
    return true;
}

// 插入修复
template <typename Key, class Comparator>
void RBTree<Key, Comparator>::insertFixup(Node* z) {
    while (z->parent_ != nullptr && z->parent_->color_ == RED) {
        if (z->parent_ == z->parent_->parent_->left_) {
            Node* y = z->parent_->parent_->right_;
            if (y != nullptr && y->color_ == RED) {
                z->parent_->color_ = BLACK;
                y->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                z = z->parent_->parent_;
            } else {
                if (z == z->parent_->right_) {
                    z = z->parent_;
                    leftRotate(z);
                }
                z->parent_->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                rightRotate(z->parent_->parent_);
            }
        } else {
            Node* y = z->parent_->parent_->left_;
            if (y != nullptr && y->color_ == RED) {
                z->parent_->color_ = BLACK;
                y->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                z = z->parent_->parent_;
            } else {
                if (z == z->parent_->left_) {
                    z = z->parent_;
                    rightRotate(z);
                }
                z->parent_->color_ = BLACK;
                z->parent_->parent_->color_ = RED;
                leftRotate(z->parent_->parent_);
            }
        }
    }
    root_->color_ = BLACK;
}

// 左旋
template <typename Key, class Comparator>
void RBTree<Key, Comparator>::leftRotate(Node* x) {
    Node* y = x->right_;
    x->right_ = y->left_;
    if (y->left_ != nullptr) {
        y->left_->parent_ = x;
    }
    y->parent_ = x->parent_;
    if (x->parent_ == nullptr) {
        root_ = y;
    } else if (x == x->parent_->left_) {
        x->parent_->left_ = y;
    } else {
        x->parent_->right_ = y;
    }
    y->left_ = x;
    x->parent_ = y;
}

// 右旋
template <typename Key, class Comparator>
void RBTree<Key, Comparator>::rightRotate(Node* x) {
    Node* y = x->left_;
    x->left_ = y->right_;
    if (y->right_ != nullptr) {
        y->right_->parent_ = x;
    }
    y->parent_ = x->parent_;
    if (x->parent_ == nullptr) {
        root_ = y;
    } else if (x == x->parent_->right_) {
        x->parent_->right_ = y;
    } else {
        x->parent_->left_ = y;
    }
    y->right_ = x;
    x->parent_ = y;
}

// 查找最小节点
template <typename Key, class Comparator>
typename RBTree<Key, Comparator>::Node* RBTree<Key, Comparator>::minimum(Node* x) {
    while (x->left_ != nullptr) {
        x = x->left_;
    }
    return x;
}

// 节点替换
template <typename Key, class Comparator>
void RBTree<Key, Comparator>::transplant(Node* u, Node* v) {
    if (u->parent_ == nullptr) {
        root_ = v;
    } else if (u == u->parent_->left_) {
        u->parent_->left_ = v;
    } else {
        u->parent_->right_ = v;
    }
    if (v != nullptr) {
        v->parent_ = u->parent_;
    }
}
template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::deleteNode(const Key &k) {
    Node* z;
    if (!findNode(k, &z)) {
        return false;
    }

    Node* y = z;
    Color y_original_color = y->color_;
    Node* x;

    if (z->left_ == nullptr) {
        x = z->right_;
        transplant(z, z->right_);
    } else if (z->right_ == nullptr) {
        x = z->left_;
        transplant(z, z->left_);
    } else {
        y = minimum(z->right_);
        y_original_color = y->color_;
        x = y->right_;
        if (y->parent_ == z) {
            if (x != nullptr) x->parent_ = y;
        } else {
            transplant(y, y->right_);
            y->right_ = z->right_;
            y->right_->parent_ = y;
        }
        transplant(z, y);
        y->left_ = z->left_;
        y->left_->parent_ = y;
        y->color_ = z->color_;
    }

    delete z;
    if (y_original_color == BLACK && x != nullptr) {
        deleteFixup(x);
    }
    return true;
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::deleteFixup(Node* x) {
    while (x != root_ && (x == nullptr || x->color_ == BLACK)) {
        if (x == x->parent_->left_) {
            Node* w = x->parent_->right_;
            if (w->color_ == RED) {
                w->color_ = BLACK;
                x->parent_->color_ = RED;
                leftRotate(x->parent_);
                w = x->parent_->right_;
            }
            if ((w->left_ == nullptr || w->left_->color_ == BLACK) &&
                (w->right_ == nullptr || w->right_->color_ == BLACK)) {
                w->color_ = RED;
                x = x->parent_;
            } else {
                if (w->right_ == nullptr || w->right_->color_ == BLACK) {
                    w->left_->color_ = BLACK;
                    w->color_ = RED;
                    rightRotate(w);
                    w = x->parent_->right_;
                }
                w->color_ = x->parent_->color_;
                x->parent_->color_ = BLACK;
                w->right_->color_ = BLACK;
                leftRotate(x->parent_);
                x = root_;
            }
        } else {
            Node* w = x->parent_->left_;
            if (w->color_ == RED) {
                w->color_ = BLACK;
                x->parent_->color_ = RED;
                rightRotate(x->parent_);
                w = x->parent_->left_;
            }
            if ((w->right_ == nullptr || w->right_->color_ == BLACK) &&
                (w->left_ == nullptr || w->left_->color_ == BLACK)) {
                w->color_ = RED;
                x = x->parent_;
            } else {
                if (w->left_ == nullptr || w->left_->color_ == BLACK) {
                    w->right_->color_ = BLACK;
                    w->color_ = RED;
                    leftRotate(w);
                    w = x->parent_->left_;
                }
                w->color_ = x->parent_->color_;
                x->parent_->color_ = BLACK;
                w->left_->color_ = BLACK;
                rightRotate(x->parent_);
                x = root_;
            }
        }
    }
    if (x != nullptr) x->color_ = BLACK;
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Contains(const Key &key) const {
    return findNode(key, nullptr);
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Insert(const Key& key) {
    Node* n_node = nullptr;
    auto ret = insertNode(key, n_node);
    return ret;
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Remove(const Key& key) {
    return deleteNode(key);
}
