/*
	watch dog app: user input <wd_app_exe> <frequency_check> <client_app>
	This process is spawned by wd_thrd (a thread inside the client process).
    Its job is to monitor the client process and revive it if it dies.
    In return, wd_thrd monitors this process and revives it if it dies.
    This creates a mutual protection system ie each side watches the other.
*/
/*TODO:
- change scheduler to support for users to pass an empty function for cleanup(does nothing).
- give perameters more meaningful names
- add check if scheduler runningfailed SCH_FAIL
- use getppid to check validity rather than kill 0 
- errno of kill check it! 
- */
/*-----------------------------------------------------------------------------*/
#define _POSIX_SOURCE
#include <signal.h>		/* kill */
#include <unistd.h>		/* getppid, execvp*/
#include <stdlib.h>		/* atoi*/
#include <assert.h>		/* assert  */
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>		/*for printing remove later*/
/*-----------------------------------------------------------------------------*/
#include "utils.h"
#include "scheduler.h"
/*-----------------------------------------------------------------------------*/
typedef struct
{
	pid_t m_pid;
	char** m_client_cmd;
	int m_frequency_check;
}client_ty;
/*-----------------------------------------------------------------------------*/

