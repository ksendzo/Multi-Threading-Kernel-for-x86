/*
 * KerSem.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#include "KerSem.h"
#include "PCBStack.h"
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include "KSemStac.h"

void dispatch();

KernelSem::KernelSem(int init) {

	System::lock();
	value = init;
	blockedList = new PCBStack;

	if(System::listOfSemaphores == 0)
		System::listOfSemaphores = new KSemStack;
	System::listOfSemaphores->push(this);
	System::unlock();
}

KernelSem::~KernelSem() {
	System::lock();
	if(!blockedList->isEmpty())
		printf("ostalo je blokiranih niti na semaforu!\n");
	delete blockedList;
	System::listOfSemaphores->removeMe(this);
	if(System::listOfSemaphores->isEmpty()){
		delete System::listOfSemaphores;
		System::listOfSemaphores = 0;
	}
	System::unlock();
}


void KernelSem::tick() volatile{		//	tikni sve niti koje su na ovom sem
	//System::lock();

	PCBStack* PCBToRemove = new PCBStack;

	for(volatile PCB* i = blockedList->first(); i != 0; i = blockedList->next()){
		if(i->tick()) {
//			printf("to remove %d\n", i->id);
			PCBToRemove->push(i);
		}
	}

	volatile PCB* temp = PCBToRemove->pop();

	while(temp){
		temp->unblockedBySignal = 0;
		blockedList->removeMe(temp);
		value++;
//		printf("free\n");
		temp = PCBToRemove->pop();
	}

	delete PCBToRemove;

	//System::unlock();
}



int KernelSem::val() const {
	return value;
}


int KernelSem::wait(Time t){
	System::lock();
	value--;
//	printf("wait = %d\n", value);
	if(value >= 0){				// ima dovoljno val da samo nastavi daje
		System::unlock();
		return 1;				// VIDI STA VRACA
	}
	else {		// zablokira se
//		printf("wait\n");
		System::running->state = PCB::BLOCKED;
		blockedList->push(System::running);

		if(t == 0){
			System::running->isWaitingForSem = 0;
		}
		else {
			System::running->isWaitingForSem = 1;
			System::running->semWaitTimeLeft = t;
		}
		System::unlock();
		dispatch();
		if(System::running->unblockedBySignal == 1)
			return 1;
	}
	return 0;
}

void KernelSem::signal(){
	System::lock();
	value++;
//	printf("signal = %d\n", value);
	if(value > 0){			// 	Niko nije bio blokiran
		System::unlock();
		return;
	}
//	printf("signal\n");
	volatile PCB* sig = blockedList->pop();
	sig->unblockedBySignal = 1;
	sig->state = PCB::READY;
	Scheduler::put((PCB*)sig);
	System::unlock();

}
