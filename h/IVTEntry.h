/*
 * IVTEntry.h
 *
 *  Created on: Apr 20, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

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
	static KernelEv* IVTEntryArray[256];
private:
	pInterrupt oldRout;
	KernelEv* myKernelEvent;
	int myEntry;

};

#endif /* IVTENTRY_H_ */
