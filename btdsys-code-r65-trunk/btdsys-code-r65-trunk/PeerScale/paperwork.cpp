/*
Copyright (c) 2005-2011, Edward Powley
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

#include "machine.h"

CMachineParameter const paraScale =
{
	pt_word,			//Type
	"Scale",			//Short name
	"Scale",			//Long name
	0,					//Min value
	MAX_SCALES-1,		//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraRoot =
{
	pt_byte,			//Type
	"Root",				//Short name
	"Root",				//Long name
	0,					//Min value
	11,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLowOct =
{
	pt_byte,			//Type
	"Low octave",		//Short name
	"Low octave",		//Long name
	0,					//Min value
	9,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	4					//Default value
};

CMachineParameter const paraHighOct =
{
	pt_byte,			//Type
	"High octave",		//Short name
	"High octave",		//Long name
	0,					//Min value
	9,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	5					//Default value
};

CMachineParameter const paraPeerMe =
{
	pt_word,						//Type
	"Peer Me!",						//Short name
	"Peer Me! (aka value)",			//Long name
	0,								//Min value
	0x8000,							//Max value
	0xFFFF,							//No value
	MPF_STATE | MPF_TICK_ON_EDIT,	//Flags
	0								//Default value
};

CMachineParameter const paraTargetTrack =
{
	pt_byte,						//Type
	"Targ Track",					//Short name
	"Target Track (FE = all)",		//Long name
	0,								//Min value
	0xFE,							//Max value
	0xFF,							//No value
	MPF_STATE,						//Flags
	0								//Default value
};

////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraScale,
	&paraRoot,
	&paraLowOct,
	&paraHighOct,
	&paraPeerMe,
	&paraTargetTrack,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

////////////////////////////////////////////////////////////////////////

CMachineInfo const MacInfo =
{
	MT_GENERATOR,																//Machine type
	MI_VERSION,																	//Interface version
	MIF_CONTROL_MACHINE | MIF_NO_OUTPUT,					//Flags
	0,																			//Min tracks
	0,																			//Max tracks
	NUMG,																		//Number of global parameters
	0,																			//Number of track parameters
	pParameters,																//Pointer to parameters
	NUMA,																		//Number of attributes
	pAttributes,																//Pointer to attributes
	MACHINE_NAME,															//Full name
	"PeerScale",																//Short name
	MACHINE_AUTHOR,														//Author
	"Assign...\nChoose scale...\nCustom scales...\nMIDI settings...\nAbout..."	//Commands
};
