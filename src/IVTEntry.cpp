/*
 * IVTEntry.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "System.h"
#include <dos.h>
#include <stdio.h>
#include "KernelEv.h"

KernelEv* IVTEntry::IVTEntryArray[256] = {0};

IVTEntry::IVTEntry(int numEntry, pInterrupt newRout){
	System::lock();
#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(numEntry);
	setvect(numEntry,newRout);
#endif
	myEntryNo = numEntry;
	System::unlock();
}

IVTEntry::~IVTEntry(){
	System::lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntryNo,oldRout);
#endif
	System::unlock();
}


void IVTEntry::signal(){
	if(IVTEntryArray[myEntryNo])
		IVTEntryArray[myEntryNo]->signal();
}

void IVTEntry::callOld(){
	oldRout();
}
