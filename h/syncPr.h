/*
 * syncPr.cpp
 *
 *  Created on: Aug 6, 2020
 *      Author: OS1
 */

#ifndef _SYNCPR_H_
#define _SYNCPR_H_

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
// zabranjuje prekide
//#define lock asm cli

// dozvoljava prekide
//#define unlock asm sti

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	//lock;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	//unlock;
		return res;
}

#endif
