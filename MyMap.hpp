
#pragma once


#include "RBTree.h"


#include <string>
template<class K,class V>
class Map
{
	struct MapKeyOfValue
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return kv.first;
		}
	};
public:
	
	typedef typename RBTree<K, pair<K, V>, MapKeyOfValue>::Iterator Iterator;
	Iterator Begin()
	{
		return _t.Begin();
	}

	Iterator End()
	{
		return _t.End();
	}

	pair<Iterator, bool> Insert(const pair<K, V>& kv)
	{
		return _t.Insert(kv);
	}

	Iterator Find(const K& key)
	{
		return _t.Find(key);
	}

	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = Insert(make_pair(key, V()));
		return ret.first->second;
	}
protected:
	RBTree<K, pair<K, V>,MapKeyOfValue> _t;
};

void TestMap()
{
	Map<string, string> dict;
	dict.Insert(make_pair("sort", "ÅÅÐò"));
	dict.Insert(make_pair("string", "×Ö·û´®"));
	dict["left"] = "×ó±ß";
	dict["left"] = "Ê£Óà";

	Map<string, string>::Iterator it = dict.Begin();
	while (it != dict.End())
	{
		cout << it->first << ":" << it->second << endl;
		++it;
	}

	Map<string, int> countMap;
	typedef Map<string, int>::Iterator CountMapIter;
	string strs[] = { "string", "string", "end", "string", "second", "end" };
	for (size_t i = 0; i < sizeof(strs) / sizeof(string); ++i)
	{
		/*CountMapIter ret = countMap.Find(strs[i]);
		if (ret != countMap.End())
		{
		ret->second++;
		}
		else
		{
		countMap.Insert(make_pair(strs[i], 1));
		}*/

		countMap[strs[i]]++;
	}

	CountMapIter cnIt = countMap.Begin();
	while (cnIt != countMap.End())
	{
		cout << cnIt->first << ":" << cnIt->second << endl;
		++cnIt;
	}
	cout << endl;

}