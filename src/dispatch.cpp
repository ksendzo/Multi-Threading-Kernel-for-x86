/*
 * dispatch.cpp
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */
//#include "SCHEDULE.H"
//#include "PCB.h"
#include <iostream.h>
#include "System.h"
#include <stdio.h>

void interrupt timer();

void dispatch(){

	System::lock();
//	cout << "\t\t\t\tdispatch\n";
	System::isDispatch = 1;
	System::unlock();
//	printf("w\n");
	asm int 08h;
//	printf("f\n");
}


