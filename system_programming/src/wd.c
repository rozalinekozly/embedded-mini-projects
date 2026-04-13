/* shared library of watchdog that contains 
	1- MakeMeImmortal 
	2- DoNotResussitate
	implementation
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
void DoNotResussitate(void)
*/
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
{
	/*asserts*/
	
	/*build a wd_args strings array (char**) of size cmd_len+3*/
	/*handle failure*/
	/*set wd_args enteries */
		/*set wd_args[0] ="./wd_app"*/
		/*wd_args[1] = how_often via sprintf*/
		/*the rest entries with cmd and null at the end(execvp requires it)*/
		
	/*fork*/
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

}
