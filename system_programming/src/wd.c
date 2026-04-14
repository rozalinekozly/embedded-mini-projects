/* shared library of watchdog that contains 
	implementation
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
void DoNotResussitate(void)
*/

static pid_t g_wd_pid = 0;

int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
{
	char** wd_args = NULL; 
	
	/*asserts*/
	assert(how_often > 0);
	assert(NULL != cmd);
	
	/*build a wd_args strings array (char**) of size cmd_len+3*/
	/*handle failure*/
	/*set wd_args enteries */
		/*set wd_args[0] ="./wd_app"*/
		/*wd_args[1] = how_often via sprintf*/
		/*the rest entries with cmd and null at the end(execvp requires it)*/
		
	/*g_w_pid = fork*/
	/*if failed*/
		/*return 1 (=failed)*/
		
	/*if child*/
		/*replace code with wd_app with proper arguments via execvp*/
		/*if execvp failed*/
			/*exit*/
	
	/*if parent*/
		/*version 1: sleep 2 seconds */
		/*free wd_args*/
	
	/*return 0 = success*/
}

void DoNotResussitate(void)
{
	/*send SIGTERM to g_wd_pid*/
	/*wait og it to die before returning*/
}
