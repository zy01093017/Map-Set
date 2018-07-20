#pragma once


#include "RBTree.h"

template<class K>
class Set
{
public:

	//取出Set里面的key
	struct SetOfValue
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};

	typedef typename RBTree<K, K, SetOfValue>::Iterator Iterator;

	Iterator Begin()
	{
		return _t.Begin();
	}
	Iterator End()
	{
		return _t.End();
	}

	pair<Iterator,bool> Insert(const K& key)
	{
		return _t.Insert(key);
	}
protected:
	RBTree<K, K, SetOfValue> _t;
};


void TestSet()
{
	Set<int> s;
	s.Insert(2);
	s.Insert(1);
	s.Insert(3);
	s.Insert(3);
	s.Insert(5);

	Set<int>::Iterator it = s.Begin();
	while (it != s.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}