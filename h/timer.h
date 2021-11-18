/*
 * timer.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "dos.h"
#include "list.h"
#include "kerSem.h"
class KernelSem;

void tick();
//extern List<KernelSem> *listOfKernelSem;


void tickAllSem(){
	listOfKernelSem->begin();
	KernelSem* curr = listOfKernelSem->getCurr();
	while(curr != NULL){
		curr->tick();
		listOfKernelSem->next();
		curr = listOfKernelSem->getCurr();
	}
}

extern volatile int zahtevana_promena_konteksta, bilo_je_zahteva;
extern volatile int signal_u_toku;
// prekidna rutina
void interrupt timer(){	// prekidna rutina

	if (!zahtevana_promena_konteksta) {
		if(brojac > 0)
			brojac--;
		tick();
		tickAllSem();
	}

	int dispatchAgain = 1;
	//promeni kontekst
	while(dispatchAgain){
		dispatchAgain = 0;
	if (((brojac == 0 && running->timeSlice != 0) || zahtevana_promena_konteksta) ) {
		if(lockFlag == 0){
			bilo_je_zahteva = 0;
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
			}

			running->sp = tsp;
			running->ss = tss;

			if(running != nultaNit && running->state == PCB::FINISHED){
				delete running->stack;
				running->stack = NULL;
			}
			if(running != nultaNit && running->state == PCB::ACTIVE){
				  Scheduler::put(running);
			}
			running = Scheduler::get();

			if(running == NULL)
				running = nultaNit;

			tsp = running->sp;
			tss = running->ss;

			brojac = running->timeSlice;

			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
			}
			running->processSignals();
			if (running != nultaNit && running->state == PCB::FINISHED){
				dispatchAgain = 1;
				//delete []running->stack;

//				printf("\ndispatchAgain\n");
//				printf("finished\n");
			}
		}
		else
			bilo_je_zahteva = 1;

	}
	}
	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
	if(!zahtevana_promena_konteksta)
		asm int 60h;

	zahtevana_promena_konteksta = 0;

}



// stara prekidna rutina
unsigned oldTimerOFF, oldTimerSEG;

// postavlja novu prekidnu rutinu
// u Interrupt vektor tabeli
void inicTimer(){
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}
}

// restauira staru prekidnu rutinu
// u Interrupt vektor tabeli
void restoreTimer(){
	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
}

