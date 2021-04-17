/*
 * PCBStack.h
 *
 *  Created on: Apr 17, 2021
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

class PCB;

typedef int ID;

class PCBStack {
	struct Node {
		PCB* info;
		Node *next;
		Node(PCB* info, Node* next = 0):info(info), next(next){}
	};

	Node *top;
public:
	PCBStack();
	virtual ~PCBStack();

	void push(PCB*);
	PCB* pop();

	int isEmpty();

	PCB* find(ID id);
};

#endif /* PCBLIST_H_ */
