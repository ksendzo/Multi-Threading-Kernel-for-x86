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

void dispatch();

KernelSem::KernelSem(int init) {

	value = init;
	blockedList = new PCBStack;
}

KernelSem::~KernelSem() {
	delete blockedList;
}




int KernelSem::val() const {
	return value;
}


int KernelSem::wait(Time t){
	System::lock();
	value--;
	if(value >= 0){				// ima dovoljno val da samo nastavi daje
		System::unlock();
		return 0;				// VIDI STA VRACA
	}
	else {						// zablokira se
		System::running->state = PCB::BLOCKED;
		blockedList->push(System::running);

		if(t == 0){
			System::running->isWaitingForSem = 0;
		}
		else if(t < 0){
			printf("wait time < 0\n");
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
	if(value > 0){			// 	Niko nije bio blokiran
		System::unlock();
		return;
	}
	volatile PCB* sig = blockedList->pop();
	sig->unblockedBySignal = 1;
	sig->state = PCB::READY;
	Scheduler::put((PCB*)sig);
	System::unlock();

}
