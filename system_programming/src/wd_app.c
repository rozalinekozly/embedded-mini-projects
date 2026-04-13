/*
	watch dog app version 2: user input <wd_app_exe> <frequency_check> <client_app>
*/

typedef struct
{
	pid_t pid;
	char** client_cmd;
	int frequency_check;
}client_ty;

#define UNUSED(x)	(void)x

/*forward declarations*/
static sch_op_status_ty ReviveIfNotAliveIMP(void* param);
static void RevivalCleanupIMP(void* unused);



int main(int argc, char* argv[])
{
	/*asserts*/
	/*if argc < 3 */
		/*exit*/
	
	/*declare client_ty struct*/
	/*init it's fields*/
		/*set frequency_check as argv[1] (convert to int)*/
		/*set client_cmd as argv + 2)*/
		/*set clent pid as parent pid via ppid*/
	
	/*create a scheduler*/
	/*handle failure*/
	
	/*add task to scheduler with frequency_check as an interval, op_func as ReviveIfNotAliveIMP,
	op_param &client instance, clean_func = dummy cleanup func, clean_param is null*/
	/*handle failure*/
	
	/*run scheduler*/
	
	/*cleanup*/
	/*destroy scheduler*/
	
	return 0;
}

static sch_op_status_ty ReviveIfNotAliveIMP(void* param_)
{
	/*cast param to client_ty*/

	/*send client app instance a signal via kill 0*/
	/*if fails (client is dead)*/
		/*execvp(client_cmd[0], client_cmd)*/
		/*execvp failed*/
			/*return SCH_NOT_REPEAT*/

	/*client alive */
		/*return SCH_REPEAT*/
}

static void RevivalCleanupIMP(void* param_)
{
	/*do nothing*/
	UNUSED(param_);
}

