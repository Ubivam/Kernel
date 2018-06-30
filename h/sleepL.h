/*
 * sleepList.h
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */

#ifndef SLEEPL_H_
#define SLEEPL_H_
#include "PCB.h"
#include <DOS.H>
class PCB;

class SleepList
{
private:
	struct SleepNode
	{
		PCB *pcb;
		SleepNode *next;
		unsigned timeToSleep;			//time to sleep
		SleepNode(PCB *p, unsigned &s) : pcb(p), timeToSleep(s), next(0){}
	};

	SleepNode *head;

	SleepList() :head(0){}
	~SleepList();
	void add(PCB *p, unsigned &s);
	void check();		//poziva se iz prekidne rutine tajmera

	//spisak prijateljskih klasa i funkcija
	friend class PCB;
	friend void interrupt timer(...);
};



#endif /* SLEEPL_H_ */

