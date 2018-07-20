#pragma once


#include "HashTable.h"
#iinclude <string>

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


template<class K,class V,class HashFunc = _HashFunc<K>>
class UnorderedMap
{
	struct KeyOfValueMap
	{
		const K& operator()(const pair<K, V>& kv)
		{
			return  kv.first;
		}
	};

public:
	typedef typename HashTable<K, pair<K, V>, KeyOfValueMap, HashFunc>::Iterator Iterator;//µü´úÆ÷ÊÇ¹şÏ£±íµÄµü´úÆ÷

	pair<Iterator,bool> Insert(const pair<K,V>& kv)
	{
		return _ht.Insert(kv);
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
	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = Insert(make_pair(key, V()));
		return (ret.first)->second;
	}
protected:
	HashTable<K, pair<K, V>, KeyOfValueMap, HashFunc> _ht;
};


void TestUnorderedMap()
{
	UnorderedMap<string, string> dict;
	dict.Insert(make_pair("insert", "²åÈë"));
	dict.Insert(make_pair("string", "×Ö·û´®"));
	dict.Insert(make_pair("make", "ÖÆÔì"));
	
	dict["left"] = "×ó±ß";
	dict["left"] = "Ê£Óà";

	UnorderedMap<string, string>::Iterator it = dict.Begin();
	while (it != dict.End())
	{
		cout << it->first << ":" << it->second << endl;
		++it;
	}
}
