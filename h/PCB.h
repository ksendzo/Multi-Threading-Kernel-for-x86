/*
 * pcb.h
 *
 *  Created on: Apr 15, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "list.h"
#include "listHand.h"


//#include "thread.h"

class Thread;
void dispatch();

extern PCB* running, *nultaNit;


typedef int ID;
typedef void (*SignalHandler) ();
typedef unsigned SignalId;

class PCB{
public:
	enum eSTATE {ACTIVE, BLOCKED, FINISHED};
    volatile unsigned sp;
    volatile unsigned ss;
    unsigned* stack;
    volatile int timeSlice;

    Thread* myThread;
    volatile const ID myID;

    PCB* parent;

//    List<PCB> waitList; // ko ceka na mene?

//    List<PCB> childrenList;

    ListHandler SignalArray[16];
    List<SignalId> waitingSignals;
    int blockedSignals[16];

    KernelSem* mySem;

    eSTATE state;

    volatile int leftToWait;
    volatile int isWaiting;
    void setWaitingTime(int);
    int tick();

    friend ostream& operator<<(ostream&, PCB&);

    PCB();
    ~PCB();

    void waitToComplete();


    void signal(SignalId singnal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void processSignals();

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void endThread();


private:
	int signalFlag1, signalFlag2;

};

void kernelSignal0(){
//	printf("\nKILL THREAD\n");
	if(running)
		running->endThread();

}

void PCB::endThread(){
	state = FINISHED;

	if(parent){
		parent->signalFlag1 = 1;
		parent->signal(1);
	}
	signalFlag2 = 1;
	signal(2);
}


volatile static ID cntID;

PCB::PCB():myID(cntID++){
	if(running != NULL && running != nultaNit){
		for(int i = 0; i < 16; i++){
			SignalArray[i].copy(running->SignalArray[i]);
			blockedSignals[i] = running->blockedSignals[i];
		}
		parent = running;
	}
	else{ // ni sistem thread nece imati roditelja...
		for(int i = 0; i < 16; i++)
			blockedSignals[i] = 0;
		registerHandler(0, kernelSignal0);
		parent = NULL;
	}
	signalFlag1 = 0;
	signalFlag2 = 0;
	mySem = NULL;
}


PCB::~PCB(){
	waitToComplete();
	delete stack;

}

void PCB::waitToComplete(){
	while(state != FINISHED)
		dispatch();
}


void PCB::setWaitingTime(int i){
	if(i){
		leftToWait = i;
		isWaiting = 1;
	}
	else{
		isWaiting = 0;
		leftToWait = -1;
	}
}


int PCB::tick(){
	if(isWaiting)
		return --leftToWait;
	else return 1;
}

ostream& operator<<(ostream& it, PCB& p){
	it << "T"<<p.myID;
	return it;
}





extern volatile int signal_u_toku;
void PCB::signal(SignalId sig){
	if(sig == 0 || (sig > 2 && sig < 16) || (sig == 1 && signalFlag1 == 1) || (sig == 2 && signalFlag2 == 1)){
		lock();
		waitingSignals.add(new SignalId(sig));
		unlock();
	}

	if(sig == 1 && signalFlag1 == 1)
		signalFlag1 = 0;
	if(sig == 2 && signalFlag2 == 1)
		signalFlag2 = 0;


	if(running == this)
		processSignals();
}

void PCB::registerHandler(SignalId sig, SignalHandler handler){
	SignalArray[sig].push(handler);
}

void PCB::unregisterAllHandlers(SignalId id){
	SignalArray[id].empty();
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	SignalArray[id].swap(hand1,hand2);
}

void PCB::processSignals(){
	waitingSignals.begin();
	while(waitingSignals.getCurr() != NULL){
		SignalId *sig = waitingSignals.getCurr(); // izvadi iz liste signala
		int obradjen = 0;
		if(!blockedSignals[*sig] && !globalBlockedSignals[*sig]){
			// izbaci iz liste zahteva
			obradjen = waitingSignals.remove(sig);
			if(*sig != 1 && *sig != 2)
//				printf("do sig %d\n", *sig);

			if(*sig == 0)
				waitingSignals.emptyAndDelete();

			if(obradjen){
				int s = *sig;
				delete sig;
				//obradjujem hendlere
				lock();
				SignalArray[s].begin();
				while(SignalArray[s].getCurrent()){
					SignalHandler sh = SignalArray[s].getCurrent();
					sh();
					SignalArray[s].next();
				}
				unlock();
				//if(state == FINISHED)
					//dispatch();
			}
		}
		if(!obradjen)
			waitingSignals.next();
	}
}


void PCB::blockSignal(SignalId sig){
	blockedSignals[sig] = 1;
}

void PCB::blockSignalGlobally(SignalId signal){
	globalBlockedSignals[signal] = 1;
}

void PCB::unblockSignal(SignalId sig){
	blockedSignals[sig] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal){
	globalBlockedSignals[signal] = 0;
}





#endif /* PCB_H_ */
