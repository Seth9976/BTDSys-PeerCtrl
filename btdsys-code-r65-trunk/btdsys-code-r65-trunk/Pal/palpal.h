/*
palpal.h: Main header file for writing add-on DLLs for PAL.
2005, Edward Powley
This file is public domain.
*/

#ifndef __BTDSYS_PALPAL_H
#define __BTDSYS_PALPAL_H

class CMachine;

struct PalPalAssign
{
	CMachine *targetmac;
	int targetgroup;
	int targetparam;
	int targettrack;
	char comment[64];
};

#endif
