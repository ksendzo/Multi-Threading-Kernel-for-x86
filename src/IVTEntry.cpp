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
#ifndef BCC_BLOCK_IGNORE
	System::lock();
	oldRout = getvect(numEntry);
	setvect(numEntry,newRout);
#endif
	myKernelEvent = 0;
	myEntry = numEntry;
	System::unlock();
//	IVTEntryArray[numEntry] = this;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	System::lock();
	setvect(myEntry,oldRout);
	System::unlock();
#endif
}


void IVTEntry::signal(){
	if(IVTEntryArray[myEntry])
		IVTEntryArray[myEntry]->signal();
	printf("IVTEntry signal\n");
										// Ovo mi deluje da je samo za tastaturu
//	asm {
//			in al, 61h		// slanje signala o pritisku
//			or al, 10000000b	// kontroleru tastature
//			out 61h, al		// bez modifikovanja
//			and al, 01111111b	// ostalih bita
//			out 61h, al		//
//
//			mov al, 20h		// slanje signala EOI (End-Of-Interrupt)
//			out 20h, al		// kontroleru prekida
//		}

}

void IVTEntry::callOld(){
	oldRout();
}

void IVTEntry::setKerEv(KernelEv* ke){
	myKernelEvent = ke;
}
