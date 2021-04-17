/*
 * aaaaaaaa.h
 *
 *  Created on: Apr 16, 2021
 *      Author: OS1
 */

#ifndef AAAAAAAA_H_
#define AAAAAAAA_H_

class PCB;
void dispatch();
void wrapper();

 //unsigned tsp, tss, tbp;

volatile int cntr =0;
volatile int context_switch_on_demand = 0;

//unsigned oldTimerOFF, oldTimerSEG;

volatile PCB* p[3];
volatile PCB* running;




#endif /* AAAAAAAA_H_ */
