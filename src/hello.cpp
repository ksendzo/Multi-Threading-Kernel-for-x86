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
#include "DOS.H"


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

//void syncPrintf(char* pr){
//	System::lock();
//	printf(pr);
//	System::unlock();
//}
//
//void syncPrintf(char* pr, int a){
//	System::lock();
//	printf(pr, a);
//	System::unlock();
//}
//
//void syncPrintf(char* pr, int a, int b){
//	System::lock();
//	printf(pr, a, b);
//	System::unlock();
//}
//
//void syncPrintf(char* pr, int a, int b, char c){
//	System::lock();
//	printf(pr, a, b, c);
//	System::unlock();
//}
//
//
///*
// 	 Test: Semafori sa spavanjem 3
//*/
//
//int t=-1;
//
//Semaphore s(0);
//
//class TestThread : public Thread
//{
//private:
//	Time waitTime;
//
//public:
//
//	TestThread(Time WT): Thread(), waitTime(WT){}
//	~TestThread()
//	{
//		waitToComplete();
//	}
//protected:
//
//	void run();
//
//};
//
//void TestThread::run()
//{
//	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
//	int r = s.wait(waitTime);
//	s.signal();
//	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
//}
//
//void tick()
//{
//	/*
//	t++;
//	if(t)
//		syncPrintf("%d\n",t);
//		*/
//}
//
//
////#include <iostream.h>
//
//Semaphore* mutex = 0;
//
//class Znak : public Thread
//{
//public:
//	Znak(char znak, int n) : Thread(), znak(znak), n(n) {}
//	virtual ~Znak() { waitToComplete(); }
//
//	void run()
//	{
//		for (long i = 0; i < 100000; i++)
//		{
////			printf("z");
//			if (mutex->wait(n)) {
//				cout << znak;
//				mutex->signal();
//			}
//
//		}
//
//		if (mutex->wait(n)) {
//			cout << endl << znak << " finished" << endl;
//			mutex->signal();
//		}
//	}
//
//private:
//	char znak;
//	int n;
//
//};
//
//
//int userMain(int argc, char* argv[]) {
//	mutex = new Semaphore(1);
//
//	Znak* a = new Znak('a', 10);
//	Znak* b = new Znak('b', 15);
//	Znak* c = new Znak('c', 20);
//
//	a->start();
//	b->start();
//	c->start();
//
//	delete a;
//	delete b;
//	delete c;
//
//	if (mutex->wait(1)) {
//		cout << endl << "userMain finished" << endl;
//		mutex->signal();
//	}
//
//	delete mutex;
//
//
//
//	return 0;
//}











int userMain(int argc, char* argv[]);
//class KernelEv;
//KernelEv* IVTEntryArray[256];
int main(int argc, char* argv[])
{
	System::init();

	userMain(argc, argv);

	System::reset();

	return 0;
}


