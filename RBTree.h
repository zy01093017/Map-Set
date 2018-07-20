
#pragma once


#include <iostream>
using namespace std;

enum Colour
{
	RED,
	BLACK,
};


template<class T>
struct RBTreeNode
{
	T _data;

	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _parent;

	Colour _colour;

	RBTreeNode(const T& data)
		:_data(data)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _colour(RED)
	{}
};



template<class T>
struct __RBTreeItreator
{
	typedef RBTreeNode<T> Node;
	typedef  __RBTreeItreator<T> Self;
	Node* _node;

	__RBTreeItreator(Node* node)
		:_node(node)
	{}

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
		//如果右边不为空，就去找右边的最左节点
		if (_node->_right)
		{
			_node = _node->_right;
			while (_node->_left)
			{
				_node = _node->_left;
			}
		}
		else//如果右边为空，就找其祖先
		{
			Node* cur = _node;
			Node* parent = cur->_parent;

			while (parent)
			{
				if (cur == parent->_right)
				{
					cur = parent;
					parent = parent->_parent;
				}
				else
				{
					break;
				}
			}

			_node = parent;
		}
		return *this;
	}
	Self operator++(int)//后置++
	{
		Self tmp(*this);
		++(*this);
		return tmp;
	}
	Self& operator--()
	{
		if (_node->_left)
		{
			_node = _node->_left;
			while (_node->_right)
			{
				_node = _node->_right;
			}
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;

			while (parent)
			{
				if (cur == parent->_left)
				{
					cur = parent;
					parent = parent->_parent;
				}
				else
				{
					break;
				}
			}
			_node = parent;
		}
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		--(*this);
		return tmp;
	}

	bool operator==(const Self& s)
	{
		return _node == s._node;
	}

	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
};
template<class K,class T,class KeyOfValue>
class RBTree
{
	typedef RBTreeNode<T> Node;
	
public:
	typedef  __RBTreeItreator<T> Iterator;
	RBTree()
		:_root(NULL)
	{}

	Iterator Begin()
	{
		Node* cur = _root;
		while (cur->_left)
		{
			cur = cur->_left;
		}
		return Iterator(cur);
	}
	Iterator End()
	{
		return Iterator(NULL);
	}


	Iterator Find(const K& key)
	{
		Node* cur = _root;
		KeyOfValue kov;
		while (cur)
		{
			if (kov(cur->_data) < key)
			{
				cur = cur->_right;
			}
			else if (kov(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else
			{
				return Iterator(cur);
			}
		}

		return End();
	}
	pair<Iterator,bool> Insert(const T& value)
	{
		if (_root == NULL)
		{
			_root = new Node(value);
			_root->_colour = BLACK;
			//return true;
			return make_pair(Iterator(_root), true);
		}

		Node* parent = NULL;
		Node* cur = _root;

		KeyOfValue kov;
		while (cur)
		{
			if (kov(cur->_data) < kov(value))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kov(cur->_data) > kov(value))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				//return false;
				return make_pair(Iterator(cur), false);

			}
		}
		
		cur = new Node(value);//插入节点
		Node* newnode = cur;
		cur->_colour = RED;

		if (kov(parent->_data) < kov(value))
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//调平衡
		while (parent && parent->_colour == RED)//当父亲的颜色为红色需要进行旋转
		{
			Node* grandfather = parent->_parent;

			if (grandfather->_left == parent)//如果父亲是左孩子
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_colour == RED)//如果叔叔存在且为红
				{
					parent->_colour = uncle->_colour = BLACK;//父亲和叔叔颜色变为黑，爷爷变为红
					grandfather->_colour = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//叔叔不存在或者叔叔为黑
				{
					//当cur是父亲的右孩子，为折线，是一个左右双旋
					if (parent->_right == cur)
					{
						RotateL(parent);

						swap(cur, parent);//先左旋之后进行交换，保证改变颜色的一定是parent和grandfather？
					}

					RotateR(grandfather);

					parent->_colour = BLACK;
					grandfather->_colour = RED;
				}
			}
			else//父亲是右孩子
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_colour == RED)//存在且为红
				{
					uncle->_colour = parent->_colour = BLACK;
					grandfather->_colour = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//叔叔不存在或者为黑
				{
					if (parent->_left == cur)
					{
						RotateR(parent);

						swap(cur, parent);
					}

					RotateL(grandfather);
					parent->_colour = BLACK;
					grandfather->_colour = RED;
				}
			}
		}
		_root->_colour = BLACK;
		//return true;
		return make_pair(Iterator(newnode), true);

	}

	/*
	1.爷爷，父亲，孙子在同一侧，叔叔不存在或者为黑，进行右单旋
	     ●   以gandfa         ○ parent                 ●
		/ \    右旋           /  \                      /  \
	   ○  ●  ---->    cur ○    ● grandfa    ---->  ○   ○            parent  = 黑
	  /                            \                          \           gandfather  = 红
     ○                             ● uncle                   ●
  */

	/*
	2.爷爷，父亲，在同一侧，孙子在父亲的右侧，是一折线，叔叔不存在或者为黑，先以父亲进行左单旋，在以爷爷进行右单旋
	     ●       以parent       ●                    ○
	    /  \      先左旋        /  \     后右旋       /  \
      ○    ●    ------>      ○  ●    ------>     ○   ●  -----> 见上图进行改变颜色
	   \                      /                            \
	   ○                    ○                             ●
	*/


	void RotateL(Node* parent)//父亲是红，在爷爷右边，孩子在右边，或者父亲红，在爷爷左边，孩子在右边，先以父亲为准左旋，在以爷爷进行右旋
	{
		Node* subR = parent->_right;//右孩子
		Node* subRL = subR->_left;//右孩子的左孩子
		Node* ppNode = parent->_parent;//父亲的父亲

		parent->_right = subRL;//父亲的右孩子指向右孩子的左孩子
		if (subRL != NULL)
			subRL->_parent = parent;

		subR->_left = parent;//右孩子的左孩子指向父亲
		parent->_parent = subR;
		if (parent == _root)//为根，subR就是根
		{
			_root = subR;
			_root->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)//如果parent是父亲的左孩子，父亲的左孩子指向SubR
			{
				ppNode->_left = subR;
			}
			else if (ppNode->_right == parent)//如果parent是父亲的右孩子，父亲的右孩子指向SubR
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}
	/*
	1.爷爷，父亲，孙子在同一侧，叔叔不存在或者为黑，进行左单旋
		 ●   以gandfa          ○ parent                 ●
	    / \    左旋            /  \        改颜色        /  \             parent = 黑
	  ●   ○    ---->grandfa ●   ○cur   ---->        ○   ○           grandfather = 红
	        \                /                         /      
	        ○         uncle●                        ●
	*/
	
	/*
	2.爷爷，父亲，在同一侧，孙子在父亲的右侧，是一折线，叔叔不存在或者为黑，先以父亲进行左单旋，在以爷爷进行右单旋
	   ●       以parent       ●                   ○
	  /  \      先右旋        /  \    后左旋       /  \
	 ●   ○    ------>      ●  ○  ------>      ●  ○-----> 见上图进行改变颜色
		 /	                       \             /     
	    ○                         ○          ●
	*/

	void RotateR(Node* parent)//父亲是红，在爷爷左边，孩子在左边，或者父亲红，在爷爷右边，孩子在左边，先以父亲为准右旋，在以爷爷进行左旋
	{
		Node* subL = parent->_left;//父亲的左孩子
		Node* subLR = subL->_right;//左孩子的右孩子
		Node* ppNode = parent->_parent;//父亲的父亲

		parent->_left = subLR;//父亲的左孩子指向左孩子的右孩子
		if (subLR != NULL)
			subLR->_parent = parent;

		subL->_right = parent;//左孩子的右孩子指向父亲		
		parent->_parent = subL;
		if (_root == parent)//根节点
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)//还有父亲，是父亲的左孩子
			{
				ppNode->_left = subL;
			}
			else//还有父亲，是父亲的右孩子
			{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}


	bool _IsBalance(Node* root, int& height)
	{
		if (root == NULL)
		{
			height = 0;
			return true;
		}

		int leftHeight = 0;
		int rightHeight = 0;
		if (_IsBalance(root->_left, leftHeight)
			&& _IsBalance(root->_right, rightHeight))
		{
			height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
			return abs(leftHeight - rightHeight) < 2;
		}
		else
		{
			return false;
		}
	}

	bool IsBalance()
	{
		int height = 0;
		return _IsBalance(_root, height);
	}
protected:
	Node* _root;
};


//void TestRBTree()
//{
//	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
//	RBTree<int, int> t;
//	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
//	{
//		t.Insert(a[i], i);
//		cout << a[i] << ":" << t.IsBalance() << endl;
//
//	}
//	t.InOrder();
//	cout << t.IsBalance() << endl;
//
//}