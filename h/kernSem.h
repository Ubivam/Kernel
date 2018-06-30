#ifndef H_KERNELSEM_H_
#define H_KERNELSEM_H_

#include "def.h"
#include "semaphor.h"

class Semaphore;
class Queue;
class QueueSem;

class KernelSem {
private:
	KernelSem(int init = 1, Semaphore * sem);
	virtual ~KernelSem();
	virtual int wait(int toBlock);
	virtual void signal();
//-------------------------------------------------------------
	int val() const;
//-------------------------------------------------------------
	volatile Semaphore* mySem;
	Queue *blockedList;
	int v;

	friend class Semaphore;
	friend class Kernel;
	friend class Queue;
};

#endif

