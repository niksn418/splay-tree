#include "SplayTree.h"

struct SplayTree::Node
{
    Node * left = nullptr;
    Node * right = nullptr;
    int value;

    Node() = default;

    Node(int value)
        : value(value)
    {
    }

    void set_children(Node * left, Node * right)
    {
        this->left = left;
        this->right = right;
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
    if (v == nullptr) {
        return;
    }
    static Node dummy;
    Node * left_tree = &dummy;
    Node * right_tree = &dummy;
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
    splay(m_root, value);
    return m_root != nullptr && m_root->value == value;
}

bool SplayTree::insert(int value)
{
    if (contains(value)) {
        return false;
    }
    if (m_root == nullptr) {
        m_root = new Node(value);
        m_size = 1;
        return true;
    }
    Node * new_root = new Node(value);
    if (value < m_root->value) {
        new_root->set_children(m_root->left, m_root);
        m_root->left = nullptr;
    }
    else {
        new_root->set_children(m_root, m_root->right);
        m_root->right = nullptr;
    }
    m_root = new_root;
    ++m_size;
    return true;
}

bool SplayTree::remove(int value)
{
    if (!contains(value)) {
        return false;
    }
    Node * new_root;
    if (m_root->left == nullptr) {
        new_root = m_root->right;
    }
    else if (m_root->right == nullptr) {
        new_root = m_root->left;
    }
    else {
        new_root = m_root->left;
        splay(new_root, value);
        new_root->right = m_root->right;
    }
    --m_size;
    delete m_root;
    m_root = new_root;
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
    values.reserve(m_size);
    get_values(m_root, values);
    return values;
}

SplayTree::~SplayTree()
{
    while (m_root != nullptr) {
        if (m_root->left != nullptr) {
            m_root = m_root->rotate_right();
        }
        else {
            Node * old_root = m_root;
            m_root = m_root->right;
            delete old_root;
        }
    }
}
