#pragma once

#include <iostream>
using namespace std;


#include <assert.h>

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	int _bf;

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _bf(0)
	{}
};

template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V>   Node;
	
public:
	AVLTree()
		:_root(NULL)
	{}

	~AVLTree()
	{

	}
	bool Insert(const K& key, const V& value)
	{
		//1.空树
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		//2.不是空树
		Node* parent = NULL;
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else//相等不插入
			{
				return false;
			}		 
		}
		//走到该插入的位置
		cur = new Node(key, value);
		if (parent->_key < key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//调整平衡因子，从插入的地方向上调整
		//当前是cur
		while (parent)
		{
			if (parent->_left == cur)//左孩子减减平衡因子
			{
				--parent->_bf;
			}
			else//右孩子加加平衡因子
			{
				++parent->_bf;
			}

			if (parent->_bf == 1 || parent->_bf == -1)//当parent
			{
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 0)
			{
				break;
			}
			else //当父亲的平衡因子是2或者-2时进行旋转
			{
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)//如果父亲是2，孩子是1，则进行左旋
					{
						RotateL(parent);
					}
					else//父亲是2，孩子是-1,进行右左双旋
					{
						RotateRL(parent);
					}
				}
				else//父亲是-2的情况
				{
					if (cur->_bf == -1)//父亲是-2，孩子是-1，进行右单旋
					{
						RotateR(parent);
					}
					else//父亲是-2，孩子是1，进行左右双旋
					{
						RotateLR(parent);
					}
				}
				break;
			}			
		}
		return true;
	}

	//     40 parent
	//    /  \  
	//  30    50 subR
	//       /  \
    //subRL 35  60
	//            \
	//             70
	void RotateL(Node* parent)//平衡因子为2，右孩子为1
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
		parent->_bf = subR->_bf = 0;//平衡因子都为0
	}

	//       70 parent
	//      /  \
    //subL 60  80
	//    /  \
	//   50  65 SubLR
	//  /
	// 40

	void RotateR(Node* parent)//父亲平衡因子是-2，孩子是-1
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
		subL->_bf = parent->_bf = 0;
	}

	/*1.三个节点 
	
panent	50                50                45
	    /     先左旋      /     再右旋      / \
subR  40      --->      45      --->       40  50
	   \                /
subRL  45              40
	*/

	/*2.插入节点是subLR的左孩子
	
	  50                  50                       45
	 /  \                / \                      / \
   40    60   先左旋    45  60      在右旋 bf=0  40   50   bf=1
   / \        --->      /           ----->      / \     \
  30  45               40                      30  43    60
     /                / \
	 43              30  43
*/

	/*3.插入节点是subLR的右孩子

	   50                  50                       45
	  /  \                / \                      /  \
	40    60   先左旋    45  60      在右旋 bf=-1 40   50   bf=0
	/ \        --->     / \          ----->      /     / \
   30  45              40  48                  30     48  60
        \             / 
         48           30  
*/

	void RotateLR(Node* parent)//左右双旋，父亲是-2，孩子是1
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;
		//先以subL进行左旋
		RotateL(parent->_left);
		//再以parent进行右旋
		RotateR(parent);

		//此时平衡因子会进行改变
		if (bf == 0)//表示subLR就是当前节点
		{
			parent->_bf = subL->_bf = subLR->_bf = 0;
		}
		else if (bf == -1)
		{
			subLR->_bf = 0;
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (bf == 1)
		{
			subLR->_bf = 0;
			subL->_bf = -1;
			parent->_bf = 0;
		}
		else
		{
			assert(false);//出错
		}
	}
	/*1.三个节点
	   30                   30                            35
		 \     先右旋         \          再左旋          /  \
		 40    ---->          35         ----->        30   40
		/                       \
	  35                        40
	*/

	/*2.插入节点是subRL的左孩子
	   
	   30 parent                                30                                          35 subRL  bf = 0
	  / \            以subR进行右旋            /  \              以parent进行左旋          / \
    20   40  subR    ---->                   20    35  SubRL    -------->parent bf=0     30   40 subR bf=1
	     / \                                      / \                                    / \   \
bf=-1   35  50                                   33  40 subR                            20  33  50
	   /                                              \
	  33                                               50
	*/



	/*3.插入节点是subRL的右孩子

		30 parent                                 30                                          35 subRL  bf = 0
		/ \            以subR进行右旋            /  \              以parent进行左旋          / \
	   20 40  subR    ---->                     20  35  SubRL    -------->     parent bf=-1 30 40 subR bf=0
	      / \                                         \                                    /   / \
bf=-1    35  50                                       40 subR                            20   37  50
	      \                                          /  \
	      37                                        37  50
*/
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		//先以subL进行右旋
		RotateR(parent->_right);
		//再以parent进行左旋
		RotateL(parent);

		if (bf == 0)//当前节点就是插入的节点
		{
			subR->_bf = subRL->_bf = parent->_bf = 0;
		}
		else if (bf == -1)
		{
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else if (bf == 1)
		{
			subR->_bf = 0;
			parent->_bf = -1;
			subRL->_bf = 0;
		}
		else
		{
			assert(false);
		}
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

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
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
			if (rightHeight - leftHeight != root->_bf)
			{
				cout << "平衡因子异常" << root->_key << endl;
				return false;
			}

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


void TestAVLtree()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };//{16, 3, 7, 11, 9, 26, 18, 14, 15};
	AVLTree<int, int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
	{
		t.Insert(a[i], i);
		cout << a[i] << ":" << t.IsBalance() << endl;
	}

	t.InOrder();
	cout << t.IsBalance() << endl;
}