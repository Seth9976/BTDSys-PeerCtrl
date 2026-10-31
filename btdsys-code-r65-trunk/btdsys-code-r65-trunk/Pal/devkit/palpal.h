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
