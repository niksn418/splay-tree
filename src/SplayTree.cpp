#include "SplayTree.h"

#include <vector>

struct SplayTree::Node
{
    Node * left = nullptr;
    Node * right = nullptr;
    int value;

    Node() = default;

    Node(int _value)
        : value(_value)
    {
    }

    void set_children(Node * _left, Node * _right)
    {
        left = _left;
        right = _right;
    }

    Node * rotate_left()
    {
        Node * temp = right;
        right = temp->left;
        temp->left = this;
        return temp;
    }

    Node * rotate_right()
    {
        Node * temp = left;
        left = temp->right;
        temp->right = this;
        return temp;
    }
};

void SplayTree::splay(Node *& v, int value) const
{
    if (v == nullptr)
        return;
    Node *left_tree, *right_tree;
    static Node dummy;
    left_tree = right_tree = &dummy;
    while (v->value != value) {
        if (value < v->value) {
            if (v->left != nullptr && value < v->left->value) {
                v = v->rotate_right();
            }
            if (v->left == nullptr) {
                break;
            }
            right_tree->left = v;
            right_tree = v;
            v = v->left;
        }
        else {
            if (v->right != nullptr && value > v->right->value) {
                v = v->rotate_left();
            }
            if (v->right == nullptr) {
                break;
            }
            left_tree->right = v;
            left_tree = v;
            v = v->right;
        }
    }
    left_tree->right = v->left;
    right_tree->left = v->right;
    v->set_children(dummy.right, dummy.left);
}

void SplayTree::get_values(const Node * v, std::vector<int> & values) const
{
    if (v == nullptr) {
        return;
    }
    get_values(v->left, values);
    values.push_back(v->value);
    get_values(v->right, values);
}

bool SplayTree::contains(int value) const
{
    splay(root, value);
    return root != nullptr && root->value == value;
}

bool SplayTree::insert(int value)
{
    if (contains(value)) {
        return false;
    }
    if (root == nullptr) {
        root = new Node(value);
        m_size = 1;
        return true;
    }
    Node * new_root = new Node(value);
    if (value < root->value) {
        new_root->set_children(root->left, root);
        root->left = nullptr;
    }
    else {
        new_root->set_children(root, root->right);
        root->right = nullptr;
    }
    root = new_root;
    ++m_size;
    return true;
}

bool SplayTree::remove(int value)
{
    if (!contains(value)) {
        return false;
    }
    Node * new_root;
    if (root->left == nullptr) {
        new_root = root->right;
    }
    else if (root->right == nullptr) {
        new_root = root->left;
    }
    else {
        new_root = root->left;
        splay(new_root, value);
        new_root->right = root->right;
    }
    --m_size;
    delete root;
    root = new_root;
    return true;
}

std::size_t SplayTree::size() const
{
    return m_size;
}

bool SplayTree::empty() const
{
    return m_size == 0;
}

std::vector<int> SplayTree::values() const
{
    std::vector<int> values;
    get_values(root, values);
    return values;
}

SplayTree::~SplayTree()
{
    while (m_size) {
        remove(root->value);
    }
}
