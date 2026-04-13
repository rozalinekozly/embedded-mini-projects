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
	/*fork*/
	/*if failed*/
		/*return 1 (=failed)*/
	
	/*if child*/
		/*replace code with wd_app with proper arguments via execvp*/
		/*if execvp failed*/
			/*exit*/
	
	/*if parent*/
		/*version 1: sleep 2 seconds */
	
	/*return 0 = success*/
}

void DoNotResussitate(void)
{

}
