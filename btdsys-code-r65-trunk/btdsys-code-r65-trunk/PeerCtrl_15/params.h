/*
params.h: machine paperwork
Copyright (c) 2002-2006, Edward Powley
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of BTDSys nor the names of its contributors may be used
      to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PEERCTRL_PARAMS_H
#define PEERCTRL_PARAMS_H

#include "../mdk/mdk.h"

extern CMachineParameter const paraInertia;
#ifndef _NOSLAVE
	extern CMachineParameter const paraSlave;
#endif
extern CMachineParameter const paraValue;
extern CMachineAttribute const attrMIDIIncDecAmt;
//extern CMachineAttribute const attrDblClickAction;
extern CMachineAttribute const attrSendFreq;

extern CMachineParameter const *pParameters[];
extern CMachineAttribute const *pAttributes[];

enum {
	numInertia = 0,
	NUMG,

#	ifndef _NOSLAVE
		numSlave = NUMG,
		numValue,
#	else
		numValue = NUMG,
#	endif
	NUMT = numValue + 1 - NUMG,

	NUMA = 3,

#	ifndef _NOSLAVE
		numValueInT = 1,
#	else
		numValueInT = 0,
#	endif

};

#pragma pack(1)

class gvals
{
public:
	word Inertia;
};

class tvals
{
public:
#	ifndef _NOSLAVE
		byte Slave;
#	endif
	word Value;
};

class avals
{
public:
	int MIDIIncDecAmt;
//	int DblClickAction;
	int SendFreq;
	int StopOnMute;
};

#pragma pack()


#endif
