/*
 * SystemThread.h
 *
 *  Created on: Aug 6, 2020
 *      Author: OS1
 */

#ifndef SYSTHR_
#define SYSTHR_

#include "thread.h"
//#include "userMain.h"
//#include "timer.h"


class SystemThread: public Thread {
public:
	SystemThread(int, char**);
	~SystemThread();
protected:
	void run();
private:
	int argc;
	char** argv;
};


SystemThread::SystemThread(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

SystemThread::~SystemThread() {

	waitToComplete();

}

int userMain(int argc, char** argv);
void SystemThread::run(){
	userMain(argc, argv);
}


#endif /* SYSTHR_ */
