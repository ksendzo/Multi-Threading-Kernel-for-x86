/*
 * Event.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "KernelEv.h"
#include "IVTEntry.h"

typedef unsigned char IVTNo;
//class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	//friend class IVTEntry;
	void signal(); // can call Kernel

private:
	KernelEv* myImpl;
};

Event::Event(IVTNo ivtNo){
	lock();
	myImpl = new KernelEv(ivtNo);
	unlock();
}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}


#endif /* EVENT_H_ */
