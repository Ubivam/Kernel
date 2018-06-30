/*
 * sem.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */
#include "../h/semaphor.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init,this);
}
Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	myImpl=0;
}
void Semaphore::signal() {
	myImpl->signal();
}
int Semaphore::wait(int toBlock) {
	return myImpl->wait(toBlock);
}
int Semaphore::val() const {
	return myImpl->val();
}

