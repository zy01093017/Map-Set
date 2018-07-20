#pragma once

#include "HashTable.h"

#include <string>

template<class K>
struct _HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

template<>
struct _HashFunc<string>
{
	size_t BKDR(const char* str)
	{
		size_t num = 131;
		size_t value = 0;
		while (*str)
		{
			value = value*num + *str;
			++str;
		}

		return value;
	}

	size_t operator()(const string& s)
	{
		return BKDR(s.c_str());
	}
};


template<class K, class HashFunc = _HashFunc<K>>
class UnorderedSet
{
	struct KeyOfValueSet
	{
		const K& operator()(const K& key)
		{
			return  key;
		}
	};

public:
	typedef typename HashTable<K, K, KeyOfValueSet, HashFunc>::Iterator Iterator;//迭代器是哈希表的迭代器

	pair<Iterator, bool> Insert(const K& key)
	{
		return _ht.Insert(key);
	}

	Iterator Begin()
	{
		return _ht.Begin();
	}
	Iterator End()
	{
		return _ht.End();
	}

	Iterator Find(const K& key)
	{
		return _ht.Find(key);
	}
	
protected:
	HashTable<K, K, KeyOfValueSet, HashFunc> _ht;
};

void TestUnorderSet()
{
	UnorderedSet<int> s;
	s.Insert(2);
	s.Insert(3);
	s.Insert(5);
	s.Insert(7);
	s.Insert(0);


	UnorderedSet<int>::Iterator it1 = s.Begin();
	while (it1 != s.End())
	{
		cout << *it1 << " ";
		++it1;
	}
	cout << endl;


	UnorderedSet<string> s1;
	s1.Insert("string");
	s1.Insert("insert");
	s1.Insert("left");
	s1.Insert("make");
	s1.Insert("right");

	UnorderedSet<string>::Iterator it = s1.Begin();
	while (it != s1.End())
	{
		cout << *it << endl;
		++it;
	}
	

}
