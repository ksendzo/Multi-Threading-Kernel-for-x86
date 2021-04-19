/*
 * Idle.h
 *
 *  Created on: Apr 18, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.h"

//class Thread;

class Idle: public Thread {
public:
	Idle();
	virtual ~Idle();
protected:
	virtual void run();
};

#endif /* IDLE_H_ */
