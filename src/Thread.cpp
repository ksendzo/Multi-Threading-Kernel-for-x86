/*
 * Thread.cpp
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */

#include "Thread.h"
//#include "aaaaaaaa.h"
#include "PCB.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include <stdio.h>
#include "System.h"

extern volatile PCB* running;

void dispatch();


Thread::Thread(StackSize stackSizeParam, Time timeSlice) {
	myPCB = new PCB(this,
				((stackSizeParam<maxStackSize)?stackSizeParam:maxStackSize) / sizeof(unsigned),
				timeSlice);
}

Thread::~Thread() {
	if(myPCB != 0)
		delete myPCB;
}


void Thread::start(){
	if(myPCB != 0)
		((PCB*)myPCB)->start();
}


void Thread::waitToComplete(){
	if(myPCB == 0)
		printf("PCB == 0 waitToComplete\n");
	else
		myPCB->waitToComplete();
}


ID Thread::getId(){
	if(myPCB)
		return myPCB->id;
}


ID Thread::getRunningId(){
	return running->id;
}

Thread* Thread::getThreadById(ID id){
	return NULL;
}
