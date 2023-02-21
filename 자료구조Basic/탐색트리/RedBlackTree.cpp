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
	// -> 회전을 통해 case 3으로 변환
	
	// 3) p = red , uncle = black (list)
	// -> 색상 변경 + 회전

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

				// List 타입
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

				// List 타입
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

// BST 삭제 실행
// case 1
//	삭제할 노드가 Red -> 삭제

// case 2
//  root가 DoubleBlack -> 추가 Black 삭제

// case 3
//  Double Black 의 sibling 노드가 Red
//  s = black, p = red ( s <-> p 색상교환)
//  DB방향으로 roatate(p)

// case 4
//  추가 Black을 parent에게 이전
//  // p 가 Red면 Black
//  // p 가 Black이면 Red
//  s = red
//  p를 대상으로 알고리즘 이어서 실행 (Double Black이 존재한다면)

// case 5
//  DB의 sibling 노드가 Black && sibling의 near child = red, far child = black
//  s <-> near 색상 교환
//  far 방향으로 rotate(s)
//  go to case 6

// case 6
// Double Black의 sibling노드가 Black && sibling의 far child = red
// far = black
// rotate(p) DB방향으로
// 추가 black 제거

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
		// 다음 데이터 찾기
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
			//  Double Black 의 sibling 노드가 Red
			//  s = black, p = red ( s <-> p 색상교환)
			//  DB방향으로 roatate(p)

			Node* s = x->parent->right;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color == Color::Red;

				LeftRotate(x->parent);

				s = x->parent->right;
			}

			// [Case 4]
			//  추가 Black을 parent에게 이전
			//  // p 가 Red면 Black
			//  // p 가 Black이면 Red
			//  s = red
			//  p를 대상으로 알고리즘 이어서 실행 (Double Black이 존재한다면)
			if (s->left->color == Color::Black && s->right->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//[Case 5]
				//  DB의 sibling 노드가 Black && sibling의 near child = red, far child = black
				//  s <-> near 색상 교환
				//  far 방향으로 rotate(s)
				//  go to case 6
				if (s->right->color == Color::Black)
				{
					s->left->color == Color::Black;
					s->color == Color::Red;
					RightRotate(s);
					s = x->parent->right;
				}

				//[Case 6]
				// Double Black의 sibling노드가 Black && sibling의 far child = red
				// far = black
				// rotate(p) DB방향으로
				// 추가 black 제거
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
			//  Double Black 의 sibling 노드가 Red
			//  s = black, p = red ( s <-> p 색상교환)
			//  DB방향으로 roatate(p)

			Node* s = x->parent->left;
			if (s->color == Color::Red)
			{
				s->color = Color::Black;
				x->parent->color == Color::Red;

				RightRotate(x->parent);

				s = x->parent->left;
			}

			// [Case 4]
			//  추가 Black을 parent에게 이전
			//  // p 가 Red면 Black
			//  // p 가 Black이면 Red
			//  s = red
			//  p를 대상으로 알고리즘 이어서 실행 (Double Black이 존재한다면)
			if (s->right->color == Color::Black && s->left->color == Color::Black)
			{
				s->color = Color::Red;
				x = x->parent;
			}
			else
			{
				//[Case 5]
				//  DB의 sibling 노드가 Black && sibling의 near child = red, far child = black
				//  s <-> near 색상 교환
				//  far 방향으로 rotate(s)
				//  go to case 6
				if (s->left->color == Color::Black)
				{
					s->right->color == Color::Black;
					s->color == Color::Red;
					RightRotate(s);
					s = x->parent->left;
				}

				//[Case 6]
				// Double Black의 sibling노드가 Black && sibling의 far child = red
				// far = black
				// rotate(p) DB방향으로
				// 추가 black 제거
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

// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
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



