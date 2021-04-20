/*
 * KSemStac.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#include "KSemStac.h"
#include "KerSem.h"


KSemStack::KSemStack() {
	topNode = 0;
}

KSemStack::~KSemStack() {

}



void KSemStack::push(volatile KernelSem* newKSem) volatile {
	topNode = new Node(newKSem, topNode);
}

volatile KernelSem* KSemStack::pop() volatile {
	if(isEmpty())
		return 0;

	Node *temp = topNode;
	topNode = topNode->next;
	volatile KernelSem* ret = temp->info;
	temp->next = 0;
	temp->info = 0;
	delete temp;

	return ret;
}


void KSemStack::removeMe(volatile KernelSem* me) volatile {
	Node* temp = topNode;
	Node* last = topNode;

	while(temp && temp->info != me){
		last = temp;
		temp = temp->next;
	}

	if(!temp)
		return;

	if(temp == topNode)
		topNode = topNode->next;
	else
		last->next = temp->next;

	temp->next = 0;
	delete temp;
}


int KSemStack::isEmpty() volatile {
	if(topNode == 0)
		return 1;
	else
		return 0;
}

volatile KernelSem* KSemStack::top() volatile{
	if(!topNode)
		return 0;
	else
		return topNode->info;
}



//volatile KernelSem* KSemStack::find(ID id) volatile{
//	Node *temp = topNode;
//
//	while(temp != 0){
//		if(temp->info != 0 && temp->info->id == id)
//			return temp->info;
//		temp = temp->next;
//	}
//	return 0;
//}


