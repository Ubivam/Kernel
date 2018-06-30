/*
 * Thread.cpp
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */
#include "../h/thread.h"

struct ConstThread;
struct SleepData;

void dispatch() {
	//syncPrintf("Uso u Dispatch nit je: %d\n", PCB::running->id);
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	PCB::context_switch_on_demand = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif

}
Thread::Thread(StackSize stackS, Time timeSlice) {

//----------------------System call and sending the structure to Kernel---------------------1

	myPCB = 0;
	ConstThread *p = new ConstThread(stackS, timeSlice, this);
	Kernel::SYS_CALL(THREAD_CREATE, p);

}
Thread::~Thread() {

//---------------------System call for freeing memory----------------------------------------2
	Kernel::SYS_CALL(WAIT_TO_COMPLETE, this);
	Kernel::SYS_CALL(THREAD_DESTROY, this);

}
void Thread::waitToComplete() {
//---------------------System call for wait to complete -------------------------------------4

	Kernel::SYS_CALL(WAIT_TO_COMPLETE, this);

}
void Thread::start() {
//--------------------System call for starting the Thread------------------------------------3

	Kernel::SYS_CALL(THREAD_START, this);
}
void Thread::sleep(Time timeToSleep) {
//--------------------System call for sleep thread-------------------------------------------
	int *p =&timeToSleep;
	Kernel::SYS_CALL(THREAD_SLEEP, p);
}

ID Thread::getId() {
	return myPCB->id;
}
ID Thread::getRunningId() {
	return PCB::running->id;
}
Thread * Thread::getThreadById(ID id) {
	Queue::Elem *help = PCB::pList->first;
	while (help != 0) {
		if (help->p->id == id) {
			return (Thread*) help->p->myThread;
		}
		help = help->next;
	}
	return 0;
}
