#pragma once

#include <vector>

class SplayTree
{
    struct Node;
    mutable Node * root = nullptr;
    std::size_t m_size = 0;

    void splay(Node *& v, int value) const;

    void get_values(const Node * v, std::vector<int> & values) const;

public:
    bool contains(int value) const;
    bool insert(int value);
    bool remove(int value);

    std::size_t size() const;
    bool empty() const;

    std::vector<int> values() const;

    ~SplayTree();
};
