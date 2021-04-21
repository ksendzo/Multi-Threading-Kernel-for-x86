/*
 * KernelEv.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: OS1
 */

#include "KernelEv.h"
#include "System.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "IVTEntry.h"
#include <stdio.h>

void dispatch();

KernelEv::KernelEv(IVTNo ivtNo) {
	System::lock();
	creatorPCB = System::running;
	printf("created Kernel Event %d \n", ivtNo);
	this->ivtNo = ivtNo;
	this->value = 0;
	this->blocked = 0;
	IVTEntry::IVTEntryArray[ivtNo] = this;
	System::unlock();
}

KernelEv::~KernelEv() {
	// TODO Auto-generated destructor stub
}


void KernelEv::wait() {
	System::lock();
	if(creatorPCB != System::running){
//		printf("pogresna nit\n");
		System::unlock();
		return;
	}
	if(value) {
		value = 0;
//		printf("samo proslo\n");
		System::unlock();
		return;
	}
	else {
		creatorPCB->state = PCB::BLOCKED;
		blocked = 1;
//		printf("blkirao nit\n");
		System::unlock();
		dispatch();
	}
}


void KernelEv::signal() {
	System::lock();

	if(blocked == 0) {
		value = 1;
		printf("Event signal no blocked\n");
	}
	else {
		printf("Event signal unblock\n");
		creatorPCB->state = PCB::READY;
		Scheduler::put((PCB*)creatorPCB);
		blocked = 0;
	}
	System::unlock();
}
