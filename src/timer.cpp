#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdio.h>
#include <iostream.h>
#include "System.h"
#include "Idle.h"

//extern volatile int System::isDispatch;

void inicTimer();
void restoreTimer();

//void tick(){
//	printf(".");
//}
void tick();
// nova prekidna rutina tajmera
void interrupt timer(){
	static unsigned tsp, tss, tbp;
	static unsigned int timerCnt = 0;
	if (!System::isDispatch){
		if(timerCnt > 0)
			timerCnt--;
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

//		if(System::running->state == PCB::FINISHED)
//			printf("gotova nit\n");

		if(System::running->state == PCB::READY){
			Scheduler::put((PCB*)System::running);
		}

		System::running = Scheduler::get();

		if(System::running == 0){
			printf("idle\n");
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
	if(!System::isDispatch) asm int 60h;

	System::isDispatch = 0;
}


unsigned oldTimerOFF, oldTimerSEG;

// postavlja novu prekidnu rutinu
void inicTimer(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax // es = 0

		// pamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		// postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		// postavlja staru rutinu na int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
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
