#pragma once

class Node
{
public:
	Node* Next;
	int age;
	int lives;

	Node(int g) {
		Next = nullptr;
		age = g;
		if (age > 69) {
			lives = 5;
		}
	}

};

