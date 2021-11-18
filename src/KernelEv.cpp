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

void dispatch();

KernelEv::KernelEv(IVTNo ivtNo) {
	System::lock();
	creatorPCB = System::running;
	this->ivtNo = ivtNo;
	this->blocked = 0;
	IVTEntry::IVTEntryArray[ivtNo] = this;
	System::unlock();
}

KernelEv::~KernelEv() {
	System::lock();
	IVTEntry::IVTEntryArray[ivtNo] = 0;
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
