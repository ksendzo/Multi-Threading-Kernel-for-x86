/*
 * KerSem.cpp
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#include "KerSem.h"

KernelSem::KernelSem(int init) {

	value = init;
}

KernelSem::~KernelSem() {
	// TODO Auto-generated destructor stub
}




int KernelSem::val() const {
	return value;
}

