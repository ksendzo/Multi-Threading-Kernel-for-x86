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
	this->ivtNo = ivtNo;
	this->blocked = 0;
	IVTEntry::IVTEntryArray[ivtNo] = this;
//	printf("KernelEv added to IVTEntryArray[%d]\n", ivtNo);
//	for(int i = 0; i < 256; i ++)
//			if(IVTEntry::IVTEntryArray[i] != 0) printf("%d  -> %d\t", i, IVTEntry::IVTEntryArray[i]);
	System::unlock();
}

KernelEv::~KernelEv() {
	System::lock();
	IVTEntry::IVTEntryArray[ivtNo] = 0;
//	printf("KernelEv removied from IVTEntryArray[%d]\n", ivtNo);
	System::unlock();
}


void KernelEv::wait() {
	System::lock();
	if(creatorPCB != System::running){
		System::unlock();
		return;
	}
	if(blocked) {
//		value = 0;
		System::unlock();
		return;
	}
	else {
		creatorPCB->state = PCB::BLOCKED;
		blocked = 1;
		System::unlock();
		dispatch();
	}
}


void KernelEv::signal() {
	if(blocked) {
		creatorPCB->state = PCB::READY;
		Scheduler::put((PCB*)creatorPCB);
		blocked = 0;
	}
}
