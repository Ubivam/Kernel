/*
 * queue.cpp
 *
 *  Created on: May 15, 2018
 *      Author: OS1
 */
#include "../h/queue.h"

#include <stdlib.h>
#include "queue.h"
#include "pcb.h"
#include "def.h"

Queue::Queue() {
	first = 0;
	last = 0;
}

Queue::Queue(PCB* p) {
	first = last = new Elem(p);
}

Queue::~Queue() {
	Elem* temp = first;
	while (first != 0) {
		temp = first;
		first = first->next;
		temp->next = 0;
		delete temp;
	}
	last = 0;
}

PCB* Queue::get() {
	if (first == 0)
		return 0;
	Elem* help = first;
	first = first->next;
	PCB* help_pcb = help->p;
	delete help;
	if (first == 0)
		last = 0;
	return help_pcb;
}

void Queue::put(PCB* p) {
	if (first == 0) {
		first = new Elem(p);
		last = first;
	} else {
		Elem *temp = new Elem(p);
		last->next = temp;
		last = last->next;
	}
}

