/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <stdio.h>
#include "Thread.h"
#include "PCB.h"
#include <iostream.h>
#include "System.h"
#include "Semaphor.h"
#include <stdlib.h>
#include "Event.h"
#include "IVTEntry.h"

//class A: public Thread {
//public:
//	A():Thread(12048, 20){}
//protected:
//	virtual void run();
//};
//
//void A::run() {
//	for(int i = 0; i < 30; i++){
//		System::lock();
//		printf("u a() i = %d\n", i);
//		System::unlock();
//
//		for(int k = 0; k < 10000; k++)
//			for(int j = 0; j < 30000; j++);
//	}
//}
//
//void doSomething(){
//	System::lock();
//	A *a = new A;
//	A *b = new A;
//	a->start();
//	b->start();
//	System::unlock();
//
//	for(int i = 0; i < 5; i++){
//		printf( "main %d\n", i);
//
//		//dispatch();
//		for(int j = 0; j < 30000; j++)
//			for(int k = 0; k < 30000; k++);
//	}
//	System::lock();
//	printf("Kraj.\n");
//	System::unlock();
//
//	a->waitToComplete();
//	b->waitToComplete();
//	System::lock();
//	delete a;
//	delete b;
//	System::unlock();
//}

void syncPrintf(char* pr){
	System::lock();
	printf(pr);
	System::unlock();
}

void syncPrintf(char* pr, int a){
	System::lock();
	printf(pr, a);
	System::unlock();
}

void syncPrintf(char* pr, int a, int b){
	System::lock();
	printf(pr, a, b);
	System::unlock();
}

void syncPrintf(char* pr, int a, int b, char c){
	System::lock();
	printf(pr, a, b, c);
	System::unlock();
}





#include <iostream.h>
#include <stdlib.h>

PREPAREENTRY(9,1);

Semaphore* mutex = 0;
Semaphore* sleepSem = 0;

void tick() {}


class Znak : public Thread
{
public:
	Znak(char znak, int n) : Thread(), znak(znak), n(n) {}
	virtual ~Znak() { waitToComplete(); }

	void run()
	{
		// for (long i = 0; i < 100000; i++)
		for (int i = 0; i < n; i++)
		{
			if (mutex->wait(1)) {
				cout << znak;
				mutex->signal();
			}

			// for (int j = 0; j < 10000; j++)
				// for (int k = 0; k < 10000; k++);
			Time sleepTime = 2 + rand() % 8;
			sleepSem->wait(sleepTime);

			// dispatch();
		}

		if (mutex->wait(1)) {
			cout << endl << znak << " finished" << endl;
			mutex->signal();
		}
	}

private:
	char znak;
	int n;

};


class Key : public Thread {
public:
	Key(int n) : Thread(), n(n) {}
	virtual ~Key() { waitToComplete(); }

	void run() {
		Event e(9);

		for (int i = 0; i < n; i++) {
			if (mutex->wait(1)) {
				cout << endl << "key waiting " << (i + 1) << endl;
				mutex->signal();
			}

			e.wait();

			if (mutex->wait(1)) {
				cout << endl << "key continue " << (i + 1) << endl;
				mutex->signal();
			}

			sleepSem->wait(5);
		}

		if (mutex->wait(1)) {
			cout << endl << "key finished" << endl;
			mutex->signal();
		}
	}

private:
	int n;

};


int userMain(int argc, char* argv[]) {
	mutex = new Semaphore(1);
	sleepSem = new Semaphore(0);

	Znak* a = new Znak('a', 10);
	Znak* b = new Znak('b', 15);
	Znak* c = new Znak('c', 20);
	Key* k = new Key(10);

	a->start();
	b->start();
	c->start();
	k->start();

	delete a;
	delete b;
	delete c;
	delete k;

	if (mutex->wait(1)) {
		cout << endl << "userMain finished" << endl;
		mutex->signal();
	}

	delete sleepSem;
	delete mutex;

	return 0;
}












int userMain(int argc, char* argv[]);
int main(int argc, char* argv[])
{
	System::init(); 	// pocetni running

//	doSomething();

	userMain(argc, argv);
	syncPrintf("Kraj userMain();\n");

	System::reset();
	return 0;
}


