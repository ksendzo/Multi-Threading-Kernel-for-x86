/*
 * Semaphor.h
 *
 *  Created on: Apr 18, 2021
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

class KernelSem;

class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual void signal();

	int val() const;

private:
	KernelSem* myImpl;
};

#endif /* SEMAPHOR_H_ */
