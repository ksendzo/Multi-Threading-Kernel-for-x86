/*
 * KernelEv.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

//#include "IVTEntry.h"

typedef unsigned char IVTNo;


class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
	friend class IVTEntry;
private:
	PCB* creator;
	PCB* blockedThread;
	int value;
};

//extern KernelEv* IVTEntryArray[];

KernelEv::KernelEv(IVTNo ivtNo){
	blockedThread = NULL;
	creator = running;
	value = 0;

	//postavi se IVTEntry-u
	IVTEntryArray[ivtNo] = this;
}

KernelEv::~KernelEv(){
	creator = NULL;
}

void KernelEv::wait(){
	if(value)
		value = 0;
	else if(running == creator){
		blockedThread = creator;
		creator->state = PCB::BLOCKED;
		dispatch();
	}
}

void KernelEv::signal(){
	if(blockedThread){ // da li treba da proverim ko je pozvao signal?
		blockedThread->state = PCB::ACTIVE;
		Scheduler::put(blockedThread);
		blockedThread = NULL;
	}
	else
		value = 1;
}


#endif /* KERNELEV_H_ */
