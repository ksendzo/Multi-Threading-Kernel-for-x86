/*
 * prekidi.h
 *
 *  Created on: May 16, 2020
 *      Author: OS1
 */

#ifndef PREKIDI_H_
#define PREKIDI_H_

//#include "thread.h"
//#include "list.h"

class KernelSem;

// zabranjuje prekide
#define lock() {\
	lockFlag++;\
}

// dozvoljava prekide
#define unlock() {\
	lockFlag--;\
	if(lockFlag == 0 && bilo_je_zahteva)\
		dispatch();\
}

extern volatile int lockFlag, bilo_je_zahteva;
extern volatile int zahtevana_promena_konteksta, brojac;
extern unsigned tsp, tss;



void interrupt timer();

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}



#endif /* PREKIDI_H_ */
