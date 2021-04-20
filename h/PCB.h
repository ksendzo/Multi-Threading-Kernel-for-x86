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
	PCB(StackSize, Time, Thread* = 0);
//	PCB();
	virtual ~PCB();

	enum State { CREATED, READY, FINISHED, BLOCKED };

	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;
	volatile unsigned* stack;
	volatile unsigned stackSize;
	volatile State state;
	volatile int timeSlice;

	volatile int isWaitingForSem;
	volatile int semWaitTimeLeft;
	volatile int unblockedBySignal;

	static ID idCnt;
	volatile ID id;

	volatile PCBStack *waitingForMeStack;

	Thread* myThread;
	static void wrapper();
	int tick() volatile;

	void start();
	void waitToComplete();
	static Thread* getThreadById(ID);

};

#endif /* PCB_H_ */
