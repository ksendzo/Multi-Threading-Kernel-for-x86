/*
 * IVTEntry.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "dos.h"
#include "KernelEv.h"
typedef unsigned char IVTNo;


typedef void interrupt (*pInterrupt) (...);
//class IVTEntry;
class KernelEv;

#define PREPAREENTRY(numEntry, callOldFlag)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if (callOldFlag == 1) newEntry##numEntry.callOld();\
}

//KernelEv* IVTEntryArray[256];

class IVTEntry{
public:
	IVTEntry(int, pInterrupt);
	~IVTEntry();
	void signal();
	void callOld();
	void setKerEv(KernelEv*);
private:
	pInterrupt oldRout;
	KernelEv* myKernelEvent;
	int myEntry;
};

IVTEntry::IVTEntry(int numEntry, pInterrupt newRout){
#ifndef BCC_BLOCK_IGNORE
	lock();
	oldRout = getvect(9);
	setvect(9,newRout);
	unlock();
#endif
	myKernelEvent = NULL;
	myEntry = numEntry;
//	IVTEntryArray[numEntry] = this;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(9,oldRout);
	unlock();

#endif
}


void IVTEntry::signal(){
	if(IVTEntryArray[myEntry])
		IVTEntryArray[myEntry]->signal();
	asm {
			in al, 61h		// slanje signala o pritisku
			or al, 10000000b	// kontroleru tastature
			out 61h, al		// bez modifikovanja
			and al, 01111111b	// ostalih bita
			out 61h, al		//

			mov al, 20h		// slanje signala EOI (End-Of-Interrupt)
			out 20h, al		// kontroleru prekida
		}

}

void IVTEntry::callOld(){
	oldRout();
}

void IVTEntry::setKerEv(KernelEv* ke){
	myKernelEvent = ke;
}


#endif /* IVTENTRY_H_ */
