/*
params.cpp: machine paperwork
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

#include "stdafx.h"

#include "../mdk/mdk.h"
#include "ctrl.h"
#include "params.h"

CMachineParameter const paraInertia =
{
	pt_word,			//Type
	"Inertia",			//Short name
	"Inertia (length in tick/10, or 0=off)",		//Long name
	0,					//Min value
	1280,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

#ifndef _NOSLAVE
	CMachineParameter const paraSlave =
	{
		pt_switch,			//Type
		"Slaved",			//Short name
		"Slaved to previous track (1=yes 0=no)",	//Long name
		-1,					//Min value
		-1,					//Max value
		SWITCH_NO,			//No value
		MPF_STATE,			//Flags
		SWITCH_OFF			//Default value
	};
#endif

CMachineParameter const paraValue =
{
	pt_word,			//Type
	"Value",			//Short name
	"Value (0=0% FFFE=100%)",	//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

/////////////////////////////////////////////////////////////////////////////

CMachineAttribute const attrMIDIIncDecAmt =
{
	"MIDI Inc/Dec Amount",	//name
	0,		//min
	65534,	//max
	1024	//def
};
/*
CMachineAttribute const attrDblClickAction =
{
	"Doubleclick (0=params 1=XY)",	//name
	0,		//min
	1,		//max
	0		//def
};
*/
CMachineAttribute const attrSendFreq =
{
	"Send Freq (*256 smpls, 0=tick only)",	//name
	0,		//min
	64,		//max
	2		//def
};

CMachineAttribute const attrStopOnMute =
{
	"No ctrl on mute?",	//name
	0,		//min
	1,		//max
	0		//def
};

CMachineAttribute const attrNextTickMode =
{
	"Use NextTick as well (0=never, 1=only MPF_STATE, 2=always)",	//name
	0,		//min
	2,		//max
	1		//def
};

/////////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraInertia,

#	ifndef _NOSLAVE
		&paraSlave,
#	endif
	&paraValue,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	&attrMIDIIncDecAmt,
//	&attrDblClickAction,
	&attrSendFreq,
	&attrStopOnMute,
	NULL
};
