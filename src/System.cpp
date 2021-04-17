/*
 * System.cpp
 *
 *  Created on: Apr 17, 2021
 *      Author: OS1
 */

#include "System.h"
#include <dos.h>
#include "PCB.h"
//#include "aaaaaaaa.h"

extern volatile PCB* running;

System::System() {
	// TODO Auto-generated constructor stub

}

System::~System() {
	// TODO Auto-generated destructor stub
}


void inicTimer();
void restoreTimer();

void System::init() {
	inicTimer();
	running = new PCB;
}

void System::reset(){
	restoreTimer();
//	running->finished = 1;
	delete running;
}
