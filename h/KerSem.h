/*
 * KernelSem.h
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#ifndef KERSEM_
#define KERSEM_

#include "list.h"
#include "thread.h"
#include "prekidi.h"
#include "Schedule.h"


class KernelSem{
public:
	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	void tick();
	//static void tickAll();

	int getVal();
	friend ostream& operator<<(ostream&, KernelSem&);

private:
	volatile int value;
	volatile int id;

	List<PCB> listOfPCBs;
	void block();
	void unblock(PCB*);
};

volatile static int kernelIDcnt = 0;

extern List<KernelSem>* listOfKernelSem;
extern int numSem;
KernelSem::KernelSem(int init){
	lock();
	if(listOfKernelSem == NULL)
		listOfKernelSem = new List<KernelSem>;
	unlock();

	numSem++;

	id = ++kernelIDcnt;
	value = init;
	listOfKernelSem->add(this);

	}



KernelSem::~KernelSem(){
	numSem--;
	if(listOfKernelSem)
		listOfKernelSem->remove(this);
}

extern PCB* running;


void KernelSem::block(){

	if(running->state == PCB::FINISHED){
		printf("pokusan block %d\n", id);
		return;
	}

	lock();
	listOfPCBs.add(running);
	if(running->mySem)
		printf("\nNIJE JEDINSTVEN BLOK\n");
	running->mySem = this;
	running->state = PCB::BLOCKED;
	unlock();
}

void KernelSem::unblock(PCB* x){
	if(x->state == PCB::FINISHED){
		printf("pokusan unblock\n");
		return;
	}

	lock();
	listOfPCBs.remove(x);
	if(x->state == PCB::BLOCKED){
		x->mySem = NULL;
	x->state = PCB::ACTIVE;
	Scheduler::put(x);
	}
	unlock();
}


int KernelSem::wait(Time maxTimeToWait){
	value--;
	if(value < 0){
		running->setWaitingTime(maxTimeToWait);
		block();
		dispatch();
	}
	// else - ne zablokira nit

	if(maxTimeToWait && running->leftToWait == 0)
		return 0;
	return 1;
}

int KernelSem::signal(int n){
	if(n < 0)
		return n;
	if(n == 0){
		value++;
		PCB* putBack = listOfPCBs.getFirst();
		if(!putBack)
			return n;
		unblock(putBack);
		return 0;
	}
	else{
		value += n;
		int i;
		for(i = 0; i < n; i++){
			PCB* putBack = listOfPCBs.getFirst();
			if(!putBack)
				break;
			unblock(putBack);
		}
		return i;
	}
}


void KernelSem::tick(){
	lock();
	listOfPCBs.begin();
	PCB* curr = listOfPCBs.getCurr();

	while(curr){
		if(!curr->tick()){ // treba da se odblokira
			value++;
			unblock(curr); // ja odblokiram nit i izbrisem je iz liste a current iz liste se vrati na pocetak
		}
		else
			listOfPCBs.next();
		curr = listOfPCBs.getCurr();

	}
	unlock();
}



int KernelSem::getVal(){
	return value;
}

ostream& operator<<(ostream& it, KernelSem& ks){
	it << "S" << ks.id;
	return it;
}


#endif /* KERSEM_ */
