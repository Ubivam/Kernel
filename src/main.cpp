/*
 * main.cpp
 *
 *  Created on: May 28, 2017
 *      Author: OS1
 */

#include "kernel.h"
#include "idle.h"
#include "UsrMain.h"

//=======================================//
int syncPrintf(const char *format, ...);


int main(int argc, char *argv[]) {

	//---------------BOOT---------------//
	Kernel::BOOT();
	Idle * IDLE = new Idle();  //Pravljenje IDLE niti
	Kernel::SYS_CALL(IDLE_SETUP, IDLE);  //Setup IDLE niti
	syncPrintf("\n\n\n**************************************************************\n888       888        888   \n888   o   888        888       \n888  d8b  888        888           \n888 d888b 888 .d88b. 888 .d8888b .d88b. 88888b.d88b.  .d88b.  \n888d88888b888d8P  Y8b888d88P\"   d88\"\"88b888 \"888 \"88bd8P  Y8b  \n88888P Y8888888888888888888     888  888888  888  88888888888 \n8888P   Y8888Y8b.    888Y88b.   Y88..88P888  888  888Y8b.  \n888P     Y888 \"Y8888 888 \"Y8888P \"Y88P\" 888  888  888 \"Y8888\n**************************************************************\n\n\n");


	//----------------MAIN---------------//
	UsrMain* uMain=new UsrMain(argc,argv);
	uMain->start();
	int usrmn=uMain->ret;

	//-------------SHUTDOWN--------------//
	uMain->~UsrMain();
    syncPrintf("Izvrsio sam usera\n");
	IDLE->~Idle();
	Kernel::SHUTDOWN();
	syncPrintf("Goodbye!!!\n");

	return usrmn;
}
