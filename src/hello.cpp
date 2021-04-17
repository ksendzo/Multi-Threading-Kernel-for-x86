/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <stdio.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "aaaaaaaa.h"
#include "Thread.h"
#include "PCB.h"
#include <iostream.h>
#include "System.h"

// zabranjuje prekide
#define lock asm cli

// dozvoljava prekide
#define unlock asm sti

void inicTimer();
void restoreTimer();


class A: public Thread {
public:
	A():Thread(12048, 20){}
protected:
	virtual void run();
};

void A::run() {
	for(int i = 0; i < 30; i++){
		lock;
		printf("u a() i = %d\n", i);
		unlock;

		for(int k = 0; k < 10000; k++)
			for(int j = 0; j < 30000; j++);
	}
}

void doSomething(){
	lock;
	A *a = new A;
	A *b = new A;
	a->start();
	b->start();
	unlock;

	for(int i = 0; i < 5; i++){
		printf( "main %d\n", i);

		//dispatch();
		for(int j = 0; j < 30000; j++)
			for(int k = 0; k < 30000; k++);
	}
	lock;
	printf("Kraj.\n");
	unlock;

	a->waitToComplete();
	lock;
	delete a;
	delete b;
	unlock;
}

int main()
{
	System::init(); 	// pocetni running

	doSomething();

	System::reset();
//	restoreTimer();
//	delete running;
	return 0;


}


