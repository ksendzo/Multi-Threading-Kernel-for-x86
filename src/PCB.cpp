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
//	cout << "\n\t\t\tWrapper :D\n";

	if(System::running != 0 && System::running->id != 0){

		(System::running)->myThread->run();
		(System::running)->state = PCB::FINISHED;

		System::lock();
		while(!System::running->waitingForMeStack->isEmpty()){
			volatile PCB* temp = System::running->waitingForMeStack->pop();
			temp->state = READY;
			Scheduler::put((PCB*)temp);
//			printf("odblokirano %d\n", temp->id);
		}
//		printf("\n \t\t\t nit %d gotova\n", System::running->id);
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
	this->isWaitingForSem = 0;
	this->semWaitTimeLeft = 0;
	this->unblockedBySignal = 0;
	if(thread != 0){
		this->stack = new unsigned[stackSize];
	#ifndef BCC_BLOCK_IGNORE
		this->stack[stackSize - 1] = 0x200;

		this->stack[stackSize - 2] = FP_SEG(&(PCB::wrapper));
		this->stack[stackSize - 3] = FP_OFF(&(PCB::wrapper));

		this->ss = FP_SEG(stack + stackSize - 12);
		this->sp = FP_OFF(stack + stackSize - 12);
		this->bp = FP_OFF(stack + stackSize - 12);
	#endif
		this->state = CREATED;
	}
	else {
		this->stack = 0;
		this->state = READY;
//		printf("nulta nit kreirana\n");
	}
	System::listOfPCB->push(this);
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
	System::listOfPCB->removeMe(this);

	if(stack != 0)
		delete [] ((unsigned*)stack);
	System::unlock();

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
	if(this->state != FINISHED){
		System::running->state = BLOCKED;
		this->waitingForMeStack->push(System::running);
//		printf("nit %d nije gotova -> blokirana je %d\n", this->id, System::running->id);
		System::unlock();
		dispatch();
	}
	else
		System::unlock();

}


Thread* PCB::getThreadById(ID id){
	System::lock();
	volatile PCB *ret = System::listOfPCB->find(id);
	System::unlock();

	return ret->myThread;
}





int PCB::tick() volatile {
	if(this->state == BLOCKED && isWaitingForSem && semWaitTimeLeft > 0){
		semWaitTimeLeft--;
		//printf("ceka na semaforu %d\n", semWaitTimeLeft);
		if(semWaitTimeLeft == 0){
		//	printf("END SEM WAIT\n");
			state = READY;
			unblockedBySignal = 0;
			Scheduler::put((PCB*)this);
			return 1;
		}
	}
	return 0;
}





