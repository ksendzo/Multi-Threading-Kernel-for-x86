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

extern volatile PCB* running;
void dispatch();

void PCB::wrapper(){
	cout << "\n\t\t\tWrapper :D\n";

	if(running != 0){
		(running)->myThread->run();
		(running)->finished = 1;
	}
	dispatch();
}

PCB::PCB(Thread* thread, StackSize stackSize, Time timeSlice) {
	this->myThread = thread;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->stack = new unsigned[stackSize];
	this->waitingForMeStack = new PCBStack;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = 0x200;

	stack[stackSize - 2] = FP_SEG(&(PCB::wrapper));
	stack[stackSize - 3] = FP_OFF(&(PCB::wrapper));

	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = FP_OFF(stack + stackSize - 12);
#endif
	finished = 0;
}

PCB::PCB(){		// za pocetnu nit
	timeSlice = 2;
	finished = 0;
	myThread = 0;
}

PCB::~PCB() {
	if(stack != 0)
		delete [] ((unsigned*)stack);

}


void PCB::start(){
	cout << "Scheduler\n";
	Scheduler::put(this);
}


void PCB::waitToComplete(){
	while(!this->finished)
		dispatch();
}














