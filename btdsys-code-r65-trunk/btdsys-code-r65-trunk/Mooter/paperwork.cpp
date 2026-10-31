/*
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
#include "machine.h"

CMachineParameter const paraMute =
{
	pt_byte,			//Type
	"Mute",				//Short name
	"Mute",				//Long name
	0,					//Min value
	1,					//Max value
	255,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraPatternState =
{
	pt_byte,			//Type
	"PatState",			//Short name
	"Pattern State",	//Long name
	0,					//Min value
	2,					//Max value
	255,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraMute,
	&paraPatternState,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

////////////////////////////////////////////////////////////////////////

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_MONO_TO_STEREO | MIF_NO_OUTPUT | MIF_CONTROL_MACHINE,				//Flags
	8,								//Min tracks
	200,							//Max tracks
	NUMG,							//Number of global parameters
	NUMT,							//Number of track parameters
	pParameters,					//Pointer to parameters
	NUMA,							//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys Mooter",				//Full name
	"Mooter",						//Short name
	"Ed Powley (BTDSys)",			//Author
	"/Assign\nAbout..."						//Commands
};
