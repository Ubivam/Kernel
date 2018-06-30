/*
 * kernEv.h

 *
 *  Created on: Jun 15, 2018
 *      Author: OS1
 */
#ifndef H_KERNEV_H_
#define H_KERNEV_H_

#include "def.h"
#include "event.h"

class Event;
class Queue;
class PCB;

class KernelEv;
struct EventCreate;

class IVTEntry {
public:
	IVTEntry(IVTNo ivN, pInterrupt newRoutine);
	~IVTEntry();
	void signal();
	void callOldRoutine();
	KernelEv* events;
	static IVTEntry *ivtps[256];
	pInterrupt newRout;
	pInterrupt oldRout;
	IVTNo ivtNo;
};

class KernelEv {
public:
	int val;
	PCB* blocked_Th;
	PCB* creator;
	IVTNo ivtPointer;
	Event* event;
//---------------------------------------------
	void static evCreate(EventCreate* ec);
	void static evDestory(KernelEv * ev);
	void static evWait(KernelEv * ev);
	void static evSignal(KernelEv * ev);
//---------------------------------------------
	KernelEv(IVTNo ivtNo, Event* ev);
	~KernelEv();
	void wait();
	void signal();
	void ret_signal();
protected:
	friend class Kernel;
	friend class Event;
	friend class IVTEntry;
};

struct EventCreate{
	IVTNo ivtNo;
	Event* event;
	EventCreate(IVTNo iN, Event* e):ivtNo(iN),event(e){}
};

#endif /* H_KERNEV_H_ */
