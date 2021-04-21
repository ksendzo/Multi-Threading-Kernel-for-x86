/*
 * Event.cpp
 *
 *  Created on: Apr 20, 2021
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"
#include "System.h"

Event::Event(IVTNo ivtNo) {
	System::lock();
	myImpl = new KernelEv(ivtNo);
	System::unlock();

}

Event::~Event() {
	System::lock();
	delete myImpl;
	System::unlock();
}


void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
