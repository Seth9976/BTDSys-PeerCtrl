/*
2002-2004, Edward Powley
This file is public domain.
*/

//Code for "hacking" CMachine in Buzz machines
//Ed Powley (BTDSys) July 2002

#ifndef _BTDSYS_CMACHINE_HACK_
#define _BTDSYS_CMACHINE_HACK_

#include "../mdk/mdk.h"

//Make sure that the search won't go on forever if it's not there
#define MAX_SEARCH_LENGTH	0xFFFF

long lMIOffset;
bool bHaveMIOffset;

/////////////////////////////////////////////////////////////////////

bool FindMIOffset (CMachine *Machine, CMachineInterface *MachineInterface)
//Find the offset of the CMachineInterface pointer
{
	long TempOffset = 0;
	long *p;

	bool FoundFlag = false;
	bool ExitFlag = false;

	while (!ExitFlag)
	{
		p = (long*)Machine + TempOffset;
		if (*p == (long)MachineInterface)
		{
			lMIOffset = TempOffset;
			ExitFlag = FoundFlag = true;
		}
		TempOffset++;
		if (TempOffset > MAX_SEARCH_LENGTH) ExitFlag = true;
	}

	bHaveMIOffset = FoundFlag;

	return FoundFlag;
}

/////////////////////////////////////////////////////////////////////

CMachineInterface *GetMI (CMachine *Machine)
//Get a CMachine's CMachineInterface
{
	if (bHaveMIOffset)
	{
		long *p = (long*)Machine + lMIOffset;
		return (CMachineInterface*)*p;
	}
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////

long GetParamOffset (const CMachineInfo *minfo, 
					 byte Group, byte Track, byte ParaNum)
//Get byte offset of a parameter in GlobalVals or TrackVals
//ParaNum follows same convention as ControlChange function
//ie both global and track params are zero-based
{
	long Offset = 0;
	long TrackValsSize = 0;
	int i;

	switch(Group)
	{
	case 1: //Global
		for (i=0; i < ParaNum; i++)
		{
			if (minfo->Parameters[i]->Type == pt_word)
				Offset += 2;	//words are 2 bytes long
			else
				Offset ++;		//bytes are 1 byte long (surprise)
		}
		return Offset;
		break;

	case 2: //Track
		//First, find the size of a track's data
		for (i = minfo->numGlobalParameters; 
			 i < minfo->numGlobalParameters + minfo->numTrackParameters;
			 i++)
		{
			if (minfo->Parameters[i]->Type == pt_word)
				TrackValsSize += 2;
			else
				TrackValsSize ++;
		}
		
		//Now get the offset
		for (i=0; i<ParaNum; i++)
		{
			if (minfo->Parameters[i + minfo->numGlobalParameters]->Type == pt_word)
				Offset += 2;
			else
				Offset ++;
		}
		return Offset + (Track * TrackValsSize);
		break;

	default:
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////

byte *GetParamByte(const CMachineInterface *mint,
				   const CMachineInfo *minfo,
				   byte Group, byte Track, byte ParaNum)
//Return a byte pointer to some parameter data
{
	switch(Group)
	{
	case 1: //Global
		return (byte*)mint->GlobalVals
			+ GetParamOffset(minfo,Group,Track,ParaNum);

	case 2: //Track
		return (byte*)mint->TrackVals
			+ GetParamOffset(minfo,Group,Track,ParaNum);

	default:
		return NULL;
	}
}

/////////////////////////////////////////////////////////////////////

word *GetParamWord(const CMachineInterface *mint,
				   const CMachineInfo *minfo,
				   byte Group, byte Track, byte ParaNum)
//Return a word pointer to some parameter data
{
	return (word*) GetParamByte(mint,minfo,Group,Track,ParaNum);
}

/////////////////////////////////////////////////////////////////////

void Hack_ControlChange (const CMachineInfo *minfo, 
						 CMachine *pmac, 
						 int group, int track, int param, int value)
//Force a parameter change
{
	const CMachineParameter *Par;

	if (group == 1) //Global
		Par = minfo->Parameters[param];
	else
		Par = minfo->Parameters[param + minfo->numGlobalParameters];

	if (Par->Type == pt_word)
	{
		word *p = GetParamWord(GetMI(pmac),
								minfo,
								group,
								track,
								param);
		*p = value;
	}
	else
	{
		byte *p = GetParamByte(GetMI(pmac),
								minfo,
								group,
								track,
								param);
		*p = value;
	}
}

/////////////////////////////////////////////////////////////////////

void Hack_ControlsClear(const CMachineInfo *minfo, 
						CMachine *pmac)
//Set all parameter values to their "NoValues"
{
	//Global
	int Offset = 0;
	void *pg = GetMI(pmac)->GlobalVals;

	for (int i=0; i<minfo->numGlobalParameters; i++)
	{
		const CMachineParameter *Par = minfo->Parameters[i];

		byte *p = (byte*)pg + Offset;

		if (Par->Type == pt_word)
		{
			word *q = (word*)p;
			*q = Par->NoValue;

			Offset += 2;
		}
		else
		{
			*p = Par->NoValue;

			Offset ++;
		}
	}

	//Track
	Offset = 0;
	void *tg = GetMI(pmac)->TrackVals;

	for (int t=0; t<minfo->maxTracks; t++)
		for (int j=0; j<minfo->numTrackParameters; j++)
		{
			const CMachineParameter *Par = minfo->Parameters[j + minfo->numGlobalParameters];

			byte *p = (byte*)tg + Offset;

			if (Par->Type == pt_word)
			{
				word *q = (word*)p;
				*q = Par->NoValue;

				Offset += 2;
			}
			else
			{
				*p = Par->NoValue;

				Offset ++;
			}
		}
}

#endif
