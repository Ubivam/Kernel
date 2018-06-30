/*
 * usrmain.h
 *
 *  Created on: Jan 21, 2018
 *      Author: OS1
 */

#ifndef H_USRMAIN_H_
#define H_USRMAIN_H_

#include "Thread.h"

class UsrMain: public Thread{
public:
	UsrMain(int cnt, char**args);
	void run();

	int ret;
	char** arg;
	int j;
};




#endif /* H_USRMAIN_H_ */
