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
//	printf("created Kernel Event %d \n", ivtNo);
	this->ivtNo = ivtNo;
	this->blocked = 0;
	IVTEntry::IVTEntryArray[ivtNo] = this;
	System::unlock();
}

KernelEv::~KernelEv() {
	IVTEntry::IVTEntryArray[ivtNo] = 0;
}


void KernelEv::wait() {
	System::lock();
	if(creatorPCB != System::running){
//		printf("pogresna nit\n");
		System::unlock();
		return;
	}
	if(blocked) {
//		value = 0;
//		printf("duplo blokiranje, nesto ne valja\n");
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
//	System::lock();
	if(blocked) {
//		printf("Event signal unblock\n");
		creatorPCB->state = PCB::READY;
		Scheduler::put((PCB*)creatorPCB);
		blocked = 0;
	}
	else {
//		printf("Event signal no blocked\n");
	}
//	System::unlock();
}
