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

}

PCBStack::~PCBStack() {



}



void PCBStack::push(PCB* newPCB){
	top = new Node(newPCB, top);
}

PCB* PCBStack::pop() {
	if(isEmpty())
		return 0;

	Node *temp = top;
	top = top->next;
	temp->next = 0;
	PCB* ret = temp->info;
	delete temp;

	return ret;
}


int PCBStack::isEmpty(){
	if(top == 0)
		return 1;
	else
		return 0;
}


PCB* PCBStack::find(ID id){
	Node *temp = top;

	while(temp != 0){
		if(temp->info != 0 && temp->info->id == id)
			return temp->info;
	}
	return 0;
}







