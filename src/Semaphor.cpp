/*
 * Semaphor.cpp
 *
 *  Created on: Apr 18, 2021
 *      Author: OS1
 */

#include "Semaphor.h"
#include "System.h"
#include "KerSem.h"

Semaphore::Semaphore(int init) {
	System::lock();
	myImpl = new KernelSem(init);
	System::unlock();
}

Semaphore::~Semaphore() {
	System::lock();
	delete myImpl;
	System::unlock();
}


int Semaphore::val() const {
	return myImpl->val();
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}


void Semaphore::signal(){
	System::lock();
	myImpl->signal();
	System::unlock();
}
