/*
 * Idle.cpp
 *
 *  Created on: Apr 18, 2021
 *      Author: OS1
 */

#include "Idle.h"
#include "PCB.h"
#include <stdio.h>

Idle::Idle():Thread(256, 1) {
//	this->getMyPCB())->finished = 1;

	getMyPCB()->state = PCB::FINISHED;
//	p->finished = 1;
}

Idle::~Idle() {
	// TODO Auto-generated destructor stub
}

void Idle::run() {
	while(1){
		for(int i = 0; i < 30000; i++)
			for(int j = 0; j < 30000; j++);
		printf("in idle\n");
	}
}

