/*
 * kernSem.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */
#include "kernSem.h"

KernelSem::KernelSem(int init, Semaphore* sem) {
	mySem = sem;
	v = init;
	blockedList = new Queue();
}

int KernelSem::wait(int toBlock) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if (--v >= 0) {
#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
		return 0;
	} else if (!toBlock) {
		v++;
#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
		return -1;
	} else {
		PCB::running->state = PCB::Blocked;
		blockedList->put((PCB*) PCB::running);
		dispatch();
#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
		return 1;
	}

}
void KernelSem::signal() {
#ifndef BCC_BLOCK_IGNORE
		lock();
#endif
	if(++v <= 0)
	{
		PCB* pcb = blockedList->get();
		pcb->state=PCB::Ready;
		Scheduler::put(pcb);
	}
#ifndef BCC_BLOCK_IGNORE
		unlock();
#endif
}
KernelSem::~KernelSem() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	while(v<0)
		signal();
	delete blockedList;
	blockedList=0;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int KernelSem::val() const {
	return v;
}
