/*
 * queue
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */

#ifndef H_QUEUE_H_
#define H_QUEUE_H_

#include "pcb.h"

class PCB;

class Queue {
	struct Elem {
		PCB* p;
		Elem* next;
		Elem(PCB* pp, Elem* nxt = 0) {
			p = pp;
			next = nxt;
		}
	};
public:
	Queue();
	Queue(PCB* p);
	~Queue();
	Elem* first;
	Elem* last;
	void put(PCB* pcb);
	PCB* get();
protected:
	friend class PCB;
};

#endif /* H_QUEUE_H_ */
