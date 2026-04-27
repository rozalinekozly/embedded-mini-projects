#include <sys/types.h>  /*pid_t */ 
#include <stdio.h>		/*printf()*/
#include <unistd.h> 	/* getpid, sleep */
/*-----------------------------------------------------------------------------*/
#include "utils.h"		/*EXIT_IF_BAD*/
#include "wd.h"			/*DoNotResussitate, MakeMeImmortal*/
/*-----------------------------------------------------------------------------*/
enum
{
	CLIENT_HOW_OFTEN = 2,
};
/*-----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	 int status = 0;
	 pid_t pid = getpid();
	/*call MakeMeImmortal*/
	status = MakeMeImmortal((argc), (const char**)argv, CLIENT_HOW_OFTEN, 3);
	/*check return value*/
		/*if failed*/
		/*exit*/
	EXIT_IF_BAD(0 == status, 1, "failed to create wd ");
	while(1)
	{
		printf("client app PID: %d\n", pid);
		sleep(2);
	}
	/*call DoNotResuscitate*/
	DoNotResuscitate();
	return 0;
}
