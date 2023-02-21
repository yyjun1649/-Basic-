#include "RedBlackTree.h"
#include <iostream>
#include <windows.h>
using namespace std;

RedBlackTree::RedBlackTree()
{
	_nil = new Node();
	_root = _nil;
}

RedBlackTree::~RedBlackTree()
{
	delete _nil;
}

void RedBlackTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	if (node->color == Color::Black)
		cout << "B";
	else
		cout << "R";

	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);
}

Node* RedBlackTree::Search(Node* node, int key)
{
	if (node == _nil || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

Node* RedBlackTree::Min(Node* node)
{
	while (node->left != _nil)
		node = node->left;

	return node;
}

Node* RedBlackTree::Max(Node* node)
{
	while (node->right != _nil)
		node = node->right;

	return node;
}

Node* RedBlackTree::Next(Node* node)
{
	if (node->right != _nil)
		return Min(node->right);

	Node* parent = node->parent;

	while (parent != _nil && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

void RedBlackTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	Node* node = _root;
	Node* parent = _nil;

	while (node != _nil)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (parent == _nil)
		_root = newNode;

	if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;

	//Check
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red;

	InsertFixup(newNode);
}

void RedBlackTree::InsertFixup(Node* node)
{
	// 1) p = red , uncle = red
	// -> p = black, uncle= black, pp = red
	
	// 2) p = red , uncle = black (triangle)
	// -> ȸ���� ���� case 3���� ��ȯ
	
	// 3) p = red , uncle = black (list)
	// -> ���� ���� + ȸ��

	while (node->parent->color == Color::Red)
	{
		if (node->parent == node->parent->parent->left)
		{
			Node* uncle = node->parent->parent->right;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;			// p
				uncle->color = Color::Black;				// u
				node->parent->parent->color = Color::Red;	// pp
			}
			else
			{
				if (node == node->parent->right) // Triangle
				{
					node = node->parent;
					LeftRotate(node);
				}

				// List Ÿ��
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				RightRotate(node->parent->parent);
			}
		}
		else
		{
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;			// p
				uncle->color = Color::Black;				// u
				node->parent->parent->color = Color::Red;	// pp
			}
			else
			{
				if (node == node->parent->left) // Triangle
				{
					node = node->parent;
					RightRotate(node);
				}

				// List Ÿ��
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
}

void RedBlackTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

// BST ���� ����
// case 1
//	������ ��尡 Red -> ����

// case 2
//  root�� DoubleBlack -> �߰� Black ����

// case 3
//  Double Black �� sibling ��尡 Red
//  s = black, p = red ( s <-> p ����ȯ)
//  DB�������� roatate(p)

// case 4
//  �߰� Black�� parent���� ����
//  // p �� Red�� Black
//  // p �� Black�̸� Red
//  s = red
//  p�� ������� �˰��� �̾ ���� (Double Black�� �����Ѵٸ�)

// case 5
//  DB�� sibling ��尡 Black && sibling�� near child = red, far child = black
//  s <-> near ���� ��ȯ
//  far �������� rotate(s)
//  go to case 6

// case 6
// Double Black�� sibling��尡 Black && sibling�� far child = red
// far = black
// rotate(p) DB��������
// �߰� black ����

void RedBlackTree::Delete(Node* node)
{
	if (node == _nil)
		return;

	if (node->left == _nil)
	{
		Color color = node->color;
		Node* right = node->right;
		Replace(node, node->right);

		if (color == Color::Black)
			DeleteFixup(right);
	}
	else if (node->right == _nil)
	{
		Color color = node->color;
		Node* left = node->left;
		Replace(node, node->left);

		if (color == Color::Black)
			DeleteFixup(left);
	}
	else
	{
		// ���� ������ ã��
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}



}

void RedBlackTree::DeleteFixup(Node* node)
{
	Node* x = node;

	// [Case 1][Case 2]
	while (x != _root && x->color == Color::Black)
	{
		if (x == x->parent->left)
		{
			// [Case 3]
			//  Double Black �� sibling ��尡 Red
			//  s = black, p = red ( s <-> p ����ȯ)
			//  DB�������� roatate(p)

			Node* s = x->parent->right;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color == Color::Red;

				LeftRotate(x->parent);

				s = x->parent->right;
			}

			// [Case 4]
			//  �߰� Black�� parent���� ����
			//  // p �� Red�� Black
			//  // p �� Black�̸� Red
			//  s = red
			//  p�� ������� �˰��� �̾ ���� (Double Black�� �����Ѵٸ�)
			if (s->left->color == Color::Black && s->right->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//[Case 5]
				//  DB�� sibling ��尡 Black && sibling�� near child = red, far child = black
				//  s <-> near ���� ��ȯ
				//  far �������� rotate(s)
				//  go to case 6
				if (s->right->color == Color::Black)
				{
					s->left->color == Color::Black;
					s->color == Color::Red;
					RightRotate(s);
					s = x->parent->right;
				}

				//[Case 6]
				// Double Black�� sibling��尡 Black && sibling�� far child = red
				// far = black
				// rotate(p) DB��������
				// �߰� black ����
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->right->color = Color::Black;
				LeftRotate(x->parent);
				x = _root;
			}
		}
		else
		{
			// [Case 3]
			//  Double Black �� sibling ��尡 Red
			//  s = black, p = red ( s <-> p ����ȯ)
			//  DB�������� roatate(p)

			Node* s = x->parent->left;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color == Color::Red;

				RightRotate(x->parent);

				s = x->parent->left;
			}

			// [Case 4]
			//  �߰� Black�� parent���� ����
			//  // p �� Red�� Black
			//  // p �� Black�̸� Red
			//  s = red
			//  p�� ������� �˰��� �̾ ���� (Double Black�� �����Ѵٸ�)
			if (s->right->color == Color::Black && s->left->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//[Case 5]
				//  DB�� sibling ��尡 Black && sibling�� near child = red, far child = black
				//  s <-> near ���� ��ȯ
				//  far �������� rotate(s)
				//  go to case 6
				if (s->left->color == Color::Black)
				{
					s->right->color == Color::Black;
					s->color == Color::Red;
					RightRotate(s);
					s = x->parent->left;
				}

				//[Case 6]
				// Double Black�� sibling��尡 Black && sibling�� far child = red
				// far = black
				// rotate(p) DB��������
				// �߰� black ����
				s->color = x->parent->color;
				x->parent->color = Color::Black;
				s->left->color = Color::Black;
				RightRotate(x->parent);
				x = _root;
			}
		}
	}

	x->color = Color::Black;
}

// u ����Ʈ���� v ����Ʈ���� ��ü
// �׸��� delete u
void RedBlackTree::Replace(Node* u, Node* v)
{
	if (u->parent == _nil)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	v->parent = u->parent;

	delete u;
}

void RedBlackTree::LeftRotate(Node* x)
{
	Node* y = x->right;

	x->right = y->left;
	if(y->left != _nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

void RedBlackTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right;

	if (y->right != _nil)
		y->right->parent = y;

	x->parent = y->parent;

	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->left = y;
	y->parent = x;
}



