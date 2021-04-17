/*
 * dispatch.cpp
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */
//#include "SCHEDULE.H"
//#include "PCB.h"
#include <iostream.h>

class PCB;

void interrupt timer();
extern volatile unsigned tsp, tss;
//extern volatile PCB* running;
extern volatile int context_switch_on_demand;


void dispatch(){

	asm cli;
	cout << "\t\t\t\tdispatch\n";
	context_switch_on_demand = 1;
	asm sti;
	asm int 08h;


}


