#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
#include "kerSem.h"
typedef unsigned int Time;

class KernelSem;

class Semaphore{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);

	int val() const;
private:
	KernelSem* myImpl;
};



Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

Semaphore::~Semaphore(){
	delete myImpl;
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const{
	return myImpl->getVal();
}

#endif
