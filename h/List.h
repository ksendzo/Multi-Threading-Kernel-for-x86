/*
 * List.h
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
#include <stdlib.h>
#include "prekidi.h"
#include "iostream.h"

template <class T>
class List{
public:
	List();
	List(const List<T>&);
	~List();

	void add(T* el);
	int remove(T* el);
	void copy(const List<T>&);
	void empty();

	void swap(T*,T*);
	T* getFirst()const;

	void begin();
	T* getCurr()const;
	void next();

	int isEmpty()const;

	void emptyAndDelete();


private:
	struct Node{
		T* info;
		Node* next;
		Node(T* i, Node* n = NULL):info(i), next(n){}
		~Node(){}
	};
	Node* head;
	Node* bottom;
	Node* current;
};

template <class T>
void List<T>::copy(const List<T>& l){
	empty();

	Node* newNode, *curL = l.head;
	head = NULL;
	bottom = NULL;
	while(curL){
		lock();
		newNode = new Node(curL->info, NULL);
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

template <class T>
List<T>::List(){
	head = NULL;
	bottom = NULL;
	current = NULL;
}

template <class T>
List<T>::List(const List<T>& l){
	this->copy(l);
}

template <class T>
List<T>::~List(){
	empty();
}

template <class T>
void List<T>::add(T* el){
	lock();
	Node* nod = new Node(el, NULL);
	unlock();
	if(bottom){
		bottom->next = nod;
		bottom = bottom->next;
	}
	else{
		head = nod;
		bottom = nod;
	}
}

template <class T>
int List<T>::remove(T* el){
	Node *cur = head;
	Node *prev = head;

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

template <class T>
void List<T>::empty(){
	while(head != NULL){
		Node* temp = head;
		head = head->next;
		temp->next = NULL;
		delete temp;
	}
	current = NULL;
	head = NULL;
	bottom = NULL;
}


template <class T>
void List<T>::emptyAndDelete(){
	while(head != NULL){
		Node* temp = head;
		head = head->next;
		temp->next = NULL;
		delete temp->info;
		delete temp;
	}
	current = NULL;
	head = NULL;
	bottom = NULL;
}

template <class T>
void List<T>::swap(T* e1, T* e2){
	Node* cur1 = head;
	Node* cur2 = head;
	while(cur1 && cur2 && (cur1->info != e1 || cur2->info != e2)){
		if(cur1->info != e1)
			cur1 = cur1->next;
		if(cur2->info != e2)
			cur2 = cur2->next;
	}
	if(cur1 && cur2){
		T* tr = cur1->info;
		cur1->info = cur2->info;
		cur2->info = tr;
	}
}

template <class T>
T* List<T>::getFirst()const{
	if(head)
		return head->info;
	return NULL;
}


template <class T>
void List<T>::begin(){
	current = head;
}


template <class T>
T* List<T>::getCurr()const{
	if(current)
		return current->info;
	return NULL;
}


template <class T>
void List<T>::next(){
	if(current)
		current = current->next;
}

template <class T>
int List<T>::isEmpty()const{
	if(head)
		return 0;
	else return 1;
}


#endif /* LIST_H_ */
