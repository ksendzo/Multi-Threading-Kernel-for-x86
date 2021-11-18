//  v1_zad4.cpp
//  prevodjenje iz komandne linije: bcc -ms -Ic:\bc31\include -Lc:\bc31\lib v1_zad4.cpp

#include "aaaaaaaa.h"

#include <stdio.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "pcb.h"
#include "thread.h"
#include "iostream.h"
#include "prekidi.h"
#include <STDARG.H>

#include <stdlib.h>

#include "semaphor.h"
#include "SysThr.h"
#include "syncPr.h"
#include "user.h"
#include "timer.h"



void init(){
	::cntID = 0;

	for(int i = 0; i < 16; i++)
		globalBlockedSignals[i] = 0;

	nultaNit = NULL;
	running = NULL;
	lock();
	nultaNit = new PCB;
	unlock();

	running = nultaNit;
	nultaNit->state = PCB::FINISHED;
	inicTimer();
}

void reset(){
	delete nultaNit;
	restoreTimer();

}


int main(int argc, char** argv){

	for(int i = 0; i <1; i++){
		init();
		lock()
		SystemThread* st = new SystemThread(argc, argv);
		unlock();
		st->start();
		delete st;

		reset();
	}

	delete listOfKernelSem; // ako ubacujem main u petlju ovo mora da ide NA KRAJU
	listOfKernelSem = NULL;
	return 0;
}
