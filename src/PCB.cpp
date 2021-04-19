/*
 * PCB.cpp
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include "System.h"
#include <dos.h>
#include "Thread.h"
#include <iostream.h>
#include "PCBStack.h"
#include "SCHEDULE.H"
#include <stdio.h>

void dispatch();

ID PCB::idCnt = 0;

void PCB::wrapper(){
	cout << "\n\t\t\tWrapper :D\n";

	if(System::running != 0){
		(System::running)->myThread->run();
		(System::running)->state = PCB::FINISHED;

		System::lock();
		while(!System::running->waitingForMeStack->isEmpty()){
			volatile PCB* temp = System::running->waitingForMeStack->pop();
			temp->state = READY;
			Scheduler::put((PCB*)temp);
		}
		System::unlock();

	}
	dispatch();
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {
	System::lock();
	this->id = idCnt++;
	this->myThread = thread;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->waitingForMeStack = new PCBStack;
	if(thread != 0){
		this->stack = new unsigned[stackSize];
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize - 1] = 0x200;

		stack[stackSize - 2] = FP_SEG(&(PCB::wrapper));
		stack[stackSize - 3] = FP_OFF(&(PCB::wrapper));

		ss = FP_SEG(stack + stackSize - 12);
		sp = FP_OFF(stack + stackSize - 12);
		bp = FP_OFF(stack + stackSize - 12);
	#endif

		state = CREATED;
	}
	else {
		state = READY;
	}
	System::ListOfPCB->push(this);
	System::unlock();
}

//PCB::PCB(){		// za pocetnu nit
//	System::lock();
////	timeSlice = 2;
//	state = READY;
//	myThread = 0;
//	System::ListOfPCB->push(this);
//	waitingForMeStack = new PCBStack;
//	id = idCnt++;
//	System::unlock();
//}

PCB::~PCB() {
	waitToComplete();
	System::lock();
	System::ListOfPCB->removeMe(this);
	System::unlock();
	if(stack != 0)
		delete [] ((unsigned*)stack);

}


void PCB::start(){
	cout << "Scheduler\n";
	state = READY;
	Scheduler::put(this);
}


void PCB::waitToComplete(){
	System::lock();

	if(System::running == this && System::running != System::mainPCB){		// ovo ne bi trebalo nikad da se desi...
		printf("zoves waitToComplete samog sebe...\n");

	}
	if(state != FINISHED){
		System::running->state = BLOCKED;
		waitingForMeStack->push(System::running);
		System::unlock();
		dispatch();
	}
	else
		System::unlock();

}


Thread* PCB::getThreadById(ID id){
	System::lock();
	volatile PCB *ret = System::ListOfPCB->find(id);
	System::unlock();

	return ret->myThread;
}











