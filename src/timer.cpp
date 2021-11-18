#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdio.h>
#include <iostream.h>
#include "System.h"
#include "Idle.h"


void tick();
// nova prekidna rutina tajmera
void interrupt timer(...){
	static unsigned tsp, tss, tbp;
	static unsigned int timerCnt = 0;
	if (!System::isDispatch){
		if(timerCnt > 0)
			timerCnt--;
		System::tickSemaphores();
		tick();
	}

	if (System::isDispatch || (timerCnt == 0 && System::running->timeSlice > 0)) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		System::running->sp = tsp;
		System::running->ss = tss;
		System::running->bp = tbp;

		if(System::running->state == PCB::READY){
			Scheduler::put((PCB*)System::running);
		}

		System::running = Scheduler::get();

		if(System::running == 0){
			System::running = System::idle->getMyPCB();
		}

		timerCnt = System::running->timeSlice;

		tsp = System::running->sp;
		tss = System::running->ss;
		tbp = System::running->bp;

		asm {
			// restaurira sp
			mov bp, tbp
			mov sp, tsp
			mov ss, tss
		}
	}

	// poziv stare prekidne rutine
	// koja se nalazila na 08h, a sad je na 60h;
	// poziva se samo kada nije zahtevana promena konteksta
	// tako da se stara rutina poziva
	// samo kada je stvarno doslo do prekida
	if(!System::isDispatch) // asm int 60h;
		System::oldTimer();

	System::isDispatch = 0;
}
