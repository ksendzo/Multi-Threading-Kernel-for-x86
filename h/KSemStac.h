/*
 * KSemStac.h
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#ifndef KSEMSTAC_H_
#define KSEMSTAC_H_

class KernelSem;

class KSemStack {
	struct Node {
			volatile KernelSem* info;
			Node *next;
			Node(volatile KernelSem* info, Node* next = 0):info(info), next(next){}
		};

	Node *topNode;
	Node *iterator;
public:
	KSemStack();
	virtual ~KSemStack();

	void push(volatile KernelSem*) volatile;
	volatile KernelSem* pop() volatile;
	volatile KernelSem* top() volatile;

	void removeMe(volatile KernelSem*) volatile;

	volatile KernelSem* first() volatile;
	volatile KernelSem* next() volatile;

	int isEmpty() volatile;

//	volatile KernelSem* find(ID id) volatile;
};

#endif /* KSEMSTAC_H_ */
