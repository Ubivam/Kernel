/*
 * Thread.h
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */

#ifndef H_THREAD_H_
#define H_THREAD_H_
#include "def.h"
#include <iostream.h>
#include "kernel.h"
#include "def.h"

class PCB;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	static void sleep(Time timeToSleep);

protected:
	friend class PCB;
	friend void interrupt timer(...);
	Thread(StackSize stackSize = defaultStackSize,
			Time timeSlice = defaultSlice);
	virtual void run() {} 						//User is to define this method
private:
	PCB* myPCB;
};
void dispatch();

#endif /* H_THREAD_H_ */
