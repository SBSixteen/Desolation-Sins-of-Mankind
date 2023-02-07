#include "LinkedList.h"

LinkedList::LinkedList() {

	Head = NULL;
	length = 0;
}

void LinkedList::insertHead(Enemy* G) {

	Node* temp = new Node(G);
	if (Head == NULL) {
		Head = temp;
		length++;
		return;
	}
	else {
		temp->Next = Head;
		Head = temp;
		length++;
		return;
	}
}

Enemy* LinkedList::at(int i) {

	int count = 0;
	Node* temp = Head;
	while (count < i) {
		temp = temp->Next;
		count++;
	}
	return temp->G;	
}

void LinkedList::deleteIndex(int index) {

	Node* temp = Head;

	if (length == 0) {
		free(temp);
		return;
	}

	if (length == 1) {
		Head = NULL;
		length--;
		free(temp);
		return;
	}

	if (index == 0) {
		Head = temp->Next;
		free(temp);
		length--;
		return;
	}

	if (index == length - 1) {

		while (temp->Next != NULL) {
			temp = temp->Next;
		}
		temp = NULL;
		free(temp);
		length--;
		return;
	}

	int i = 0;

	while (i < index-1) {
		temp = temp->Next;
		i++;
	}

	Node* del = temp->Next;
	temp->Next = del->Next;
	free(del);
	length--;
	return;


}


