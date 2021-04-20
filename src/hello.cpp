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







int t=-1;

const int n=15;

Semaphore s(1);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	if(getId()%2)
		s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread* t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i] = new TestThread(5*(i+1));
		t[i]->start();
	}
	for(i=0;i<n;i++)
	{
		t[i]->waitToComplete();
	}
	for(i = 0; i < n; i++)
		delete t[i];
	delete t;
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


