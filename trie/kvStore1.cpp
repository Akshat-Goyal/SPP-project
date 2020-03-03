#include <bits/stdc++.h>
using namespace std;

struct Slice
{
	uint8_t size;
	char *data;
};

enum COLOR
{
	RED,
	BLACK
};

class RBTree
{
	class Node
	{
	public:
		char ch;
		int cnt;
		RBTree *ptr;
		COLOR color;
		Node *left, *right, *parent;

		Node(char ch) : ch(ch)
		{
			ptr = new RBTree;
			cnt = 0;
			color = RED;
			parent = left = right = NULL;
		}

		inline bool isOnLeft() { return this == parent->left; }

		Node *uncle()
		{
			if (parent == NULL || parent->parent == NULL)
				return NULL;
			if (parent->isOnLeft())
				return parent->parent->right;
			else
				return parent->parent->left;
		}

		Node *sibling()
		{
			if (parent == NULL)
				return NULL;
			if (isOnLeft())
				return parent->right;
			return parent->left;
		}

		void moveDown(Node *nParent)
		{
			if (parent != NULL)
			{
				if (isOnLeft())
				{
					parent->left = nParent;
				}
				else
				{
					parent->right = nParent;
				}
			}
			nParent->parent = parent;
			parent = nParent;
		}

		inline bool hasRedChild()
		{
			return (left != NULL && left->color == RED) || (right != NULL && right->color == RED);
		}
	};
	Slice val;
	Node *root;

public:
	RBTree()
	{
		root = NULL;
		val.data = NULL;
	}

	Node *getRoot() { return root; }

	Node *search(char ch)
	{
		Node *temp = root;
		while (temp != NULL)
		{
			if (ch < temp->ch)
			{
				if (temp->left == NULL)
					break;
				else
					temp = temp->left;
			}
			else if (ch > temp->ch)
			{
				if (temp->right == NULL)
					break;
				else
					temp = temp->right;
			}
			else
				break;
		}
		return temp;
	}

	void leftRotate(Node *x)
	{
		Node *nParent = x->right;
		if (x == root)
			root = nParent;
		x->moveDown(nParent);
		x->right = nParent->left;
		if (nParent->left != NULL)
			nParent->left->parent = x;
		nParent->left = x;
	}

	void rightRotate(Node *x)
	{
		Node *nParent = x->left;
		if (x == root)
			root = nParent;
		x->moveDown(nParent);
		x->left = nParent->right;
		if (nParent->right != NULL)
			nParent->right->parent = x;
		nParent->right = x;
	}

	inline void swapColors(Node *x1, Node *x2)
	{
		COLOR temp;
		temp = x1->color;
		x1->color = x2->color;
		x2->color = temp;
	}

	void fixRedRed(Node *x)
	{
		if (x == root)
		{
			x->color = BLACK;
			return;
		}
		Node *parent = x->parent, *grandparent = parent->parent,
			 *uncle = x->uncle();
		if (parent->color != BLACK)
		{
			if (uncle != NULL && uncle->color == RED)
			{
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				fixRedRed(grandparent);
			}
			else
			{
				if (parent->isOnLeft())
				{
					if (x->isOnLeft())
					{
						swapColors(parent, grandparent);
					}
					else
					{
						leftRotate(parent);
						swapColors(x, grandparent);
					}
					rightRotate(grandparent);
				}
				else
				{
					if (x->isOnLeft())
					{
						rightRotate(parent);
						swapColors(x, grandparent);
					}
					else
					{
						swapColors(parent, grandparent);
					}
					leftRotate(grandparent);
				}
			}
		}
	}

	RBTree *putChar(char ch)
	{
		if (root == NULL)
		{
			Node *newNode = new Node(ch);
			newNode->color = BLACK;
			root = newNode;
			return newNode->ptr;
		}
		else
		{
			Node *temp = search(ch);
			if (temp->ch == ch)
				return temp->ptr;
			Node *newNode = new Node(ch);
			newNode->parent = temp;
			if (ch < temp->ch)
				temp->left = newNode;
			else
				temp->right = newNode;
			fixRedRed(newNode);
			return newNode->ptr;
		}
	}

	inline bool putVal(Slice &value)
	{
		if (val.data != NULL)
		{
			free(val.data);
			val = value;
			return false;
		}
		val = value;
		return true;
	}

	inline void incCnt(char ch)
	{
		Node *temp = search(ch);
		temp->cnt++;
	}

	inline RBTree *getChar(char ch)
	{
		Node *temp = search(ch);
		if (temp == NULL || temp->ch != ch)
			return NULL;
		return temp->ptr;
	}

	inline void getVal(Slice &value)
	{
		value = val;
	}

	Node *successor(Node *x)
	{
		Node *temp = x;
		while (temp->left != NULL)
			temp = temp->left;
		return temp;
	}

	Node *BSTreplace(Node *x)
	{
		if (x->left != NULL and x->right != NULL)
			return successor(x->right);
		if (x->left == NULL and x->right == NULL)
			return NULL;
		if (x->left != NULL)
			return x->left;
		else
			return x->right;
	}

	void swapValues(Node *u, Node *v)
	{
		RBTree *temp = u->ptr;
		u->ptr = v->ptr;
		v->ptr = temp;
		int tmp = u->ch;
		u->ch = v->ch;
		v->ch = tmp;
		tmp = u->cnt;
		u->cnt = v->cnt;
		v->cnt = tmp;
	}

	void fixDoubleBlack(Node *x)
	{
		if (x == root)
			return;
		Node *sibling = x->sibling(), *parent = x->parent;
		if (sibling == NULL)
		{
			fixDoubleBlack(parent);
		}
		else
		{
			if (sibling->color == RED)
			{
				parent->color = RED;
				sibling->color = BLACK;
				if (sibling->isOnLeft())
				{
					rightRotate(parent);
				}
				else
				{
					leftRotate(parent);
				}
				fixDoubleBlack(x);
			}
			else
			{
				if (sibling->hasRedChild())
				{
					if (sibling->left != NULL and sibling->left->color == RED)
					{
						if (sibling->isOnLeft())
						{
							sibling->left->color = sibling->color;
							sibling->color = parent->color;
							rightRotate(parent);
						}
						else
						{
							sibling->left->color = parent->color;
							rightRotate(sibling);
							leftRotate(parent);
						}
					}
					else
					{
						if (sibling->isOnLeft())
						{
							sibling->right->color = parent->color;
							leftRotate(sibling);
							rightRotate(parent);
						}
						else
						{
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							leftRotate(parent);
						}
					}
					parent->color = BLACK;
				}
				else
				{
					sibling->color = RED;
					if (parent->color == BLACK)
						fixDoubleBlack(parent);
					else
						parent->color = BLACK;
				}
			}
		}
	}

	void deleteNode(Node *v)
	{
		Node *u = BSTreplace(v);
		bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
		Node *parent = v->parent;
		if (u == NULL)
		{
			if (v == root)
			{
				root = NULL;
			}
			else
			{
				if (uvBlack)
				{
					fixDoubleBlack(v);
				}
				else
				{
					if (v->sibling() != NULL)
						v->sibling()->color = RED;
				}
				if (v->isOnLeft())
				{
					parent->left = NULL;
				}
				else
				{
					parent->right = NULL;
				}
			}
			delete v->ptr;
			delete v;
			return;
		}

		if (v->left == NULL || v->right == NULL)
		{
			if (v == root)
			{
				delete v->ptr;
				delete v;
				root = u;
				u->parent = NULL;
			}
			else
			{
				if (v->isOnLeft())
				{
					parent->left = u;
				}
				else
				{
					parent->right = u;
				}
				u->parent = parent;
				delete v->ptr;
				delete v;
				if (uvBlack)
				{
					fixDoubleBlack(u);
				}
				else
				{
					u->color = BLACK;
				}
			}
			return;
		}
		swapValues(u, v);
		deleteNode(u);
	}

	inline void decCnt(char ch)
	{
		Node *temp = search(ch);
		temp->cnt--;
		if (temp->cnt == 0)
		{
			deleteNode(temp);
		}
	}

	inline bool delVal()
	{
		if (val.data == NULL)
			return false;
		free(val.data);
		val.data = NULL;
		return true;
	}

	// bool get(Slice &key, Slice &value)
	// {
	//     if (root == NULL)
	//         return false;
	//     Node *temp = search(key);
	//     int ret = strncmp(key.data, temp->key.data, min(key.size, temp->key.size));
	//     if (!ret && key.size == temp->key.size)
	//     {
	//         value.size = temp->value.size;
	//         value.data = temp->value.data;
	//         return true;
	//     }
	//     return false;
	// }

	// bool del(Slice &key)
	// {
	//     if (root == NULL)
	//         return false;
	//     Node *temp = search(key);
	//     int ret = strncmp(key.data, temp->key.data, min(key.size, temp->key.size));
	//     if (!ret && key.size == temp->key.size)
	//     {
	//         deleteNode(temp);
	//         return true;
	//     }
	//     return false;
	// }

	// Node *search(Node *temp, int N)
	// {
	//     if (temp == NULL)
	//         return temp;
	//     if (N > temp->child || N < 0)
	//         return NULL;
	//     if (N <= getChild(temp->left))
	//         return search(temp->left, N);
	//     else if (N == getChild(temp->left) + 1)
	//         return temp;
	//     return search(temp->right, N - getChild(temp->left) - 1);
	// }

	// bool get(int N, Slice &key, Slice &value)
	// {
	//     Node *temp = search(root, N + 1);
	//     if (temp == NULL)
	//         return false;
	//     key = temp->key;
	//     value = temp->value;
	//     return true;
	// }

	// bool del(int N)
	// {
	//     Node *temp = search(root, N + 1);
	//     if (temp == NULL)
	//         return false;
	//     deleteNode(temp);
	//     return true;
	// }

	// string sliceToStr(Slice &a)
	// {
	//     string ret = "";
	//     for (int i = 0; i < a.size; i++)
	//         ret += a.data[i];
	//     return ret;
	// }

	// void levelOrder(Node *x)
	// {
	//     if (x == NULL)
	//         return;
	//     queue<Node *> q;
	//     Node *curr;
	//     q.push(x);
	//     while (!q.empty())
	//     {
	//         curr = q.front();
	//         q.pop();
	//         cout << sliceToStr(curr->key) << "\n"
	//              << sliceToStr(curr->value) << "\n"
	//              << curr->child << "\n";
	//         if (curr->left != NULL)
	//             q.push(curr->left);
	//         if (curr->right != NULL)
	//             q.push(curr->right);
	//     }
	// }

	// void inorder(Node *x)
	// {
	//     if (x == NULL)
	//         return;
	//     inorder(x->left);
	//     cout << sliceToStr(x->key) << "\n"
	//          << sliceToStr(x->value) << "\n"
	//          << x->child << "\n";
	//     inorder(x->right);
	// }

	// void printInOrder()
	// {
	//     cout << "Inorder: " << endl;
	//     if (root == NULL)
	//         cout << "Tree is empty" << endl;
	//     else
	//         inorder(root);
	//     cout << endl;
	// }

	// void printLevelOrder()
	// {
	//     cout << "Level order: " << endl;
	//     if (root == NULL)
	//         cout << "Tree is empty" << endl;
	//     else
	//         levelOrder(root);
	//     cout << endl;
	// }
};

class kvStore
{

private:
	RBTree *root;

public:
	kvStore() { root = new RBTree(); }

	bool put(Slice &key, Slice &value, RBTree *root, int i)
	{
		if (i == key.size)
			return root->putVal(value);
		if (put(key, value, root->putChar(key.data[i]), i + 1))
		{
			root->incCnt(key.data[i]);
			return true;
		}
		return false;
	}

	bool put(Slice &key, Slice &value)
	{
		put(key, value, root, 0);
		return true;
	}

	bool get(Slice &key, Slice &value)
	{
		if (root == NULL)
			return false;
		RBTree *temp = root;
		for (int i = 0; i < key.size; i++)
		{
			temp = temp->getChar(key.data[i]);
			if (temp == NULL)
				return false;
		}
		temp->getVal(value);
		if (value.data == NULL)
			return false;
		return true;
	}

	bool del(Slice &key, RBTree *root, int i)
	{
		if (root == NULL)
			return false;
		if (i == key.size)
		{
			return root->delVal();
		}
		if (del(key, root->getChar(key.data[i]), i + 1))
		{
			root->decCnt(key.data[i]);
			return true;
		}
		return false;
	}

	bool del(Slice &key)
	{
		return del(key, root, 0);
	}
};
