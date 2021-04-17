#include "PCB.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdio.h>
#include <iostream.h>
#include "System.h"

//extern volatile int cntr;
extern volatile int context_switch_on_demand;

//extern  unsigned tsp, tss, tbp;
extern volatile PCB* running;

//extern unsigned oldTimerOFF, oldTimerSEG;
void inicTimer();
void restoreTimer();

void tick(){
	printf(".");
}

// nova prekidna rutina tajmera
void interrupt timer(){
	static unsigned tsp, tss, tbp;
	static unsigned int timerCnt = 0;
	if (!context_switch_on_demand){
		if(timerCnt > 0)
			timerCnt--;
		tick();
	}

	if (context_switch_on_demand || (timerCnt == 0 && running->timeSlice > 0)) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		// scheduler
		//cout << "old running = " << running->ss << ":" << running->sp << endl;

		if(running->finished == 1)
			printf("gotova nit\n");
		else {

		if(running->finished == 0){
//			printf("\tScheduler::put\n");
			Scheduler::put((PCB*)running);
		}
//		cout << "\t\t\tTIMER" << endl;
		}
		running = Scheduler::get();

		if(running == 0)
			cout << "greskaaaaaaaaaaaaaaaaaaaaaaa" << endl;
//		else
//			cout << "new running = " << running->ss << ":" << running->sp << endl;

		timerCnt = running->timeSlice;

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

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
	if(!context_switch_on_demand) asm int 60h;

	context_switch_on_demand = 0;
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
