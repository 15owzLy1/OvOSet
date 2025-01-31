#include "rb_tree.h"

template <typename Key, class Comparator>
RBTree<Key, Comparator>::RBTree() : root(nullptr) {}

template <typename Key, class Comparator>
RBTree<Key, Comparator>::~RBTree() {
    clear(root);
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::rotateLeft(Node*& p, Node*& pt) {
    Node* ptRight = pt->right;
    pt->right = ptRight->left;

    if (pt->right != nullptr) {
        pt->right->parent = pt;
    }

    ptRight->parent = pt->parent;

    if (pt->parent == nullptr) {
        p = ptRight;
    } else if (pt == pt->parent->left) {
        pt->parent->left = ptRight;
    } else {
        pt->parent->right = ptRight;
    }

    ptRight->left = pt;
    pt->parent = ptRight;
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::rotateRight(Node*& p, Node*& pt) {
    Node* ptLeft = pt->left;
    pt->left = ptLeft->right;

    if (pt->left != nullptr) {
        pt->left->parent = pt;
    }

    ptLeft->parent = pt->parent;

    if (pt->parent == nullptr) {
        p = ptLeft;
    } else if (pt == pt->parent->right) {
        pt->parent->right = ptLeft;
    } else {
        pt->parent->left = ptLeft;
    }

    ptLeft->right = pt;
    pt->parent = ptLeft;
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::fixInsert(Node*& p, Node*& pt) {
    Node* parentPt = nullptr;
    Node* grandParentPt = nullptr;

    while ((pt != p) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parentPt = pt->parent;
        grandParentPt = pt->parent->parent;

        if (parentPt == grandParentPt->left) {
            Node* unclePt = grandParentPt->right;

            if (unclePt != nullptr && unclePt->color == RED) {
                grandParentPt->color = RED;
                parentPt->color = BLACK;
                unclePt->color = BLACK;
                pt = grandParentPt;
            } else {
                if (pt == parentPt->right) {
                    rotateLeft(p, parentPt);
                    pt = parentPt;
                    parentPt = pt->parent;
                }

                rotateRight(p, grandParentPt);
                std::swap(parentPt->color, grandParentPt->color);
                pt = parentPt;
            }
        } else {
            Node* unclePt = grandParentPt->left;

            if (unclePt != nullptr && unclePt->color == RED) {
                grandParentPt->color = RED;
                parentPt->color = BLACK;
                unclePt->color = BLACK;
                pt = grandParentPt;
            } else {
                if (pt == parentPt->left) {
                    rotateRight(p, parentPt);
                    pt = parentPt;
                    parentPt = pt->parent;
                }

                rotateLeft(p, grandParentPt);
                std::swap(parentPt->color, grandParentPt->color);
                pt = parentPt;
            }
        }
    }

    p->color = BLACK;
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::fixDelete(Node*& p, Node*& pt) {
    while (pt != p && pt->color == BLACK) {
        if (pt == pt->parent->left) {
            Node* sibling = pt->parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateLeft(p, pt->parent);
                sibling = pt->parent->right;
            }

            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                pt = pt->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(p, sibling);
                    sibling = pt->parent->right;
                }

                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(p, pt->parent);
                pt = p;
            }
        } else {
            Node* sibling = pt->parent->left;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                pt->parent->color = RED;
                rotateRight(p, pt->parent);
                sibling = pt->parent->left;
            }

            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                pt = pt->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(p, sibling);
                    sibling = pt->parent->left;
                }

                sibling->color = pt->parent->color;
                pt->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(p, pt->parent);
                pt = p;
            }
        }
    }

    pt->color = BLACK;
}

template <typename Key, class Comparator>
typename RBTree<Key, Comparator>::Node* RBTree<Key, Comparator>::insertBST(Node* p, Node* pt) {
    if (p == nullptr) {
        return pt;
    }

    if (compare(pt->key, p->key) < 0) {
        p->left = insertBST(p->left, pt);
        p->left->parent = p;
    } else if (compare(pt->key, p->key) > 0) {
        p->right = insertBST(p->right, pt);
        p->right->parent = p;
    }

    return p;
}

template <typename Key, class Comparator>
typename RBTree<Key, Comparator>::Node* RBTree<Key, Comparator>::searchTree(Node* p, const Key& key) const {
    if (p == nullptr || compare(p->key, key) == 0) {
        return p;
    }

    if (compare(p->key, key) < 0) {
        return searchTree(p->right, key);
    }

    return searchTree(p->left, key);
}

template <typename Key, class Comparator>
typename RBTree<Key, Comparator>::Node* RBTree<Key, Comparator>::minValueNode(Node* p) {
    Node* current = p;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::transplant(Node*& p, Node* u, Node* v) {
    if (u->parent == nullptr) {
        p = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template <typename Key, class Comparator>
void RBTree<Key, Comparator>::clear(Node* p) {
    if (p == nullptr) {
        return;
    }

    clear(p->left);
    clear(p->right);
    delete p;
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Insert(const Key& key) {
    Node* pt = new Node(key);

    root = insertBST(root, pt);
    fixInsert(root, pt);
    return true;
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Remove(const Key& key) {
    Node* z = searchTree(root, key);
    if (z == nullptr) {
        return false;
    }

    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;

    if (z->left == nullptr) {
        x = z->right;
        transplant(root, z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(root, z, z->left);
    } else {
        y = minValueNode(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK) {
        fixDelete(root, x);
    }

    return true;
}

template <typename Key, class Comparator>
bool RBTree<Key, Comparator>::Find(const Key& key) const {
    Node* result = searchTree(root, key);
    return result != nullptr;
}
