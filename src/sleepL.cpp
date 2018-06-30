/*
 * sleep.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: OS1
 */
#include "sleepL.h"

void SleepList::add(PCB *p, unsigned &s) {
	p->state = PCB::Blocked;
	SleepNode *newl = new SleepNode(p, s);
	if (head == 0)
		head = newl;
	else {
		if (newl->timeToSleep < head->timeToSleep) {
			head->timeToSleep -= newl->timeToSleep;
			newl->next = head;
			head = newl;
		} else {
			SleepNode *current = head, *last;
			while (current != 0 && current->timeToSleep <= newl->timeToSleep) {
				newl->timeToSleep -= current->timeToSleep;
				last = current;
				current = current->next;
			}
			if (current == 0) {
				last->next = newl;
			} else {
				newl->next = current->next;
				current->next = newl;
			}
		}
	}
}

SleepList::~SleepList() {
	SleepNode *old;
	while (head != 0) {
		head->pcb->state = PCB::Ready;
		Scheduler::put(head->pcb);
		old = head;
		head = head->next;
		delete old;
	}
}

void SleepList::check() {
	if (head != 0) {
		--head->timeToSleep;
		while (head != 0 && head->timeToSleep == 0) {
			head->pcb->state = PCB::Ready;
			Scheduler::put(head->pcb);
			SleepNode *old = head;
			head = head->next;
			delete old;

		}
	}
}

