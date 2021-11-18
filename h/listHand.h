/*
 * listHand.h
 *
 *  Created on: Aug 27, 2020
 *      Author: OS1
 */

#ifndef H_LISTHAND_H_
#define H_LISTHAND_H_
#include <stdlib.h>

typedef void (*SignalHandler) ();

struct handNode{
	SignalHandler info;
	handNode* next;
	handNode(SignalHandler s, handNode* n){info = s; next = n;}
};

class ListHandler{
public:
	void push(SignalHandler s);
	SignalHandler pop();

	void begin();
	SignalHandler getCurrent();
	void next();

	void swap(SignalHandler h1, SignalHandler h2);
	void copy(ListHandler&);
	void empty();
	int remove(SignalHandler);

	ListHandler();

private:
	handNode* head, *bottom, *current;
};

ListHandler::ListHandler(){
	head = NULL;
	bottom = NULL;
	current = NULL;
}

void ListHandler::push(SignalHandler s){
	handNode* newNode = new handNode(s, NULL);
	if(head){
		bottom->next = newNode;
		bottom = bottom->next;
	}
	else{
		head = newNode;
		bottom = newNode;
		current = head;
	}
}

SignalHandler ListHandler::pop(){
	if(!head)
		return NULL;

	handNode* tr = head;
	head = head->next;
	if(tr == bottom)
		bottom = bottom->next;
	if(current == tr)
		current = current->next;
	SignalHandler sh = tr->info;
	delete tr;
	return sh;

}

void ListHandler::begin(){
	current = head;
}

SignalHandler ListHandler::getCurrent(){
	if (current)
		return current->info;
	return NULL;
}


void ListHandler::copy(ListHandler& l){
	empty();

	handNode* newNode, *curL = l.head;
	head = NULL;
	bottom = NULL;
	while(curL){
		lock();
		newNode = new handNode(curL->info, NULL);
		unlock();
		if(!head){
			head = newNode;
			bottom = head;
		}
		else{
			bottom->next = newNode;
			bottom = bottom->next;
		}
		curL = curL->next;
	}
	current = head;
}


void ListHandler::empty(){
	while(head != NULL){
		handNode* temp = head;
		head = head->next;
		temp->next = NULL;
		delete temp;
	}
	current = NULL;
	head = NULL;
	bottom = NULL;
}

void ListHandler::swap(SignalHandler e1, SignalHandler e2){
	handNode* cur1 = head;
	handNode* cur2 = head;
	while(cur1 && cur2 && (cur1->info != e1 || cur2->info != e2)){
		if(cur1->info != e1)
			cur1 = cur1->next;
		if(cur2->info != e2)
			cur2 = cur2->next;
	}
	if(cur1 && cur2){
		SignalHandler tr = cur1->info;
		cur1->info = cur2->info;
		cur2->info = tr;
	}
}

void ListHandler::next(){
	if(current)
		current = current->next;
}

int ListHandler::remove(SignalHandler el){
	handNode *cur = head;
	handNode *prev = head;

	while((cur != NULL) && (cur->info != el)){
		prev = cur;
		cur = cur->next;
	}

	if(cur && cur->info == el){ // postoji element za brisanje
		if(head == cur)
			head = cur->next;
		if(current == cur)
			current = cur->next;
		if(bottom == cur){
			if(head)
				bottom = prev;
			else
				bottom = NULL;
		}
		prev->next = cur->next;
		cur->next = NULL;
		delete cur;
		return 1;
	}
	return 0;
}


#endif /* H_LISTHAND_H_ */
