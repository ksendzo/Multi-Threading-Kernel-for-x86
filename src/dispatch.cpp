/*
 * dispatch.cpp
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */

#include <iostream.h>
#include "System.h"
#include <stdio.h>

void interrupt timer();

void dispatch(){

	System::lock();
	System::isDispatch = 1;
	System::unlock();

	asm int 08h;
}


