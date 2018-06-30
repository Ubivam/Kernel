/*
 * event.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: OS1
 */
#include "../h/event.h"

struct EventCreate;

Event::Event(IVTNo ivtNo) {
	myImpl=0;
	EventCreate* ec= new EventCreate(ivtNo,this);
	Kernel::SYS_CALL(EVENT_CREATE,ec);
}
Event::~Event() {
	Kernel::SYS_CALL(EVENT_DESTROY,myImpl);
}

void Event::wait(){
	Kernel::SYS_CALL(EVENT_WAIT,myImpl);
}
void Event::signal(){
	Kernel::SYS_CALL(EVENT_SIGNAL,myImpl);
}

