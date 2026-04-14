/*
	client app that uses wd.h functions MakeMeImmortal and DoNotResussitate
	version 1
*/ 
#include <stdio.h>		/*printf()*/
#include <unistd.h> 	/* getpid, sleep */
/*-----------------------------------------------------------------------------*/
#include "utils.h"		/*EXIT_IF_BAD*/
#include "wd.h"			/*DoNotResussitate, MakeMeImmortal*/
/*-----------------------------------------------------------------------------*/
enum
{
	CLIENT_HOW_OFTEN = 2,
	NUM_TO_PRINT = 2000
};
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	 size_t i = 0;
	 int status = 0;
	 pid_t pid = getpid();
	/*call MakeMeImmortal*/
	status = MakeMeImmortal((argc), (const char**)argv, CLIENT_HOW_OFTEN);
	/*check return value*/
		/*if failed*/
		/*exit*/
	EXIT_IF_BAD(0 == status, 1, "failed to create wd ");
	/*print PID 2000 times*/
	for(i = 0 ; i < NUM_TO_PRINT ; i++)
	{
		printf("client app PID: %d\n", pid);
		sleep(1);
	}
	/*call DoNotResussitate*/
	DoNotResussitate();
	return 0;
}
