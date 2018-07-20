#pragma once

#include <iostream>
using namespace std;

#include <vector>


template<class T>
struct HashNode
{
	HashNode<T>* _next;
	T _data;

	HashNode(const T& data)
		:_data(data)
		, _next(NULL)
	{}
};

template<class K, class T, class KeyOfValue, class __HashFunc>
class HashTable;


//������
template<class K, class T, class KeyOfValue, class __HashFunc>
struct __HashTableIterator
{
	typedef HashNode<T> Node;//�ڵ�
	typedef __HashTableIterator<K, T, KeyOfValue, __HashFunc> Self;//�Լ�����
	Node* _node;
	
	typedef HashTable<K, T, KeyOfValue, __HashFunc>  Table;//��ϣ��
	Table *_ht;

	__HashTableIterator(Node* node, Table* ht)
		:_node(node)
		, _ht(ht)
	{}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}

	T& operator*()
	{
		return _node->_data;
	}

	T* operator->()
	{
		return &(operator*());
	}


	Self& operator++()
	{

		if (_node->_next)//��һ������
		{
			_node->_next;
		}
		else
		{
			//�㵱ǰ���ڵ�λ�ã�����һ����Ϊ�յ�Ͱ�ĵ�һ��
			KeyOfValue kov;
			size_t index = _ht->HashFunc(kov(_node->_data), _ht->_table.size()) + 1;
			for (; index < _ht->_table.size(); ++index)
			{
				if (_ht->_table[index] != NULL)
				{
					_node = _ht->_table[index];
					return  *this;
				}
			}
			_node = NULL;
		}
		return  *this;//�ߵ��⣬��ʾû��

	}
	
};


template<class K, class T, class KeyOfValue, class __HashFunc>
class HashTable
{
	typedef HashNode<T> Node;
	friend  struct __HashTableIterator<K, T, KeyOfValue, __HashFunc>;
public:

	typedef __HashTableIterator<K, T, KeyOfValue, __HashFunc> Iterator;

	Iterator Begin()
	{
		for (size_t i = 0; i < _table.size(); ++i)
		{
			if (_table[i] != NULL)
			{
				return Iterator(_table[i],this);
			}
		}
	}

	Iterator End()
	{
		return Iterator(NULL,this);
	}

	Iterator Find(const K& key)
	{
		KeyOfValue kov;
		//��λ��
		size_t index = HashFunc(kov(key), _table.size());
		Node* cur = _table[index];

		//����
		while (cur)
		{
			if (kov(cur->_data) == key)
			{
				return Iterator(cur, this);
			}
			cur = cur->_next;
		}
		//�Ҳ���
		return Iterator(NULL, this);
	}
	HashTable()
		:_size(0)
	{}
	pair<Iterator,bool> Insert(const T& data)
	{
		//1.���ռ䣬������
		CheckCapacity();
		//2.��λ��
		KeyOfValue kov;
		size_t index = HashFunc(kov(data), _table.size());
		Node* cur = _table[index];

		//3.�鿴�Ƿ����
		while (cur)
		{
			if (kov(cur->_data) == kov(data))
			{
				//return false;
				return make_pair(Iterator(cur, this), false);
			}
			cur = cur->_next;
		}

		//4.����ͷ��
		Node* newnode = new Node(data);
		newnode->_next = _table[index];
		_table[index] = newnode;
		++_size;

		//return true;
		return make_pair(Iterator(newnode, this), true);

	}

	void CheckCapacity()
	{
		//��������Ϊ1ʱ����������
		if (_size == _table.size())
		{
			//���ռ�
			size_t nextprime = GetNextPrime(_table.size());
			vector<Node*> newtable;
			newtable.resize(nextprime, NULL);
			//������
			for (size_t i = 0; i < _table.size(); ++i)
			{
				Node* cur = _table[i];

				KeyOfValue kov;
				while (cur)
				{
					Node* next = cur->_next;
					size_t index = HashFunc(kov(cur->_data), _table.size());

					//����ͷ��
					cur->_next = newtable[index];
					newtable[index] = cur;

					cur = next;
				}
				_table[i] = NULL;
			}
			newtable.swap(_table);
		}
	}
protected:
	
	size_t GetNextPrime(size_t cur)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		for (size_t i = 0; i < _PrimeSize; ++i)
		{
			if (_PrimeList[i] > cur)
			{
				return _PrimeList[i];
			}
		}
		return _PrimeList[_PrimeSize - 1];
	}

	size_t HashFunc(const K& key, size_t n)
	{
		__HashFunc hf;
		return hf(key) % n;
	}

public:
	vector<Node*> _table;
	size_t _size;
};