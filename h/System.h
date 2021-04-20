/*
 * System.h
 *
 *  Created on: Apr 17, 2021
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

class PCB;
class PCBStack;
class Idle;
class KSemStack;

class System {
public:

	static volatile PCB* running;
	static volatile PCB* mainPCB;
	static volatile Idle* idle;
	static PCBStack* listOfPCB;
	static KSemStack* listOfSemaphores;

	static volatile int isDispatch;
	static volatile int lockCnt;

	static void init();
	static void reset();

	static void lock();
	static void unlock();

	static void tickSemaphores();


};

#endif /* SYSTEM_H_ */
