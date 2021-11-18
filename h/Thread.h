/*
 * Thread.h
 *
 *  Created on: Apr 15, 2020
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_
#include <stdio.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "pcb.h"
#include "prekidi.h"

typedef void (*SignalHandler) ();
typedef unsigned SignalId;

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;

typedef unsigned int Time;
const Time defaultTimeSlice = 2;

typedef int ID;

class Thread;

struct ThreadNode{
	Thread* info;
	ThreadNode* next;
};


class Thread{
public:
	void start();

	void waitToComplete();

	virtual ~Thread();

	ID getId();
	static ID getRunningId();

	static Thread* getThreadById(ID id);

	int getTimeSlice();

	void _run();
	static void wrapper();


	void signal(SignalId singnal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run(){}

private:

	PCB* myPCB;

	static ThreadNode* allThreads;

};



void dispatch();


ThreadNode* Thread::allThreads = NULL;


extern PCB* running, *nultaNit;

void Thread::wrapper(){
	running->myThread->_run();
}


void Thread::waitToComplete(){
	myPCB->waitToComplete();
}


Thread::Thread(StackSize stackSize, Time timeSlice){
		lock();
		myPCB = new PCB;

		unsigned* st = new unsigned[stackSize/sizeof(unsigned)];
		unlock();
		#ifndef BCC_BLOCK_IGNORE
		 /* st[stackSize - 1] = FP_SEG(wrapper);
		  st[stackSize - 2] = FP_OFF(wrapper);

		  // pocetni kontekst
		  //(proizvoljne vrednosti)
		  // st[1023-1018] = {PC,BP,AX, BX, CX,DX}
		  myPCB->sp = FP_OFF(st + stackSize - 8);
		  myPCB->ss = FP_SEG(st + stackSize - 8); */

		st[stackSize - 1] = FP_SEG(this);
		st[stackSize - 2] = FP_OFF(this);

		st[stackSize - 5] = 512;

		st[stackSize - 6] = FP_SEG(wrapper);
		st[stackSize - 7] = FP_OFF(wrapper);

		myPCB->sp = FP_OFF(st + stackSize - 16);
		myPCB->ss = FP_SEG(st + stackSize - 16);
		#endif
	  myPCB->stack = st;
	  myPCB->timeSlice = timeSlice;
	  myPCB->state = PCB::ACTIVE;
	  myPCB->myThread = this;

	  lock();
	  ThreadNode* newNode = new ThreadNode;
	  unlock();
	  newNode->info = this;
	  newNode->next = allThreads;
	  allThreads = newNode;


	}

void Thread::start(){
	Scheduler::put(myPCB);
}

void Thread::_run(){
	run();
	myPCB->endThread();
	dispatch();
}




Thread::~Thread(){
	waitToComplete();

	lock()
	ThreadNode* temp = allThreads;
	ThreadNode* prev = temp;
	while(temp != NULL && temp->info->getId() != myPCB->myID){
		prev = temp;
		temp = temp->next;
	}

	if(temp != NULL){
		if(temp == allThreads)
			allThreads = temp->next;

		prev->next = temp->next;
		delete temp;
	}
	unlock();

	delete myPCB;
}


ID Thread::getId(){
	return myPCB->myID;
}

ID Thread::getRunningId(){
	extern PCB* running;
	return running->myID;
}

int Thread::getTimeSlice(){
	return myPCB->timeSlice;
}

Thread* Thread::getThreadById(ID id){
	ThreadNode* temp = allThreads;
	while(temp != NULL && temp->info->getId() != id)
		temp = temp->next;
	return temp->info;
}




extern volatile int signal_u_toku;
void Thread::signal(SignalId sign){
	//signal_u_toku = 1;
	myPCB->signal(sign);

	//signal_u_toku = 0;
}

void Thread::registerHandler(SignalId sig, SignalHandler handler){
	myPCB->registerHandler(sig, handler);
}

void Thread::unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id, hand1, hand2);
}


void Thread::blockSignal(SignalId sig){
	myPCB->blockSignal(sig);
}

void Thread::blockSignalGlobally(SignalId sig){
	PCB::blockSignalGlobally(sig);
}

void Thread::unblockSignal(SignalId sig){
	myPCB->unblockSignal(sig);
}

void Thread::unblockSignalGlobally(SignalId sig){
	PCB::unblockSignalGlobally(sig);
}






#endif /* THREAD_H_ */
