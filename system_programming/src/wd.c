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

static void FillWdArgsIMP(char** wd_args, const char** cmd, size_t cmd_len, char* how_often_str);

int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often)
{
	char** wd_args = NULL; 
	char how_often_str[MAX_DIGITS] = {0};
    

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
	
	/*convert how_often to str*/
    sprintf(how_often_str, "%d", how_often);
	/*set wd_args enteries */
		/*call FillWdArgsIMP*/
	FillWdArgsIMP(wd_args, cmd, cmd_len, how_often_str);
	
	/*g_w_pid = fork*/
	g_wd_pid = fork();
	/*if failed*/
	if(-1 == g_wd_pid)
		{
			/*return 1 (=failed)*/
			return FAIL;
		}
	/*if child*/
	if(0 == g_w_pid)
	{
		/*replace code with wd_app with proper arguments via execvp*/
		execvp(wd_args[0], wd_args);
		/*if execvp failed*/
			/*exit*/
		exit(1);
	}
	/*if parent*/
	else
	{
		/*version 1: sleep 2 seconds */
		sleep(2);
		/*free wd_args*/
		free(wd_args);
	}
	
	/*return 0 = success*/
	return SUCCESS;
}

static void FillWdArgsIMP(char** wd_args, const char** cmd, size_t cmd_len, char* how_often_str)
{
	size_t i = 0;

    
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
    kill(g_wd_pid, SIGTERM);
    
    /*wait for it to die before returning*/
    waitpid(g_wd_pid, NULL, 0);
}
