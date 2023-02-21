#pragma once

enum class Color
{
	Red = 0,
	Black = 1,
};

struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		key = {};
	Color color = Color::Black;
};

//Red - Black Tree
// 1) ��� ���� Red Or Black
// 2) Root = Black
// 3) Leaf = Black
// 4) Red ����� �ڽ��� Black
// 5) �� ���κ��� �������� ���� ��ε��� ��� ���� ���� Black
class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();

	void	Print() { Print(_root, 10, 0); }
	void	Print(Node* node, int x, int y);

	Node*	Search(Node* node, int key);


	Node*	Min(Node* node);
	Node*	Max(Node* node);
	Node*	Next(Node* node);

	void	Insert(int key);
	void	InsertFixup(Node* node);

	void	Delete(int key);
	void	Delete(Node* node);
	void	DeleteFixup(Node* node);

	void	Replace(Node* u, Node* v);

	//RB TREE
	void LeftRotate(Node* node);
	void RightRotate(Node* node);

private:
	Node*	_root = nullptr;
	Node* _nil = nullptr;
};

