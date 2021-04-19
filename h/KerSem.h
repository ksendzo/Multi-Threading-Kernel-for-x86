/*
 * KerSem.h
 *
 *  Created on: Apr 19, 2021
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

class KernelSem {
public:
	KernelSem(int init);
	virtual ~KernelSem();

	int value;

	int val() const;

};

#endif /* KERSEM_H_ */
