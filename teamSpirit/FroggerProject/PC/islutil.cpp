#include "islutil.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <windows.h>

//returns fixed format
unsigned long utilSqrt(unsigned long num)
{
//	return (unsigned long)(sqrt(num)*4096);
	return (unsigned long)(sqrt(num)*65536);
}

//return angle -2048 to +2048
/*int utilCalcAngle(int adj, int opp)
{
	return (int)((atan2(adj,opp) / 6.283185308) * 4096.0);
}*/


int utilPrintf(char* fmt, ...)				
{
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	
	OutputDebugString(buffer);

	return 0;
}
