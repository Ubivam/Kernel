/*
 * Kernel.cpp
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */
#include "../h/kernel.h"

struct PCBList;
unsigned tsp, tss, tbp;
unsigned oldTimerOFF, oldTimerSEG;
volatile unsigned Kernel::KernelMode = 0;
volatile unsigned Kernel::isBooted = 0;

class PCB;
class KernelSem;
//===============================Timer interrupt method===============================//
void interrupt timer(...) {

	if (!PCB::context_switch_on_demand) {
		PCB::cntr--;
		PCB::sList->check(); //Vrsi proveru sleep liste
	}
	if ((PCB::cntr == 0) || PCB::context_switch_on_demand) {
//		syncPrintf("Menja kontekst niti: %d\n", PCB::running->id);
//========================Cuvanje starog konteksta sa steka=================================//

#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		// scheduler
//========================PROMENA KONTEKSTA=================================//
		if (((PCB::running->state == PCB::New)
				|| (PCB::running->state == PCB::Ready)
				|| (PCB::running->state == PCB::Active))
				&& (PCB::running->id != 1)) {
			PCB::running->state = PCB::Ready;
			Scheduler::put((PCB*) PCB::running);
		}
		PCB::running = Scheduler::get();
		if (PCB::running == 0) {
			PCB::running = PCB::doNothing->myPCB;
		} else {
			PCB::running->state = PCB::Active;
		}
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		PCB::cntr = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm {
			// restaurira sp
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif
		//	syncPrintf("Nova nit je : %d\n", PCB::running->id);
	}
	if (!PCB::context_switch_on_demand) {
		tick();
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}
	PCB::context_switch_on_demand = 0;
	;
}

//==============================================================================================//

//===============================Booting and shouting down the OS==================================//
void Kernel::BOOT() {
	if (!isBooted) {	//Zastita od visestrukog paljenja
		KernelMode = 1;
		PCB *MAIN = new PCB(0, 100, defaultSlice);
		PCB::cntr = MAIN->timeSlice;
		PCB::running = MAIN;
#ifndef BCC_BLOCK_IGNORE
		lock();
		pInterrupt oldRoutine = getvect(0x8);
		setvect(0x60,oldRoutine);
		setvect(0x8, timer);
		unlock();
#endif
		isBooted = 1; //Zastita od vistrukog inicijalizovanja
		KernelMode = 0;
	}
}

void Kernel::SHUTDOWN() {
	if (isBooted) {
		//syncPrintf("Ulazi u Shutdown\n");
		KernelMode = 1;
#ifndef BCC_BLOCK_IGNORE
		lock();
		pInterrupt RestRoutine=getvect(0x60);
		setvect(0x8, RestRoutine);
		unlock();
#endif
		isBooted = 0;
		KernelMode = 0;
	}
}
//===============================System calls===============================//
void Kernel::SYS_CALL(int id, void *p) {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	KernelMode = 1;

	switch (id) {
//****************************System Calls*********************************//

	case THREAD_CREATE:
		PCB::thCreate((ConstThread*) p);
		break;
	case THREAD_DESTROY:
		PCB::thDestroy((Thread *) p);
		break;
	case THREAD_START:
		PCB::thStart((Thread *) p);
		break;
	case WAIT_TO_COMPLETE:
		PCB::thWaitToComplete((Thread*) p);
		break;
	case IDLE_SETUP:
		PCB::thIdleSetup((Thread*) p);
		break;
	case THREAD_SLEEP:
		PCB::thSleep((unsigned *) p);
		break;
	case EVENT_CREATE:
		KernelEv::evCreate((EventCreate*) p);
		break;
	case EVENT_DESTROY:
		KernelEv::evDestory((KernelEv*) p);
		break;
	case EVENT_SIGNAL:
		KernelEv::evSignal((KernelEv*) p);
		break;
	case EVENT_WAIT:
		KernelEv::evWait((KernelEv*) p);
		break;
	default:
		break;

	}

//******************************Return to user mode*******************************************//
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	KernelMode = 0;
	//syncPrintf("Izlazi iz Kernel Swithcha\n");
	dispatch();
}
