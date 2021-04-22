/*
 * KerSem.h
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

class PCBStack;

typedef unsigned int Time;

class KernelSem {
public:
	KernelSem(int init);
	virtual ~KernelSem();

	int value;

	int val() const;

	PCBStack* blockedList;
	void tick() volatile;

	int wait(Time t);
	void signal();




};

#endif /* KERSEM_H_ */
