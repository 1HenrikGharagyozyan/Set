#pragma once

#include <initializer_list>
#include <utility>
#include <functional>

#include "RedBlackTree.h"

template<typename Key, typename Compare = std::less<Key>>
class Set
{
private:
	using Tree = RedBlackTree<Key, EmptyStruct, Compare>;
	Tree _tree;

public:
	using key_type = Key;
	using value_type = Key;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using key_compare = Compare;
	using value_compare = Compare;

	using iterator = typename Tree::iterator;
	using const_iterator = typename Tree::const_iterator;
	using reverse_iterator = typename Tree::reverse_iterator;
	using const_reverse_iterator = typename Tree::const_reverse_iterator;


	Set();
	explicit Set(const Compare& comp);
	Set(std::initializer_list<Key> init);
	Set(const Set& other);
	Set(Set&& other) noexcept;
	~Set();

	template<typename InputIt>
	Set(InputIt first, InputIt last);

	Set& operator=(const Set& other);
	Set& operator=(Set&& other);

	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	bool empty() const noexcept;
	size_type size() const noexcept;
	void clear() noexcept;

	std::pair<iterator, bool> insert(const Key& key);
	std::pair<iterator, bool> insert(Key&& key);

	template<typename... Args>
	std::pair<iterator, bool> emplace(Args&&... args);

	size_type erase(const Key& key);
	void erase(iterator pos);

	iterator find(const Key& key);
	const_iterator find(const Key& key) const;

	bool contains(const Key& key) const;

	iterator lower_bound(const Key& key);
	const_iterator lower_bound(const Key& key) const;

	iterator upper_bound(const Key& key);
	const_iterator upper_bound(const Key& key) const;

	std::pair<iterator, iterator> equal_range(const Key& key);
	std::pair<const_iterator, const_iterator> equal_range(const Key& key) const;

	template<typename K, typename C>
	friend bool operator==(const Set<K, C>& lhs, const Set<K, C>& rhs);

	template<typename K, typename C>
	friend bool operator!=(const Set<K, C>& lhs, const Set<K, C>& rhs);

	template<typename K, typename C>
	friend void swap(Set<K, C>& lhs, Set<K, C>& rhs) noexcept;
};

template<typename Key, typename Compare>
inline Set<Key, Compare>::Set() = default;

template<typename Key, typename Compare>
inline Set<Key, Compare>::Set(const Compare& comp) 
	: _tree(Tree(comp))
{
}

template<typename Key, typename Compare>
inline Set<Key, Compare>::Set(std::initializer_list<Key> init)
	: _tree(init)
{
}

template<typename Key, typename Compare>
inline Set<Key, Compare>::Set(const Set& other) = default;

template<typename Key, typename Compare>
inline Set<Key, Compare>::Set(Set&& other) noexcept = default;

template<typename Key, typename Compare>
inline Set<Key, Compare>::~Set() = default;

template<typename Key, typename Compare>
inline Set<Key, Compare>& Set<Key, Compare>::operator=(const Set& other) = default;

template<typename Key, typename Compare>
inline Set<Key, Compare>& Set<Key, Compare>::operator=(Set&& other) = default;

template<typename Key, typename Compare>
template<typename InputIt>
inline Set<Key, Compare>::Set(InputIt first, InputIt last)
{
	for (; first != last; ++first)
		_tree.insert(*first);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::iterator Set<Key, Compare>::begin() noexcept
{
	return _tree.begin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::iterator Set<Key, Compare>::end() noexcept
{
	return _tree.end();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::begin() const noexcept
{
	return _tree.begin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::end() const noexcept
{
	return _tree.end();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::cbegin() const noexcept
{
	return _tree.cbegin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::cend() const noexcept
{
	return _tree.cend();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::reverse_iterator Set<Key, Compare>::rbegin() noexcept
{
	return _tree.rbegin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::reverse_iterator Set<Key, Compare>::rend() noexcept
{
	return _tree.rend();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_reverse_iterator Set<Key, Compare>::rbegin() const noexcept
{
	return _tree.rbegin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_reverse_iterator Set<Key, Compare>::rend() const noexcept
{
	return _tree.rend();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_reverse_iterator Set<Key, Compare>::crbegin() const noexcept
{
	return _tree.crbegin();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_reverse_iterator Set<Key, Compare>::crend() const noexcept
{
	return _tree.crend();
}

template<typename Key, typename Compare>
bool Set<Key, Compare>::empty() const noexcept
{
	return _tree.empty();
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::size_type Set<Key, Compare>::size() const noexcept
{
	return _tree.size();
}

template<typename Key, typename Compare>
void Set<Key, Compare>::clear() noexcept
{
	_tree.clear();
}

template<typename Key, typename Compare>
std::pair<typename Set<Key, Compare>::iterator, bool> Set<Key, Compare>::insert(const Key& key)
{
	return _tree.insert(key);
}

template<typename Key, typename Compare>
inline std::pair<typename Set<Key, Compare>::iterator, bool> Set<Key, Compare>::insert(Key&& key)
{
	return _tree.insert(std::move(key));
}

template<typename Key, typename Compare>
template<typename... Args>
std::pair<typename Set<Key, Compare>::iterator, bool> Set<Key, Compare>::emplace(Args&&... args)
{
	return _tree.emplace(std::forward<Args>(args)...);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::size_type Set<Key, Compare>::erase(const Key& key)
{
	return _tree.erase(key) ? 1 : 0;
}

template<typename Key, typename Compare>
void Set<Key, Compare>::erase(iterator pos)
{
	if (pos != end())
		_tree.erase(pos->first);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::iterator Set<Key, Compare>::find(const Key& key)
{
	return _tree.find(key);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::find(const Key& key) const
{
	return _tree.find(key);
}

template<typename Key, typename Compare>
bool Set<Key, Compare>::contains(const Key& key) const
{
	return _tree.contains(key);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::iterator Set<Key, Compare>::lower_bound(const Key& key)
{
	return _tree.lower_bound(key);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::lower_bound(const Key& key) const
{
	return _tree.lower_bound(key);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::iterator Set<Key, Compare>::upper_bound(const Key& key)
{
	return _tree.upper_bound(key);
}

template<typename Key, typename Compare>
typename Set<Key, Compare>::const_iterator Set<Key, Compare>::upper_bound(const Key& key) const
{
	return _tree.upper_bound(key);
}

template<typename Key, typename Compare>
std::pair<typename Set<Key, Compare>::iterator, typename Set<Key, Compare>::iterator> Set<Key, Compare>::equal_range(const Key& key)
{
	return _tree.equal_range(key);
}

template<typename Key, typename Compare>
std::pair<typename Set<Key, Compare>::const_iterator, typename Set<Key, Compare>::const_iterator> 
		Set<Key, Compare>::equal_range(const Key& key) const
{
	return _tree.equal_range(key);
}

template<typename K, typename C>
inline bool operator==(const Set<K, C>& lhs, const Set<K, C>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename K, typename C>
inline bool operator!=(const Set<K, C>& lhs, const Set<K, C>& rhs)
{
	return !(lhs == rhs);
}

template<typename K, typename C>
inline void swap(Set<K, C>& lhs, Set<K, C>& rhs) noexcept
{
	std::swap(lhs._tree, rhs._tree);
}
