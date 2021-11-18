/*
 * aaaaaaaa.h
 *
 *  Created on: Aug 26, 2020
 *      Author: OS1
 */

#ifndef H_AAAAAAAA_H_
#define H_AAAAAAAA_H_

class PCB;
template <class T>
class List;
class KernelSem;
class KernelEv;

PCB* nultaNit;
PCB* running;

unsigned tsp;
unsigned tss;

int nextThread;
List<KernelSem> *listOfKernelSem;
static int numSem = 0;


int syncPrintf(const char *format, ...);

// potpis interrupt rutine za rad sa setvect() i getvect()
typedef void interrupt (*pInterrupt)(...);

// stara prekidna rutina
pInterrupt oldISR;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;
volatile int signal_u_toku = 0;
volatile int lockFlag = 0;
volatile int bilo_je_zahteva = 0;

volatile int globalBlockedSignals[16];

KernelEv* IVTEntryArray[256];


#endif /* H_AAAAAAAA_H_ */
