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
		volatile PCB* info;
		Node *next;
		Node(volatile PCB* info, Node* next = 0):info(info), next(next){}
	};

	Node *top;
public:
	PCBStack();
	virtual ~PCBStack();

	void push(volatile PCB*) volatile;
	volatile PCB* pop() volatile;
	void removeMe(volatile PCB*) volatile;

	int isEmpty() volatile;

	volatile PCB* find(ID id) volatile;
};

#endif /* PCBLIST_H_ */
