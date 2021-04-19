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







class TestThread : public Thread
{
public:

	TestThread(): Thread() {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d: loop1 starts\n", getId());

	for(int i=0;i<32000;i++)
	{
		for (int j = 0; j < 32000; j++);
	}

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());


}

class WaitThread: public Thread
{
private:
	TestThread *t1_,*t2_;

public:
	WaitThread(TestThread *t1, TestThread *t2): Thread()
	{
		t1_ = t1;
		t2_ = t2;
	};

	~WaitThread()
		{
			waitToComplete();
		}

protected:

	void run()
	{
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}
};

void tick() {}

int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread t1,t2;
	WaitThread w(&t1,&t2);
	t1.start();
	t2.start();
	w.start();
	syncPrintf("User main ends\n");
	return 16;
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


