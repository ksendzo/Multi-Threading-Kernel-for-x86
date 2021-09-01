/*
 * System.cpp
 *
 *  Created on: Apr 17, 2021
 *      Author: OS1
 */

#include "System.h"
#include <dos.h>
#include "PCB.h"
#include "PCBStack.h"
#include "Idle.h"
#include <stdio.h>
#include "KSemStac.h"
#include "KerSem.h"

volatile PCB* System::running = 0;
volatile PCB* System::mainPCB = 0;
volatile Idle* System::idle = 0;
PCBStack* System::listOfPCB = 0;
KSemStack* System::listOfSemaphores = 0;

volatile pInterrupt System::oldTimer = 0;
volatile pInterrupt System::old96 = 0;


volatile int System::isDispatch = 0;
volatile int System::lockCnt = 0;

void inicTimer();
void restoreTimer();
void interrupt timer(...);

void System::init() {
//	inicTimer();
	oldTimer = getvect(8);
	setvect(8, timer);
	old96 = getvect(96);
	setvect(96, oldTimer);
	System::running = System::mainPCB = new PCB(0, 1);
	System::idle = new Idle;
	System::listOfPCB = new PCBStack;
}

void System::reset(){
//	restoreTimer();
	setvect(8, oldTimer);
	setvect(96, old96);
	System::mainPCB->state = PCB::FINISHED;
	delete System::mainPCB;
//	delete System::running;
	delete System::idle;
	if(System::listOfPCB){
		if(System::listOfPCB->isEmpty())
			delete System::listOfPCB;
//			printf("lista PCB nije prazna\n");

	}
	if(System::listOfSemaphores){
		if(System::listOfSemaphores->isEmpty())
			delete System::listOfSemaphores;
	}
}


volatile unsigned int flagVal = 0;

void System::lock() {
	if(lockCnt == 0){
		asm {
			pushf
			pop flagVal
			cli
		}
//		printf("\n.");
//		asm cli;
	}

	lockCnt--;
//	printf("-%d-\n", lockCnt);
}

void System::unlock(){
	if(lockCnt < 0)
		lockCnt++;
	if(lockCnt == 0){
//		printf("\nunlock\n");
		asm {
			push flagVal
			popf
		}
	}
	else if(isDispatch)
		printf("DISPATCH LOCKED\n");
//		asm popf;
//	printf("+%d+\n", lockCnt);

}


void System::tickSemaphores(){
	//lock();

	for(volatile KernelSem *i = listOfSemaphores->first(); i != 0; i = listOfSemaphores->next()){
		i->tick();
	}

	//unlock();
}



