//
// Created by Lddyss on 2025/1/30.
//

#include "rb_tree.h"

void RBTree::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) this->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nullptr) this->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

void RBTree::fixViolation(Node* z) {
    while (z->parent && z->parent->isRed) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y && y->isRed) {
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->isRed = false;
}

void RBTree::insertFixup(Node* k) {
    fixViolation(k);
}

void RBTree::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) this->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

void RBTree::deleteNode(Node* root, int key) {
    Node* z = search(root, key);
    if (z == nullptr) return;

    Node* y = (z->left == nullptr) ? z : (z->right == nullptr) ? z : minValue(z->right);
    int yOriginalColor = y->isRed;
    Node* x = (y == nullptr) ? nullptr : (y->left ? y->left : y->right);

    if (x != nullptr) x->parent = y->parent;

    if (y->parent == nullptr) this->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    if (y != z) {
        z->key = y->key;
        z->isRed = y->isRed;
    }

    if (yOriginalColor == false) removeFixup(x);
}

void RBTree::removeFixup(Node* x) {
    while (x != this->root && !x->isRed) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->isRed) {
                w->isRed = false;
                x->parent->isRed = true;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (!w->right->isRed && !w->left->isRed) {
                w->isRed = true;
                x = x->parent;
            } else {
                if (!w->right->isRed) {
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                w->right->isRed = false;
                leftRotate(x->parent);
                x = this->root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->isRed) {
                w->isRed = false;
                x->parent->isRed = true;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (!w->left->isRed && !w->right->isRed) {
                w->isRed = true;
                x = x->parent;
            } else {
                if (!w->left->isRed) {
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->isRed = x->parent->isRed;
                x->parent->isRed = false;
                w->left->isRed = false;
                rightRotate(x->parent);
                x = this->root;
            }
        }
    }
    x->isRed = false;
}

RBTree::Node* RBTree::minValue(Node* node) {
    Node* current = node;
    while (current && current->left != nullptr) current = current->left;
    return current;
}

RBTree::Node* RBTree::search(Node* node, int key) const {
    while (node != nullptr && node->key != key) {
        if (key < node->key) node = node->left;
        else node = node->right;
    }
    return node;
}

void RBTree::Insert(int key) {
    Node* node = new Node(key);
    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr) {
        parent = current;
        if (key < current->key) current = current->left;
        else current = current->right;
    }
    node->parent = parent;
    if (parent == nullptr) root = node;
    else if (key < parent->key) parent->left = node;
    else parent->right = node;
    insertFixup(node);
}

bool RBTree::Remove(int key) {
    deleteNode(root, key);
    return true;
}

bool RBTree::Find(int key) const {
    return search(root, key) != nullptr;
}

RBTree::~RBTree() {
    // Implement destructor to delete all nodes
}