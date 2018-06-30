/*
 * usrmain.cpp
 *
 *  Created on: Jan 21, 2018
 *      Author: OS1
 */
#include "usrmain.h"

int syncPrintf(const char *format, ...);
int userMain(int argc, char* argv[]);

UsrMain::UsrMain(int cnt, char** args) {
	ret = -1;
	j = cnt;
	arg = args;
}
void UsrMain::run() {
	syncPrintf("USO U UsrMaim::run \n");
	ret = userMain(j, arg);
}

