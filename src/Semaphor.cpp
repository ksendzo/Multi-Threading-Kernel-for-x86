/*
 * Semaphor.cpp
 *
 *  Created on: Apr 18, 2021
 *      Author: OS1
 */

#include "Semaphor.h"
#include "KerSem.h"

Semaphore::Semaphore(int init) {

	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	// TODO Auto-generated destructor stub
}


int Semaphore::val() const {
	return myImpl->val();
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}


void Semaphore::signal(){
	myImpl->signal();
}
