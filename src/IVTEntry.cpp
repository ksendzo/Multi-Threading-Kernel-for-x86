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
//	for(int i = 0; i < 256; i++)
//			printf("%d:%d\t", i, getvect(i));
	oldRout = getvect(numEntry);
//	printf("\t_%d_\n", oldRout);
	setvect(numEntry,newRout);
#endif
	myEntryNo = numEntry;
	System::unlock();
}

IVTEntry::~IVTEntry(){
	System::lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(myEntryNo,oldRout);
//	for(int i = 0; i < 256; i++)
//				printf("%d:%d\t", i, getvect(i));
//	pInterrupt newRout = getvect(myEntryNo);
//	printf("\t_%d_\n", newRout);
#endif
//	printf("IVTEntry deleted\n");
	System::unlock();
}


void IVTEntry::signal(){
	if(IVTEntryArray[myEntryNo]){
//		printf("\n\t\tSIGNAL CALLED\n");
		IVTEntryArray[myEntryNo]->signal();
	}
//	else {
//		printf("IVTEntry DOES NOT EXIST\n");
//	}
}

void IVTEntry::callOld(){
//	printf("callOld\n");
	oldRout();
}
