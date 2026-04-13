/*
	watch dog app version 2: user input <wd_app_exe> <frequency_check> <client_app>
*/
typedef struct
{
	pid_t pid;
	char** client_cmd;
	int frequency_check;
}client_ty;

int main(int argc, char* argv[])
{
	/*asserts*/
	/*if argc < 3 */
		/*exit*/
	
	/*declare client_ty struct*/
	/*init it's fields*/
		/*set frequency_check as argv[1] (convert to int)*/
		/*set client_cmd as argv[2]*/
		/*set clent pid as parent pid via ppid*/
	
	/*create a scheduler*/
	/*handle failure*/
	
	/*add task to scheduler with frequency_check as an interval*/
	/*handle failure*/
	
	/*run scheduler*/
	
	/*cleanup*/
	/*destroy scheduler*/
	
	return 0;
}
