/*
 * KernelEv.h
 *
 *  Created on: Apr 20, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

typedef unsigned char IVTNo;
class PCB;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

	volatile PCB* creatorPCB;
	volatile int value;
	volatile int blocked;

	volatile IVTNo ivtNo;

};

#endif /* KERNELEV_H_ */
