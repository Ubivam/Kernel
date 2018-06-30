/*
 * PCB.h
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */
#ifndef H_PCB_H_
#define H_PCB_H_
#include "thread.h"
#include "queue.h"
#include "sleepL.h"
struct SleepData;
struct ConstThread;
struct PCBLst;
class Thread;
class Queue;
class SleepList;
//===============================Program Control Block===============================//
class PCB {
private:
	static Queue* pList;		//Globalna Lista svih PCB-ova
	static SleepList * sList;		//Globalna Lista svih Spavajucih Niti
	volatile static PCB* running;
	volatile static Thread* doNothing;
	volatile static unsigned context_switch_on_demand;
	volatile static unsigned cntr;                     //Global counter for time
	Queue * waitingToComplete; //Lista niti koje cekaju da se neka druga nit zavrsi da bi one mogle da nastave
	volatile Thread * myThread;

//-------------------------------------------------------------------
	PCB(Thread *myThread, StackSize sSize, Time tSlice);
	~PCB();
	static void wrapper();
//-------------------------------------------------------------------
	static void thCreate(ConstThread* p);
	static void thDestroy(Thread *p);
	static void thStart(Thread *p);
	static void thWaitToComplete(Thread* p);
	static void thIdleSetup(Thread * p);
	static void thSleep(unsigned * t);
//-------------------------------------------------------------------
	unsigned * stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	enum State {
		New, Ready, Active, Blocked, Finished, Idle
	};
	volatile State state;
//-------------------------------------------------------------------
	volatile static ID idUkupno;
	volatile ID id;

	friend class KernelEv;
	friend class SleepList;
	friend class Thread;
	friend class Kernel;
	friend class Queue;
	friend class KernelSem;
	friend void interrupt timer(...);
	friend void dispatch();
};
//===================================================================================//

//===============================Data for Thread Constructor===============================//
struct ConstThread {
	StackSize stackSize;
	Time timeSlice;
	Thread* thrd;
	ConstThread(StackSize s, Time t, Thread* th) :
			stackSize(s), timeSlice(t), thrd(th) {
	}
};
//=========================================================================================//
#endif /* H_PCB_H_ */
