#include <bits/stdc++.h>
#include <stdint.h>
using namespace std;

enum COLOR
{
	RED,
	BLACK
};

struct Slice
{
	uint8_t size;
	char *data;
};

class Node
{
public:
	Slice key, value;
	COLOR color;
	Node *left, *right, *parent;

	Node(Slice &key, Slice &value) : key(key), value(value)
	{
		parent = left = right = NULL;
		color = RED;
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

class kvStore
{
	Node *root;

public:
	kvStore() { root = NULL; }

	Node *getRoot() { return root; }

	Node *search(Slice &key)
	{
		Node *temp = root;
		while (temp != NULL)
		{
			int ret = strncmp(key.data, temp->key.data, min(key.size, temp->key.size));
			if (ret < 0 || (!ret && key.size < temp->key.size))
			{
				if (temp->left == NULL)
					break;
				else
					temp = temp->left;
			}
			else if (!ret && key.size == temp->key.size)
			{
				break;
			}
			else
			{
				if (temp->right == NULL)
					break;
				else
					temp = temp->right;
			}
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

	bool put(Slice &key, Slice &value)
	{
		if (root == NULL)
		{
			Node *newNode = new Node(key, value);
			newNode->color = BLACK;
			root = newNode;
		}
		else
		{
			Node *temp = search(key);
			int ret = strncmp(key.data, temp->key.data, min(key.size, temp->key.size));
			if (!ret && key.size == temp->key.size)
			{
				free(temp->value.data);
				temp->value.size = value.size;
				temp->value.data = value.data;
				return true;
			}
			Node *newNode = new Node(key, value);
			newNode->parent = temp;
			if (ret < 0 || (!ret && key.size < temp->key.size))
				temp->left = newNode;
			else
				temp->right = newNode;
			fixRedRed(newNode);
		}
		return true;
	}

	bool get(Slice &key, Slice &value)
	{
		if (root == NULL)
			return false;
		Node *temp = search(key);
		int ret = strncmp(key.data, temp->key.data, min(key.size, temp->key.size));
		if (!ret && key.size == temp->key.size)
		{
			value.size = temp->value.size;
			value.data = temp->value.data;
			return true;
		}
		return false;
	}

	// void swapValues(Node *u, Node *v)
	// {
	// 	int temp;
	// 	temp = u->key;
	// 	u->key = v->key;
	// 	v->key = temp;
	// }

	// Node *successor(Node *x)
	// {
	// 	Node *temp = x;

	// 	while (temp->left != NULL)
	// 		temp = temp->left;

	// 	return temp;
	// }

	// Node *BSTreplace(Node *x)
	// {
	// 	if (x->left != NULL and x->right != NULL)
	// 		return successor(x->right);
	// 	if (x->left == NULL and x->right == NULL)
	// 		return NULL;
	// 	if (x->left != NULL)
	// 		return x->left;
	// 	else
	// 		return x->right;
	// }

	// void deleteNode(Node *v)
	// {
	// 	Node *u = BSTreplace(v);
	// 	bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
	// 	Node *parent = v->parent;
	// 	if (u == NULL)
	// 	{
	// 		if (v == root)
	// 		{
	// 			root = NULL;
	// 		}
	// 		else
	// 		{
	// 			if (uvBlack)
	// 			{
	// 				fixDoubleBlack(v);
	// 			}
	// 			else
	// 			{
	// 				if (v->sibling() != NULL)
	// 					v->sibling()->color = RED;
	// 			}
	// 			if (v->isOnLeft())
	// 			{
	// 				parent->left = NULL;
	// 			}
	// 			else
	// 			{
	// 				parent->right = NULL;
	// 			}
	// 		}
	// 		delete v;
	// 		return;
	// 	}

	// 	if (v->left == NULL or v->right == NULL)
	// 	{
	// 		if (v == root)
	// 		{
	// 			v->key = u->key;
	// 			v->left = v->right = NULL;
	// 			delete u;
	// 		}
	// 		else
	// 		{
	// 			if (v->isOnLeft())
	// 			{
	// 				parent->left = u;
	// 			}
	// 			else
	// 			{
	// 				parent->right = u;
	// 			}
	// 			delete v;
	// 			u->parent = parent;
	// 			if (uvBlack)
	// 			{
	// 				fixDoubleBlack(u);
	// 			}
	// 			else
	// 			{
	// 				u->color = BLACK;
	// 			}
	// 		}
	// 		return;
	// 	}
	// 	swapValues(u, v);
	// 	deleteNode(u);
	// }

	// void fixDoubleBlack(Node *x)
	// {
	// 	if (x == root)
	// 		return;
	// 	Node *sibling = x->sibling(), *parent = x->parent;
	// 	if (sibling == NULL)
	// 	{
	// 		fixDoubleBlack(parent);
	// 	}
	// 	else
	// 	{
	// 		if (sibling->color == RED)
	// 		{
	// 			parent->color = RED;
	// 			sibling->color = BLACK;
	// 			if (sibling->isOnLeft())
	// 			{
	// 				rightRotate(parent);
	// 			}
	// 			else
	// 			{
	// 				leftRotate(parent);
	// 			}
	// 			fixDoubleBlack(x);
	// 		}
	// 		else
	// 		{
	// 			if (sibling->hasRedChild())
	// 			{
	// 				if (sibling->left != NULL and sibling->left->color == RED)
	// 				{
	// 					if (sibling->isOnLeft())
	// 					{
	// 						sibling->left->color = sibling->color;
	// 						sibling->color = parent->color;
	// 						rightRotate(parent);
	// 					}
	// 					else
	// 					{
	// 						sibling->left->color = parent->color;
	// 						rightRotate(sibling);
	// 						leftRotate(parent);
	// 					}
	// 				}
	// 				else
	// 				{
	// 					if (sibling->isOnLeft())
	// 					{
	// 						sibling->right->color = parent->color;
	// 						leftRotate(sibling);
	// 						rightRotate(parent);
	// 					}
	// 					else
	// 					{
	// 						sibling->right->color = sibling->color;
	// 						sibling->color = parent->color;
	// 						leftRotate(parent);
	// 					}
	// 				}
	// 				parent->color = BLACK;
	// 			}
	// 			else
	// 			{
	// 				sibling->color = RED;
	// 				if (parent->color == BLACK)
	// 					fixDoubleBlack(parent);
	// 				else
	// 					parent->color = BLACK;
	// 			}
	// 		}
	// 	}
	// }

	// void levelOrder(Node *x)
	// {
	// 	if (x == NULL)
	// 		return;
	// 	queue<Node *> q;
	// 	Node *curr;
	// 	q.push(x);
	// 	while (!q.empty())
	// 	{
	// 		curr = q.front();
	// 		q.pop();
	// 		cout << curr->key << " ";
	// 		if (curr->left != NULL)
	// 			q.push(curr->left);
	// 		if (curr->right != NULL)
	// 			q.push(curr->right);
	// 	}
	// }

	// void inorder(Node *x)
	// {
	// 	if (x == NULL)
	// 		return;
	// 	inorder(x->left);
	// 	cout << x->key << " ";
	// 	inorder(x->right);
	// }

	// void deleteByVal(int n)
	// {
	// 	if (root == NULL)
	// 		return;

	// 	Node *v = search(n), *u;

	// 	if (v->key != n)
	// 	{
	// 		cout << "No node found to delete with value:" << n << endl;
	// 		return;
	// 	}

	// 	deleteNode(v);
	// }

	// void printInOrder()
	// {
	// 	cout << "Inorder: " << endl;
	// 	if (root == NULL)
	// 		cout << "Tree is empty" << endl;
	// 	else
	// 		inorder(root);
	// 	cout << endl;
	// }

	// void printLevelOrder()
	// {
	// 	cout << "Level order: " << endl;
	// 	if (root == NULL)
	// 		cout << "Tree is empty" << endl;
	// 	else
	// 		levelOrder(root);
	// 	cout << endl;
	// }
};

// string sliceToStr(Slice &a)
// {
// 	string ret = "";

// 	for (int i = 0; i < a.size; i++)
// 		ret += a.data[i];

// 	return ret;
// }

// void strToSlice(string l, Slice &a)
// {
// 	a.size = l.length();
// 	a.data = (char *)malloc(a.size);
// 	strncpy(a.data, l.c_str(), a.size);
// }

// int main()
// {
// 	ios_base::sync_with_stdio(0);
// 	cin.tie(0);
// 	cout.tie(0);
// 	kvStore t;
// 	Slice key, value;
// 	key.size = 5;
// 	key.data = (char *)malloc(sizeof(char) * key.size);
// 	strncpy(key.data, "hello", key.size);
// 	value.size = 10;
// 	value.data = (char *)malloc(sizeof(char) * value.size);
// 	strncpy(value.data, "helloWorld", value.size);
// 	t.put(key, value);
// 	Slice val;
// 	int ret = t.get(key, val);
// 	cout << "Key\n";
// 	cout << sliceToStr(key) << "\n";
// 	if (!ret)
// 	{
// 		cout << "Key does not exist\n";
// 	}
// 	else
// 	{
// 		if (val.size != value.size || strncmp(val.data, value.data, val.size))
// 		{
// 			cout << "Wrong value\n";
// 		}
// 		else
// 		{
// 			cout << sliceToStr(val) << "\n";
// 		}
// 	}
// 	// key.size = 4;
// 	// key.data = (char *)malloc(sizeof(char) * key.size);
// 	// strncpy(key.data, "hell", key.size);
// 	// value.size = 8;
// 	// value.data = (char *)malloc(sizeof(char) * value.size);
// 	// strncpy(value.data, "helloWod", value.size);
// 	// t.put(key, value);
// 	// ret = t.get(key, val);
// 	// cout << "Key\n";
// 	// cout << sliceToStr(key) << "\n";
// 	// if (!ret)
// 	// {
// 	// 	cout << "Key does not exist\n";
// 	// }
// 	// else
// 	// {
// 	// 	if (val.size != value.size || strncmp(val.data, value.data, val.size))
// 	// 	{
// 	// 		cout << "Wrong value\n";
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		cout << sliceToStr(val) << "\n";
// 	// 	}
// 	// }
// 	return 0;
// }