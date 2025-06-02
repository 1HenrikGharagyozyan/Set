#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <stack>
#include <utility>
#include <iterator>
#include <memory>
#include <initializer_list>
#include <queue>

struct EmptyStruct {};

inline bool operator==(const EmptyStruct&, const EmptyStruct&)
{
    return true;
}

inline bool operator!=(const EmptyStruct&, const EmptyStruct&)
{
    return false;
}



template<typename Key, typename T = EmptyStruct, typename Compare = std::less<Key>, bool AllowDuplicates = false>
class RedBlackTree
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    enum Color
    {
        RED,
        BLACK
    };

    struct Node
    {
        value_type data;
        Node* parent;
        Node* left;
        Node* right;
        Color color;

        Node(const Key& k = Key{}, const T& val = T{}, Color c = BLACK, Node* p = nullptr)
            : data(std::make_pair(k, val))
            , parent(p)
            , left(nullptr)
            , right(nullptr)
            , color(c)
        {
        }

        Node(Key&& k, T&& val, Color c = BLACK, Node* p = nullptr)
            : data(std::make_pair(std::move(k), std::move(val)))
            , parent(p)
            , left(nullptr)
            , right(nullptr)
            , color(c)
        {
        }
    };

    Node* create_node(const Key& key, const T& value, Color color = RED, Node* parent = nullptr)
    {
        Node* node = new Node(key, value, color, parent);
        node->left = node->right = _nil;
        return node;
    }

    Node* create_node(Key&& key, T&& value, Color color = RED, Node* parent = nullptr)
    {
        Node* node = new Node(std::move(key), std::move(value), color, parent);
        node->left = node->right = _nil;
        return node;
    }


    Node* _root;
    Node* _nil;
    size_t _tree_size;
    Compare _comp;

public:
    class Iterator
    {
    private:
        Node* _node;
        Node* _nil;
        Node* _root;

        Node* minimum(Node* x) const;
        Node* maximum(Node* x) const;

        Node* successor(Node* x) const;
        Node* predecessor(Node* x) const;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename RedBlackTree::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(Node* node = nullptr, Node* nil = nullptr, Node* root = nullptr);

        reference operator*() const;
        pointer operator->() const;

        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        Node* node() const;
    };


    class ConstIterator
    {
    private:
        Iterator _it;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename RedBlackTree::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        ConstIterator(Node* node = nullptr, Node* nil = nullptr, Node* root = nullptr);

        reference operator*() const;
        pointer operator->() const;

        ConstIterator& operator++();
        ConstIterator operator++(int);

        ConstIterator& operator--();
        ConstIterator operator--(int);

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;
    };


    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    RedBlackTree();
    RedBlackTree(const RedBlackTree& other);
    RedBlackTree(RedBlackTree&& other) noexcept;
    RedBlackTree(std::initializer_list<value_type> init);
    ~RedBlackTree();

    template<typename U = T>
    explicit RedBlackTree(std::initializer_list<key_type> init, std::enable_if_t<std::is_same<U, EmptyStruct>::value>* = nullptr);


    RedBlackTree& operator=(const RedBlackTree& other);
    RedBlackTree& operator=(RedBlackTree&& other) noexcept;

    size_type size() const;
    size_type height() const;
    bool empty() const;

    void clear();

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    bool contains(const Key& key) const;

    iterator lower_bound(const Key& key);
    const_iterator lower_bound(const Key& key) const;

    iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    std::pair<iterator, iterator> equal_range(const Key& key);
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

    std::pair<iterator, bool> insert(const value_type& val);
    std::pair<iterator, bool> insert(value_type&& val);

    bool erase(const Key& key);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    template<typename U = T>
    std::enable_if_t<std::is_same<U, EmptyStruct>::value, std::pair<iterator, bool>> insert(const Key& key);

    template<typename U = T>
    std::enable_if_t<std::is_same<U, EmptyStruct>::value, std::pair<iterator, bool>> emplace(const Key& key);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    std::vector<value_type> inorder() const;
    std::vector<value_type> preorder() const;
    std::vector<value_type> postorder() const;
    std::vector<value_type> levelorder() const;

    void inorder(std::function<void(const_reference)> visit) const;
    void preorder(std::function<void(const_reference)> visit) const;
    void postorder(std::function<void(const_reference)> visit) const;
    void levelorder(std::function<void(const_reference)> visit) const;

    bool operator==(const RedBlackTree& other) const;
    bool operator!=(const RedBlackTree& other) const;


private:
    Node* minimum(Node* x) const;
    Node* maximum(Node* x) const;

    void rotate_left(Node* x);
    void rotate_right(Node* x);

    void clear_helper(Node* node);

    void insert_fix(Node* z);

    void transplant(Node* u, Node* v);
    void delete_node(Node* z);
    void erase_fix(Node* x);

    Node* find_helper(const Key& key) const;

    void copy_helper(const Node* node, const Node* source_nil);

    void inorder_helper(Node* node, std::function<void(const_reference)> visit) const;
    void preorder_helper(Node* node, std::function<void(const_reference)> visit) const;
    void postorder_helper(Node* node, std::function<void(const_reference)> visit) const;
};

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::RedBlackTree()
    : _tree_size(0)
    , _comp(Compare())
{
    _nil = new Node();
    _nil->color = BLACK;
    _nil->left = _nil->right = _nil->parent = nullptr;
    _root = _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::RedBlackTree(const RedBlackTree& other)
    : RedBlackTree()
{
    copy_helper(other._root, other._nil);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::RedBlackTree(RedBlackTree&& other) noexcept
    : _root(other._root)
    , _nil(other._nil)
    , _tree_size(other._tree_size)
    , _comp(std::move(other._comp))
{
    // Восстанавливаем other в пустое валидное состояние
    other._nil = new Node(); // создаём новый nil-узел
    other._nil->color = BLACK;
    other._nil->left = other._nil->right = other._nil->parent = nullptr;

    other._root = other._nil;  // указывает на пустое дерево
    other._tree_size = 0;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::RedBlackTree(std::initializer_list<value_type> init)
    : RedBlackTree()
{
    for (const auto& elem : init)
        insert(elem);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::~RedBlackTree()
{
    clear();
    delete _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>& 
        RedBlackTree<Key, T, Compare, AllowDuplicates>::operator=(const RedBlackTree& other)
{
    if (this != &other)
    {
        clear();
        copy_helper(other._root, other._nil);
    }
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>& 
        RedBlackTree<Key, T, Compare, AllowDuplicates>::operator=(RedBlackTree&& other) noexcept
{
    if (this != &other)
    {
        clear();      // удалить текущие узлы
        delete _nil;  // удалить текущий sentinel

        _root = other._root;
        _nil = other._nil;
        _tree_size = other._tree_size;
        _comp = std::move(other._comp);

        // Восстанавливаем other
        other._nil = new Node();
        other._nil->color = BLACK;
        other._nil->left = other._nil->right = other._nil->parent = nullptr;

        other._root = other._nil;
        other._tree_size = 0;
    }
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::size_type RedBlackTree<Key, T, Compare, AllowDuplicates>::size() const
{
    return _tree_size;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::size_type RedBlackTree<Key, T, Compare, AllowDuplicates>::height() const
{
    std::function<size_type(Node*)> dfs = [&](Node* node) -> size_type
        {
            if (node == _nil)
                return 0;
            size_type lh = dfs(node->left);
            size_type rh = dfs(node->right);
            return 1 + std::max(lh, rh);
        };
    return dfs(_root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::empty() const
{
    return _tree_size == 0;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::clear()
{
    clear_helper(_root);
    _root = _nil;
    _tree_size = 0;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::find(const Key& key)
{
    Node* result = find_helper(key);
    return (result != _nil) ? iterator(result, _nil, _root) : end();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::find(const Key& key) const
{
    Node* result = find_helper(key);
    return (result != _nil) ? const_iterator(result, _nil, _root) : end();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::contains(const Key& key) const
{
    return find_helper(key) != _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::lower_bound(const Key& key)
{
    Node* current = _root;
    Node* result = _nil;

    while (current != _nil)
    {
        if (!_comp(current->data.first, key))
        {
            result = current;
            current = current->left;
        }
        else
            current = current->right;
    }
    return iterator(result, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::lower_bound(const Key& key) const
{
    Node* current = _root;
    Node* result = _nil;

    while (current != _nil)
    {
        if (!_comp(current->data.first, key))
        {
            result = current;
            current = current->left;
        }
        else
            current = current->right;
    }
    return const_iterator(result, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::upper_bound(const Key& key)
{
    Node* current = _root;
    Node* result = _nil;

    while (current != _nil)
    {
        if (_comp(key, current->data.first))
        {
            result = current;
            current = current->left;
        }
        else
            current = current->right;
    }
    return iterator(result, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::upper_bound(const Key& key) const
{
    Node* current = _root;
    Node* result = _nil;

    while (current != _nil)
    {
        if (_comp(key, current->data.first))
        {
            result = current;
            current = current->left;
        }
        else
            current = current->right;
    }
    return const_iterator(result, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, typename RedBlackTree
    <Key, T, Compare, AllowDuplicates>::iterator> RedBlackTree<Key, T, Compare, AllowDuplicates>::equal_range(const Key& key)
{
    return { lower_bound(key), upper_bound(key) };
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator, 
    typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator> 
    RedBlackTree<Key, T, Compare, AllowDuplicates>::equal_range(const Key& key) const
{
    return { lower_bound(key), upper_bound(key) };
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
        std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, bool> 
        RedBlackTree<Key, T, Compare, AllowDuplicates>::insert(const value_type& val)
{
    Node* z = create_node(val.first, val.second);
    Node* y = _nil;
    Node* x = _root;

    while (x != _nil)
    {
        y = x;
        if (_comp(z->data.first, x->data.first))
            x = x->left;
        else if (_comp(x->data.first, z->data.first))
            x = x->right;
        else if constexpr (!AllowDuplicates)
        {
            delete z;
            return { iterator(x, _nil), false };
        }
        else
            x = x->right;
    }

    z->parent = y;
    if (y == _nil)
        _root = z;
    else if (_comp(z->data.first, y->data.first))
        y->left = z;
    else
        y->right = z;

    insert_fix(z);
    ++_tree_size;
    return { iterator(z, _nil), true };
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
        std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, bool>
        RedBlackTree<Key, T, Compare, AllowDuplicates>::insert(value_type&& val)
{
    Node* z = create_node(std::move(val.first), std::move(val.second));
    Node* y = _nil;
    Node* x = _root;

    while (x != _nil)
    {
        y = x;
        if (_comp(z->data.first, x->data.first))
            x = x->left;
        else if (_comp(x->data.first, z->data.first))
            x = x->right;
        else if constexpr (!AllowDuplicates)
        {
            delete z;
            return { iterator(x, _nil), false };
        }
        else
            x = x->right;
    }

    z->parent = y;
    if (y == _nil)
        _root = z;
    else if (_comp(z->data.first, y->data.first))
        y->left = z;
    else
        y->right = z;

    insert_fix(z);
    ++_tree_size;
    return { iterator(z, _nil), true };
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::erase(const Key& key)
{
    Node* z = find_helper(key);
    if (z == _nil)
        return false;

    delete_node(z);
    --_tree_size;
    return true;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::begin()
{
    return iterator(minimum(_root), _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::end()
{
    return iterator(_nil, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::begin() const
{
    return const_iterator(minimum(_root), _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::end() const
{
    return const_iterator(_nil, _nil, _root);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::cbegin() const
{
    return begin();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::cend() const
{
    return end();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::reverse_iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::rbegin()
{
    return reverse_iterator(end());
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::reverse_iterator RedBlackTree<Key, T, Compare, AllowDuplicates>::rend()
{
    return reverse_iterator(begin());
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_reverse_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_reverse_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::rend() const
{
    return const_reverse_iterator(begin());
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_reverse_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::crbegin() const
{
    return rbegin();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::const_reverse_iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::crend() const
{
    return rend();
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::vector<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::value_type> 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::inorder() const
{
    std::vector<value_type> result;
    inorder([&](const_reference value) { result.push_back(value); });
    return result;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::vector<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::value_type> 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::preorder() const
{
    std::vector<value_type> result;
    preorder([&](const_reference value) { result.push_back(value); });
    return result;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::vector<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::value_type> 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::postorder() const
{
    std::vector<value_type> result;
    postorder([&](const_reference value) { result.push_back(value); });
    return result;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline std::vector<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::value_type> 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::levelorder() const
{
    std::vector<value_type> result;
    levelorder([&](const_reference value) { result.push_back(value); });
    return result;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::minimum(Node* x) const
{
    while (x != _nil && x->left != _nil)
        x = x->left;
    return x;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::maximum(Node* x) const
{
    while (x != _nil && x->right != _nil)
        x = x->right;
    return x;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::clear_helper(Node* node)
{
    if (node == _nil)
        return;
    clear_helper(node->left);
    clear_helper(node->right);
    delete node;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::insert_fix(Node* z)
{
    while (z->parent && z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            Node* y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rotate_left(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(z->parent->parent);
            }
        }
        else
        {
            Node* y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rotate_right(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(z->parent->parent);
            }
        }
    }
    _root->color = BLACK;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::transplant(Node* u, Node* v)
{
    if (u->parent == _nil)
        _root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::delete_node(Node* z)
{
    Node* y = z;
    Node* x;
    Color original_color = y->color;

    if (z->left == _nil)
    {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == _nil)
    {
        x = z->left;
        transplant(z, z->left);
    }
    else
    {
        y = minimum(z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z)
            x->parent = y;
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (original_color == BLACK)
        erase_fix(x);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::erase_fix(Node* x)
{
    while (x != _root && x->color == BLACK)
    {
        if (x == x->parent->left)
        {
            Node* w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(x->parent);
                x = _root;
            }
        }
        else
        {
            Node* w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(x->parent);
                x = _root;
            }
        }
    }

    x->color = BLACK;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::find_helper(const Key& key) const
{
    Node* current = _root;
    while (current != _nil)
    {
        if (_comp(key, current->data.first))
            current = current->left;
        else if (_comp(current->data.first, key))
            current = current->right;
        else
            return current;
    }
    return _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::copy_helper(const Node* node, const Node* source_nil)
{
    if (node == source_nil)
        return;
    insert(node->data);
    copy_helper(node->left, source_nil);
    copy_helper(node->right, source_nil);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::inorder_helper(Node* node, std::function<void(const_reference)> visit) const
{
    if (node == _nil)
        return;
    inorder_helper(node->left, visit);
    visit(node->data);
    inorder_helper(node->right, visit);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::preorder_helper(Node* node, 
                std::function<void(const_reference)> visit) const
{
    if (node == _nil)
        return;
    visit(node->data);
    preorder_helper(node->left, visit);
    preorder_helper(node->right, visit);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::postorder_helper(Node* node, 
                std::function<void(const_reference)> visit) const
{
    if (node == _nil)
        return;
    postorder_helper(node->left, visit);
    postorder_helper(node->right, visit);
    visit(node->data);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::rotate_left(Node* x)
{
    Node* y = x->right;
    x->right = y->left;
    if (y->left != _nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == _nil)
        _root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::rotate_right(Node* x)
{
    Node* y = x->left;
    x->left = y->right;
    if (y->right != _nil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == _nil)
        _root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::minimum(Node* x) const
{
    while (x->left != _nil)
        x = x->left;
    return x;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::maximum(Node* x) const
{
    while (x->right != _nil)
        x = x->right;
    return x;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::successor(Node* x) const
{
    if (x->right != _nil)
        return minimum(x->right);
    Node* y = x->parent;
    while (y != nullptr && x == y->right)
    {
        x = y;
        y = y->parent;
    }
    return y ? y : _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::predecessor(Node* x) const
{
    if (x->left != _nil)
        return maximum(x->left);
    Node* y = x->parent;
    while (y != nullptr && x == y->left)
    {
        x = y;
        y = y->parent;
    }
    return y ? y : _nil;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::Iterator(Node* node, Node* nil, Node* root)
    : _node(node)
    , _nil(nil)
    , _root(root)
{
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::reference 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator*() const
{
    return _node->data;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::pointer 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator->() const
{
    return &_node->data;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::Iterator& 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator++()
{
    _node = successor(_node);
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::Iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::Iterator& 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator--()
{
    if (_node == _nil)
        _node = maximum(_root);
    else
        _node = predecessor(_node);
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::Iterator 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator--(int)
{
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator==(const Iterator& other) const
{
    return _node == other._node;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::operator!=(const Iterator& other) const
{
    return _node != other._node;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::Node* 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::Iterator::node() const
{
    return _node;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::ConstIterator(Node* node, Node* nil, Node* root)
    : _it(node, nil, root)
{
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::reference 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator*() const
{
    return *_it;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::pointer 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator->() const
{
    return &this->_node->data;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::ConstIterator& 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator++()
{
    ++_it;
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::ConstIterator
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::ConstIterator& 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator--()
{
    if (this->_node == _nil)
        this->_node = maximum(this->_root);
    else
        this->_node = predecessor(this->_node);
    return *this;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
typename RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::ConstIterator
            RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator--(int)
{
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator==(const ConstIterator& other) const
{
    return _it == other._it;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return _it != other._it;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::inorder(std::function<void(const_reference)> visit) const
{
    inorder_helper(_root, visit);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::preorder(std::function<void(const_reference)> visit) const
{
    preorder_helper(_root, visit);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::postorder(std::function<void(const_reference)> visit) const
{
    postorder_helper(_root, visit);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline void RedBlackTree<Key, T, Compare, AllowDuplicates>::levelorder(std::function<void(const_reference)> visit) const
{
    if (_root == _nil)
        return;
    std::queue<Node*> q;
    q.push(_root);

    while (!q.empty())
    {
        Node* node = q.front();
        q.pop();
        visit(node->data);

        if (node->left != _nil)
            q.push(node->left);
        if (node->right != _nil)
            q.push(node->right);
    }
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::operator==(const RedBlackTree& other) const
{
    if (this == &other)
        return true;

    if (size() != other.size())
        return false;

    auto it1 = begin();
    auto it2 = other.begin();
    auto end1 = end();

    for (; it1 != end1; ++it1, ++it2)
    {
        if (*it1 != *it2)
            return false;
    }
    return true;
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
inline bool RedBlackTree<Key, T, Compare, AllowDuplicates>::operator!=(const RedBlackTree& other) const
{
    return !(*this == other);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
template<typename U>
inline RedBlackTree<Key, T, Compare, AllowDuplicates>::RedBlackTree(std::initializer_list<key_type> init,
    std::enable_if_t<std::is_same<U, EmptyStruct>::value>*)
    : RedBlackTree()
{
    for (const auto& key : init)
        insert(key);
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
template<typename ...Args>
inline std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, bool> 
            RedBlackTree<Key, T, Compare, AllowDuplicates>::emplace(Args && ...args)
{
    return insert(value_type(std::forward<Args>(args)...));
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
template<typename U>
inline std::enable_if_t<std::is_same<U, EmptyStruct>::value, 
            std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, bool>>
            RedBlackTree<Key, T, Compare, AllowDuplicates>::insert(const Key& key)
{
    return insert(std::make_pair(key, EmptyStruct{}));
}

template<typename Key, typename T, typename Compare, bool AllowDuplicates>
template<typename U>
inline std::enable_if_t<std::is_same<U, EmptyStruct>::value, 
            std::pair<typename RedBlackTree<Key, T, Compare, AllowDuplicates>::iterator, bool>>
            RedBlackTree<Key, T, Compare, AllowDuplicates>::emplace(const Key& key)
{
    return insert(std::make_pair(key, EmptyStruct{}));
}


