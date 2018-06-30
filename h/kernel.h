/*
 * Kernel.h
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */

#ifndef H_KERNEL_H_
#define H_KERNEL_H_
#include "pcb.h"
#include "kernSem.h"
#include "kernEv.h"
#include "def.h"

//*********************************************************************************//
//============================SISTEMSKA KLASA KERNEL===============================//
class PCB;
class KernelSem;
class KernelEv;

int syncPrintf(const char *format, ...);

class Kernel {
private:
	volatile static unsigned isBooted;
	volatile static unsigned KernelMode;

	friend void interrupt timer(...);

public:
	void static BOOT();
	void static SYS_CALL(int id, void *p);
	void static SHUTDOWN();
};
//==================================================================================//
extern void tick();

#endif /* H_KERNEL_H_ */
