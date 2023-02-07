#pragma once
#include "Node.h"
class LinkedList
{
public:
	Node* Head;
	int length;
	LinkedList();
	void insertHead(Enemy* G);
	void deleteIndex(int i);
	Enemy* at(int i);

};

