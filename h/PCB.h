/*
 * PCB.h
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

class Thread;
class PCBStack;

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;


class PCB {
public:
	PCB(Thread*, StackSize, Time);
	PCB();
	virtual ~PCB();

	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;
	volatile unsigned* stack;
	volatile unsigned stackSize;
	volatile int finished;
	volatile int timeSlice;
	volatile ID id;

	volatile PCBStack *waitingForMeStack;

	Thread* myThread;
	static void wrapper();

	void start();
	void waitToComplete();

};

#endif /* PCB_H_ */
