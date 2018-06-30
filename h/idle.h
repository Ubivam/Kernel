/*
 * idle.h
 *
 *  Created on: May 30, 2017
 *      Author: OS1
 */

#ifndef H_IDLE_H_
#define H_IDLE_H_
class Thread;
class Idle: public Thread {
public:
	Idle() :
			Thread(4096, 1) {
	}
	void run();
	~Idle() {
	}
};
void Idle::run() {
	while (1)
		;
}

#endif /* H_IDLE_H_ */
