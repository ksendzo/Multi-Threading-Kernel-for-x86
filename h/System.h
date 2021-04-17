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

class System {
public:
	System();
	virtual ~System();

//	static volatile PCB* running;
	static PCBStack* ListOfPCB;

	static void init();
	static void reset();

	//static unsigned keepOldTimerOFF, keepOldTimerSEG;
//	static void inicTimer();
//	static void restoreTimer();
};

#endif /* SYSTEM_H_ */
