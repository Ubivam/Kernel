/*
 * Def.h
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */

#ifndef H_DEF_H_
#define H_DEF_H_

#include "SCHEDULE.h"
#include <dos.h>

#define TEST
//#undef TEST
// ......SYS_CALLS......... //
#define THREAD_CREATE 1
#define	THREAD_DESTROY 2
#define	THREAD_START 3
#define WAIT_TO_COMPLETE 4
#define IDLE_SETUP 5
#define THREAD_SLEEP 6
#define EVENT_CREATE 7
#define EVENT_DESTROY 8
#define EVENT_SIGNAL 9
#define EVENT_WAIT 10

// ........LOCKS......... //
#define lock() {asm {pushf;cli;}}
#define unlock() {asm {popf;}}

// .............DEFAULTS............. //
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef int Time;
const Time defaultSlice = 2;

typedef int ID;

typedef void interrupt (*pInterrupt)(...);
void interrupt timer(...);

typedef unsigned char IVTNo;

#define PREPAREENTRY(ivtNo,callOldRout)\
void interrupt intIVTEntry##ivtNo(...); \
IVTEntry entry##ivtNo(ivtNo, intIVTEntry##ivtNo); \
void interrupt intIVTEntry##ivtNo(...) \
{\
	entry##ivtNo.signal();\
	if (callOldRout==0) return;\
	if (callOldRout==1) entry##ivtNo.callOldRoutine();\
}

#endif /* H_DEF_H_ */
