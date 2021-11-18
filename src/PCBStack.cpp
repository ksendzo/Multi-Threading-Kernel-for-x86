/*
 * PCBList.cpp
 *
 *  Created on: Apr 17, 2021
 *      Author: OS1
 */

#include "../h/PCBStack.h"
#include "PCB.h"

PCBStack::PCBStack() {
	top = 0;
	bottom = 0;
	iterator = 0;
}

PCBStack::~PCBStack() {

}



void PCBStack::push(volatile PCB* newPCB) volatile {
	top = new Node(newPCB, top);
	if(bottom == 0)
		bottom = top;
}

volatile PCB* PCBStack::pop() volatile {
	if(isEmpty())
		return 0;

	Node* temp = top;
	volatile PCB* ret;

	if(top->next == 0){
		ret = top->info;
		delete top;
		top = 0;
	}
	else {
		while(temp->next->next != 0){
			temp = temp->next;
		}
		ret = temp->next->info;
		delete temp->next;
		temp->next = 0;
	}

	return ret;
}


void PCBStack::removeMe(volatile PCB* me) volatile {
	Node* temp = top;
	Node* last = top;

	while(temp && temp->info != me){
		last = temp;
		temp = temp->next;
	}

	if(!temp)
		return;

	if(temp == top)
		top = top->next;
	else
		last->next = temp->next;

	temp->next = 0;
	delete temp;
}


int PCBStack::isEmpty() volatile {
	if(top == 0)
		return 1;
	else
		return 0;
}


volatile PCB* PCBStack::find(ID id) volatile{
	Node *temp = top;

	while(temp != 0){
		if(temp->info != 0 && temp->info->id == id)
			return temp->info;
		temp = temp->next;
	}
	return 0;
}


volatile PCB* PCBStack::first() volatile {
	iterator = top;
	if(iterator)
		return iterator->info;
	return 0;
}

volatile PCB* PCBStack::next() volatile {
	if(!iterator)
		return 0;
	iterator = iterator->next;
	if(iterator)
		return iterator->info;
	return 0;
}


