/* shared library of watchdog that contains 
	implementation
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
void DoNotResussitate(void)
*/
enum
{
	MAX_DIGITS = 20
};
static pid_t g_wd_pid = 0;

static void FillWdArgsIMP(char** wd_args, const char** cmd, size_t cmd_len, int how_often);

int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
{
	char** wd_args = NULL; 
	
	/*asserts*/
	assert(how_often > 0);
	assert(NULL != cmd);
	
	/*build a wd_args strings array (char**) of size cmd_len+3*/
	wd_args = (char**)malloc((cmd_len+3)*sizeof(char*));
	/*handle failure*/
	if(NULL == wd_args)
	{
		return FAIL;
	}
	
	/*set wd_args enteries */
		/*call FillWdArgsIMP*/
	FillWdArgsIMP(wd_args, cmd, cmd_len, how_often);
	
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

static void FillWdArgsIMP(char** wd_args, const char** cmd, size_t cmd_len, int how_often)
{
	size_t i = 0;
    char how_often_str[MAX_DIGITS] = {0};
    
    /*convert how_often to str*/
    sprintf(how_often_ste, "%d", how_often);
    
    /*set wd_args[0] = "./wd_app"*/
    wd_args[0] = "./wd_app";
    
    /*wd_args[1] = how_oftenl_str*/
    wd_args[1] = how_often_str;
    
    /* copy cmd into wd_args[2...]*/
    for (i = 0; i < cmd_len; i++)
    {
        wd_args[i + 2] = (char*)cmd[i];
    }
    
    /*set wd_args[cmd_len + 2] = NULL (for execvp)*/
    wd_args[cmd_len + 2] = NULL;
}
void DoNotResussitate(void)
{
	/*send SIGTERM to g_wd_pid*/
	/*wait og it to die before returning*/
}
