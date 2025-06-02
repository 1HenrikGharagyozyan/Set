#include <cassert>
#include <iostream>
#include "Set.h"

void test_insert_and_contains()
{
	Set<int> s;
	auto [it1, inserted1] = s.insert(10);
	auto [it2, inserted2] = s.insert(5);
	auto [it3, inserted3] = s.insert(15);
	auto [it4, inserted4] = s.insert(10); // дубликат

	assert(inserted1);
	assert(inserted2);
	assert(inserted3);
	assert(!inserted4); // дубликат не должен вставиться

	assert(s.contains(10));
	assert(s.contains(5));
	assert(s.contains(15));
	assert(!s.contains(20));
}

void test_erase() 
{
	Set<int> s;
	s.insert(1);
	s.insert(2);
	s.insert(3);

	assert(s.contains(2));
	s.erase(2);
	assert(!s.contains(2));

	auto it = s.find(3);
	s.erase(it);
	assert(!s.contains(3));
}

void test_iterators() {
	Set<int> s;
	s.insert(3);
	s.insert(1);
	s.insert(2);

	int expected[] = { 1, 2, 3 };
	int i = 0;
	for (const auto& pair : s) {
		int value = pair.first;
		std::cout << value << '\n';
		assert(value == expected[i]);
		++i;
	}
	assert(i == 3); // теперь всё ок
}

void test_copy_and_move() 
{
	Set<int> original = { 1, 2, 3 };
	Set<int> copy = original;
	Set<int> moved = std::move(original);

	assert(copy.contains(1));
	assert(copy.contains(2));
	assert(copy.contains(3));

	assert(moved.contains(1));
	assert(moved.contains(2));
	assert(moved.contains(3));
	std::cout << 2;
}

void test_equal_operator() 
{
	Set<int> a = { 1, 2, 3 };
	Set<int> b = { 1, 2, 3 };
	Set<int> c = { 4, 5 };

	assert(a == b);
	assert(!(a != b));
	assert(a != c);
}

int main() 
{
	test_insert_and_contains();
	test_erase();
	test_iterators();
	test_copy_and_move();
	test_equal_operator();

	std::cout << "\n\n" << "All tests passed!\n";
	return 0;
}
