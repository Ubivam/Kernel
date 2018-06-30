/*
 * PCB.cpp
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */
#include "../h/pcb.h"

volatile unsigned PCB::context_switch_on_demand = 0;
Queue* PCB::pList = 0;
SleepList* PCB::sList = 0;
volatile unsigned PCB::cntr = 0;
volatile PCB* PCB::running = 0;
volatile Thread* PCB::doNothing = 0;
volatile ID PCB::idUkupno = 0;

class Queue;
class SleepList;

int syncPrintf(const char *format, ...);
//-------------------------PCB Constructor-------------------------//
PCB::PCB(Thread *myThread, StackSize stackSize, Time tSlice) {
//	syncPrintf("Pravi Thread(stack)\n");

	stackSize /= sizeof(unsigned); //4096 bajtova -> 2048 indeksa u nizu unsigned*

	id = idUkupno++;
	waitingToComplete = 0;
	state = New;
//Pravljenje steka nove niti
	stack = new unsigned[stackSize]; //Alokacija steka i na dole ide inicajalizovanje kljucnis pozicija u steku niti

	this->myThread = myThread;
	//Skidanje warining
	this->ss = 0;	 //
	this->sp = 0;	 //
	this->bp = 0;	 //
	//-----------------
	stack[stackSize - 1] = 0x200; //I fleg enabled

//Postavljanje pocetnog konteksta steka nove niti
#ifndef BCC_BLOCK_IGNORE
	stack [stackSize-2]=FP_SEG((PCB::wrapper)); //PC na koji ce da ode, wrapper je staticka
	stack [stackSize-3]=FP_OFF((PCB::wrapper));

	this->ss=FP_SEG(stack+stackSize-12);//Posto je unisgned 16 trebamo da dohvatimo segmentni i ofsetni deo pa koristimo dos.h funkcije
	this->sp=FP_OFF(stack+stackSize-12);
	this->bp=sp;
#endif

//Postavljanje flagova i vremenskog kvanta niti
	timeSlice = tSlice;

}

//-----------------PCB Destructor--------------------//
PCB::~PCB() {
//	syncPrintf("*************BRISE SE PCB******************\n");

	delete[] stack;
	stack = 0;

}

//-----------------Wrapper method-------------------//
void PCB::wrapper() {
	if (PCB::running != 0) {
//		syncPrintf("Wrapper Se pokrece!!!\n");  // TEST
		((Thread*) (PCB::running->myThread))->run();
//		syncPrintf("RUN zavrsio!!!\n");
#ifndef BCC_BLOCK_IGNORE
		lock();
#endif

		PCB::running->state = Finished;
		if (PCB::running->waitingToComplete != 0) {
			while (PCB::running->waitingToComplete->first != 0) {
				PCB* hlp = (PCB*) PCB::running->waitingToComplete->get();
				if (hlp->id != 1) {
					hlp->state = Ready;
					Scheduler::put(hlp);
				}
			}
		}
		//	PCB::running->waitingToComplete->~Queue();
		PCB::running->waitingToComplete = 0;

	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
//	syncPrintf("Wraper se zavrsio!!!\n"); // TEST

	dispatch(); //Mora da se zove dispatch jer vraper nesme da se zavrsi, nismo mu stavili call back pa bi imao katastrofalne posledice

}

//---------------------Thread Constructor----------------------------------//
void PCB::thCreate(ConstThread *p) {
	if (p != 0) {
		if ((p->stackSize > 65355) || (p->stackSize < 0)
				|| (p->timeSlice < 0)) {
			return;
		}

		p->thrd->myPCB = new PCB(p->thrd, p->stackSize, p->timeSlice); //Pravi se novi PCB

		if (pList == 0) {
			pList = new Queue(p->thrd->myPCB); //Stavljanje pcb-a u listu svih niti
		} else {
			pList->put(p->thrd->myPCB);
		}

//		syncPrintf("Javljam da je Thread napravljen i njegov ID je : %d \n",
//				p->thrd->myPCB->id);
	}
}

//--------------------Thread Destructor-------------------------------------//
void PCB::thDestroy(Thread* p) {
	if (p != 0) {
//		syncPrintf("Nit se unistava: %d \n", p->myPCB->id); // TEST
		if (p->myPCB->state == PCB::Finished || p->myPCB->state == PCB::Idle) {
			//Brisanje niti iz liste aktivnih niti!!!
			if ((PCB::pList != 0) && (p->myPCB->id != 0)) {
				Queue::Elem *lPointer1 = pList->first,
						*lPointer2 = pList->first;
				if (pList->first->p->id == p->myPCB->id) {
					pList->first = pList->first->next;
					lPointer1->p->~PCB();
					delete lPointer1;
					lPointer1 = 0;
					return;
				}
				while (lPointer1 != 0) {
					if (p->myPCB->id == lPointer1->p->id) {
						lPointer2->next = lPointer1->next;
						lPointer1->p->~PCB();
						delete lPointer1;
						lPointer1 = 0;
						break;
					}
					lPointer2 = lPointer1;
					lPointer1 = lPointer1->next;
				}
			}
		}
	}

}
//---------------------Thread Start method-----------------------------------//
void PCB::thStart(Thread * p) {
	if (p != 0) {
		//	syncPrintf("Javljam da je Thread pokrenut: %d \n", p->myPCB->id); // TEST
		if (p->myPCB->state != PCB::Ready) {
			p->myPCB->state = PCB::Ready;
			Scheduler::put(p->myPCB);

		}

	}
}
//-----------------------WaitToComplete---------------------------------------//
void PCB::thWaitToComplete(Thread * p) {
	if (p != 0) {
//		syncPrintf("WAIT TU COMPLETE JE POZVAN\n");  // TEST
		if ((PCB::running->id == 1) || (PCB::running->myThread == p)
				|| (p->myPCB->id == 1)) {
			syncPrintf("Nekorektan poziv WTC\n");
			return;
		}

		if ((p->myPCB->state) != Finished) {
//			syncPrintf(
//					"WaitToComplete ubacuje listu niti sa id: %d nit koja je trenutno running:%d \n",
//					p->myPCB->id, PCB::running->id);
			if (p->myPCB->waitingToComplete == 0) {
				p->myPCB->waitingToComplete = new Queue((PCB*) PCB::running);
			} else
				p->myPCB->waitingToComplete->put((PCB*) PCB::running);
			PCB::running->state = Blocked;
		}
	}

}
//---------------------Idle thread setup method--------------------------------//
void PCB::thIdleSetup(Thread *p) {
	if (p != 0) {
//		syncPrintf("PRAVI SE IDLE nit"); // TEST
		p->myPCB->state = Idle;
		doNothing = p;
	}
}
//----------------------Sleep thread------------------------------------------//
void PCB::thSleep(unsigned int * t) {
	if (t != 0) {
		if (sList == 0) {
			sList = new SleepList();
		}
		sList->add((PCB*) PCB::running, *t);
		PCB::running->state = Blocked;
	}
}

