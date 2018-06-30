/*
 * kernEv.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: OS1
 */
#include "../h/kernEv.h"

IVTEntry* IVTEntry::ivtps[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivN, pInterrupt newRoutine){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	ivtps[ivtNo]=this;
	ivtNo=ivN;
	newRout=newRoutine;
	events=0;
	oldRout=0;
#ifndef BCC_BLOCK_IGNORE
	oldRout=getvect(ivtNo);
	setvect(ivtNo, newRout);
	unlock();
#endif
}
IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock();
	setvect(ivtNo,oldRout);
	callOldRoutine();
	ivtps[ivtNo]=0;
	delete events;
	events=0;
	unlock();
#endif
}
void IVTEntry::signal(){
	if(events!=0) events->ret_signal();
}
void IVTEntry::callOldRoutine(){
	if(oldRout!=0) (*oldRout)();
}

KernelEv::KernelEv(IVTNo ivtNo, Event* ev) {
	if (IVTEntry::ivtps[ivtNo]) {
		event = ev;
		ivtPointer = ivtNo;
		creator = (PCB*) PCB::running;
		blocked_Th = 0;
		val = 0;
	}
}
KernelEv::~KernelEv() {
	IVTEntry::ivtps[ivtPointer]->events = 0;
}
void KernelEv::signal() {
	if(val==0){
		val=1;
		creator->state=PCB::Ready;
		Scheduler::put(creator);
	}
}
void KernelEv::ret_signal(){
	signal();
}
void KernelEv::wait() {
	if(PCB::running->id==creator->id){
		if(val!=0) val=0;
		else{
			PCB::running->state=PCB::Blocked;
			creator->state=PCB::Blocked;
			dispatch();
		}
	}
}

void KernelEv::evCreate(EventCreate* ec) {
	if (ec != 0) {
		KernelEv(ec->ivtNo, ec->event);
		IVTEntry::ivtps[ec->ivtNo]->events = ec->event->myImpl;
	}
}

void KernelEv::evDestory(KernelEv* e) {
	if (e != 0) {
		delete e;
	}
}

void KernelEv::evSignal(KernelEv* e) {
	if (e != 0) {
		e->signal();
	}
}

void KernelEv::evWait(KernelEv* e) {
	if (e != 0) {
		e->wait();
	}
}

