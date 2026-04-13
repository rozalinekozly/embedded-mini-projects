/*
	header file for shared library wd.so
*/
/*
	return values: 0 = success
				   1 = failed
*/
#ifndef ILRD__WD_H__
#define ILRD__WD_H__

int MakeMeImmortal(size_t cmd_len, const char** cmd, int how_often);

void DoNotResussitate(void);

#endif
