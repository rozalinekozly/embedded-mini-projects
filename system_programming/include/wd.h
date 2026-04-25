/*
	header file for shared library wd.so
*/

#ifndef ILRD__WD_H__
#define ILRD__WD_H__

enum
{
	SUCCESS = 0,
	FAIL = 1
};

/*
	return values: 0 = success
				   1 = failed
*/
int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often, int fail_cnt);

void DoNotResuscitate(void);

#endif
