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

void syncPrintf(char* pr, int a, int b, char c){
	System::lock();
	printf(pr, a, b, c);
	System::unlock();
}






const int n = 5;
int count = 10;

Semaphore s(2);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	s.wait(0);
	cout<<"Thread "<<getId()<<" in critical section."<<endl;
	for(unsigned int i=0;i<64000;i++)
		for(unsigned int j=0;j<64000;j++);
	cout << "Thread " << getId() << " finished critical section." << endl;
	s.signal();
}

void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
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


