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

#include "common.h"
#include "core.h"
#include "sgrid.h"

#include "../../mdk/mdk.h"

const char *pszPeerNames[] = {
	"Trigger",
	"Vel/Vol",
	"Pan",
	"Tune",
	"Offset",
	"Aux 1",
	"Aux 2"
};

const char *pszGroupMuteModes[] = {
	"Toggle",
	"Momentary mute",
	"Momentary unmute"
};

const char *pszLoopModes[] = {
	"None",
	"Straight",
	"Pingpong",
	"Random"
};

const char *pszTriggerModes[] = {
	"Vel",
	"Pan",
	"Tune",
	"Prob",
	"Arg 1",
	"Arg 2",
	"Offset"
};


const int nPeerAllowedTypes[] = {
	ALLOW_ALL_GROUPS | ALLOW_ALL_TYPES,
	ALLOW_ALL_GROUPS | ALLOW_BYTE | ALLOW_WORD,
	ALLOW_ALL_GROUPS | ALLOW_BYTE | ALLOW_WORD,
	ALLOW_ALL_GROUPS | ALLOW_BYTE | ALLOW_WORD | ALLOW_NOTE,
	ALLOW_ALL_GROUPS | ALLOW_BYTE | ALLOW_WORD
};

const char *pszCommands[] = {
	"Group Setup...",
	"Group Matrix...",
	"/Drumkit",
//	"Loop slicer...",
	"Envelopes...",
//	"Shuffle Editor...",
	"Command list...",
	"About..."
};

CMachineAttribute const attrRampLen =
{
	"Ramp Length (samples)",		//Name
	0,						//Min
	1000,					//Max
	50						//Default
};

CMachineAttribute const attrRampVolume =
{
	"Ramp Volume (0=off 1=on)",		//Name
	0,						//Min
	1,						//Max
	0						//Default
};

CMachineAttribute const attrHumanMode =
{
	"Humanisation Mode (0=rect 1=normal)",		//Name
	0,						//Min
	1,						//Max
	1						//Default
};

CMachineAttribute const attrInertiaVolume =
{
	"Inertia Volume (0=off 1=on)",		//Name
	0,						//Min
	1,						//Max
	1						//Default
};

CMachineAttribute const attrInertiaPan =
{
	"Inertia Pan (0=off 1=on)",		//Name
	0,						//Min
	1,						//Max
	1						//Default
};

CMachineAttribute const attrInertiaTune =
{
	"Inertia Tune (0=off 1=on)",		//Name
	0,						//Min
	1,						//Max
	1						//Default
};

CMachineAttribute const attrChannels =
{
	"Virtual channels per track",		//Name
	1,						//Min
	128,					//Max
	4						//Default
};

CMachineAttribute const attrMIDIRecord =
{
	"MIDI record",  		//Name
	0,						//Min
	1,  					//Max
	1						//Default
};

CMachineAttribute const attrMIDIRecordOffs =
{
	"MIDI record note offs",  		//Name
	0,						//Min
	1,  					//Max
	1						//Default
};

CMachineAttribute const attrMIDIRecordDelays =
{
	"MIDI record delays (0 = off, 1 = command 1, 2 = command 2)",  		//Name
	0,						//Min
	2,  					//Max
	1						//Default
};

////////////////////////////////////////////////////////////////////////

CMachineAttribute const *pAttributes[] = {
	&attrRampLen,
	&attrRampVolume,
	&attrHumanMode,
	&attrInertiaVolume,
	&attrInertiaPan,
	&attrInertiaTune,
	&attrChannels,
    &attrMIDIRecord,
    &attrMIDIRecordOffs,
    &attrMIDIRecordDelays,
	NULL
};

//GLOBAL//////////////////////////////////////////////////

CMachineParameter const paraFirstWaveNo =
{
	pt_byte,			//Type
	"First Wave",		//Short name
	"First Wave Number (not used for PSI drumkits)",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_WAVE | MPF_STATE,	//Flags
	1					//Default value
};

char textTriggers[MAXTRIGGERS][64];
char nameTriggers[MAXTRIGGERS][32];

CMachineParameter paraTriggers[MAXTRIGGERS];

CMachineParameter const paraSeparator =
{
	pt_switch,			//Type
	"----------",		//Short name
	"(separator)",		//Long name
	-1,					//Min value
	-1,					//Max value
	-1,					//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraDivider = 
{
	pt_switch,			//Type
	"(Divider)",		//Short name
	"This is just a divider, it does nothing useful",	//Long name
	-1,					//Min value
	-1,					//Max value
	-1,					//No value
	0,					//Flags
	1					//Default value
};

CMachineParameter const paraTriggerType = 
{
	pt_byte,			//Type
	"Trig Type",		//Short name
	"Trigger Type",		//Long name
	0,					//Min value
	NUM_TRIGGER_MODES + (NUM_TRIGGER_MODES)*(NUM_TRIGGER_MODES-1) - 1,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter paraSolo = 
{
	pt_byte,			//Type
	"Solo Track",		//Short name
	"Solo Track",		//Long name
	0,					//Min value
	0,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraShuffleSize =
{
	pt_byte,			//Type
	"Shuffle Size",		//Short name
	"Shuffle Size (0=off, 80=0.25 tick, FE=~0.50 tick)",	//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraShuffleStep =
{
	pt_byte,			//Type
	"Shuffle Step",		//Short name
	"Shuffle Step (in ticks)",	//Long name
	2,					//Min value
	32,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	2					//Default value
};

CMachineParameter const paraShuffleRandom =
{
	pt_byte,			//Type
	"Shuffle Rnd",	//Short name
	"Shuffle Randomness (0=0% FE=100%)",	//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraShuffleReset =
{
	pt_switch,			//Type
	"Shuffle Reset",	//Short name
	"Shuffle Reset (1=reset 0=no effect)",	//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	0,					//Flags
	SWITCH_OFF			//Default value
};

CMachineParameter const paraHumanVel =
{
	pt_byte,			//Type
	"Human Vel",		//Short name
	"Velocity humanisation",	//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraHumanPan =
{
	pt_byte,			//Type
	"Human Pan",		//Short name
	"Pan humanisation",	//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraHumanTune =
{
	pt_byte,			//Type
	"Human Tune",		//Short name
	"Tune humanisation",	//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraInertia = 
{
	pt_word,			//Type
	"Inertia",			//Short name
	"Inertia (0=off, 1=0.1 ticks, 500=128.0 ticks)",		//Long name
	0,					//Min value
	0x500,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

//TRACK//////////////////////////////////////////////////

CMachineParameter paraTrackTrigger;

CMachineParameter const paraWaveNo =
{
	pt_byte,			//Type
	"Wave No",			//Short name
	"Wave Number (or 0=Auto)",		//Long name
	0,					//Min value
	WAVE_MAX,			//Max value
	0xFF,				//No value
	MPF_WAVE | MPF_STATE,	//Flags
	0					//Default value
};

CMachineParameter const paraCommand1 =
{
	pt_byte,			//Type
	"Command 1",		//Short name
	"Command 1 (See help file for details)",			//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraArgument1 =
{
	pt_byte,			//Type
	"Argument 1",		//Short name
	"Argument 1 (See help file for details)",			//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraCommand2 =
{
	pt_byte,			//Type
	"Command 2",		//Short name
	"Command 2 (See help file for details)",			//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraArgument2 =
{
	pt_byte,			//Type
	"Argument 2",		//Short name
	"Argument 2 (See help file for details)",			//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraLenUnit =
{
	pt_byte,			//Type
	"Len Unit",			//Short name
	"Unit of length for commands",			//Long name
	0,					//Min value
	0x14,				//Max value
	0xFF,	    		//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraGlobalLenUnit =
{
	pt_byte,			//Type
	"Len Unit",			//Short name
	"Unit of length for commands",			//Long name
	1,					//Min value
	0x14,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	6					//Default value
};

CMachineParameter const paraMute =
{
	pt_switch,			//Type
	"Mute",				//Short name
	"Mute (0=not muted, 1=muted)",			//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	SWITCH_OFF			//Default value
};

CMachineParameter const paraOffset =
{
	pt_word,			//Type
	"Start",			//Short name
	"Start Offset (8000 = 50%, FFFE = ~100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLoopStart =
{
	pt_word,			//Type
	"Loop Start",		//Short name
	"Loop Start (8000 = 50%, FFFE = ~100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLoopEnd =
{
	pt_word,			//Type
	"End",				//Short name
	"End Offset (8000 = 50%, FFFF = ~100%)",		//Long name
	1,					//Min value
	0xFFFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0xFFFF				//Default value
};

CMachineParameter const paraLoopMode =
{
	pt_byte,			//Type
	"Loop Mode",		//Short name
	"Loop Mode",		//Long name
	0,					//Min value
	NUM_LOOP_MODES - 1,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraCut =
{
	pt_byte,			//Type
	"Note Cut",			//Short name
	"Note Cut in subdivs",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraVolume =
{
	pt_byte,			//Type
	"Volume",			//Short name
	"Volume (0=0% 80=100% FE=~198%)",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraVelocity =
{
	pt_byte,			//Type
	"Velocity",			//Short name
	"Velocity (0=0% 80=100% FE=~198%)",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraPan =
{
	pt_byte,			//Type
	"Pan",				//Short name
	"Pan (1=100% Left 80=Center FF=100% Right)",	//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraTune =
{
	pt_byte,			//Type
	"Tune",				//Short name
	"Tune (80=normal, 81=+0.25st, 7F=-0.25st)",			//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraVolEnv =
{
	pt_byte,			//Type
	"Vol Env",			//Short name
	"Volume Envelope",	//Long name
	0,					//Min value
	NumCustomEnvelopes,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraVolEnvLen =
{
	pt_byte,			//Type
	"  Len",			//Short name
	"Volume Envelope Length",	//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16					//Default value
};

CMachineParameter const paraPanEnv =
{
	pt_byte,			//Type
	"Pan Env",			//Short name
	"Pan Envelope",	//Long name
	0,					//Min value
	NumCustomEnvelopes,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraPanEnvLen =
{
	pt_byte,			//Type
	"  Len",			//Short name
	"Pan Envelope Length",	//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16					//Default value
};

CMachineParameter const paraTuneEnv =
{
	pt_byte,			//Type
	"Tune Env",			//Short name
	"Tune Envelope",	//Long name
	0,					//Min value
	NumCustomEnvelopes,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraTuneEnvLen =
{
	pt_byte,			//Type
	"  Len",			//Short name
	"Tune Envelope Length",	//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16					//Default value
};

CMachineParameter const paraLoopFit =
{
	pt_word,			//Type
	"Loop Fit",			//Short name
	"Loop Fit (in ticks/subdivs)",	//Long name
	0,					//Min value
	0x2000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLoopFitMode =
{
	pt_byte,			//Type
	"LpFit Mode",		//Short name
	"Loop Fit Mode",	//Long name
	1,					//Min value
	2,					//Max value
	0,					//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraGroup =
{
	pt_byte,			//Type
	"Group",			//Short name
	"Group",			//Long name
	0,					//Min value
	NUM_GROUPS-1,		//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_PLAYS_WAVES | MIF_MONO_TO_STEREO,	//Flags
	0,								//Min tracks
	0,								//Max tracks
	0,								//Number of global parameters
	NUMTRACK,						//Number of track parameters
	NULL,							//Pointer to parameters
	NUMATTR,						//Number of attributes
	pAttributes,					//Pointer to attributes
	"fullname",
	"SGridBETA",					//Short name
	"Ed Powley (BTDSys)",			//Author
	""								//Commands
};

DE(void) GetArraySize(int n, char type, int *params)
{
	*params = NUMTRACK + NUMGLOBAL + n + 2;
}

DE(void) GetInfo(int n, char type, CMachineParameter **pParameters, CMachineInfo *minfo)
{
	*minfo = MacInfo;
	minfo->minTracks = minfo->maxTracks = n;
	minfo->numGlobalParameters = NUMGLOBAL + n + 2;
	minfo->Parameters = (const CMachineParameter **)pParameters;
	char *cc = new char[1024]; cc[0] = 0;
	minfo->Commands = (const char *)cc;
	for (int c=0; c<NUM_COMMANDS; c++)
	{
		strcat(cc,pszCommands[c]);
		strcat(cc,"\n");
	}

	*pParameters[0] = paraFirstWaveNo;
	*pParameters[1] = paraDivider;

	for (int i=2; i<n+2; i++)
	{
		if (type == 's')
			pParameters[i]->Type = pt_switch;
		else
			pParameters[i]->Type = pt_byte;

		sprintf(nameTriggers[i-2], "Trigger %i", i-2);
		sprintf(textTriggers[i-2], "[sg%c]", i-2+33);
		pParameters[i]->Name = nameTriggers[i-2];
		pParameters[i]->Description = textTriggers[i-2];
		pParameters[i]->MinValue = 0;
		pParameters[i]->MaxValue = 0xFE;
		pParameters[i]->NoValue  = 0xFF;
		pParameters[i]->Flags = 0;
		pParameters[i]->DefValue = 0;
	}

	paraTrackTrigger = *pParameters[2];
	paraTrackTrigger.Name = "Trigger";
	paraTrackTrigger.Description = "[sg ]";

	static CMachineParameter const *pt[] = {
		&paraDivider,
		&paraTriggerType,
		&paraSolo,
		&paraVolume,
		&paraVelocity,
		&paraHumanVel,
		&paraPan,
		&paraHumanPan,
		&paraTune,
		&paraHumanTune,
        &paraGlobalLenUnit,
		&paraShuffleSize,
		&paraShuffleStep,
		&paraShuffleRandom,
		&paraShuffleReset,
		&paraInertia,

		//track
		&paraTrackTrigger,
	&paraSeparator,
		&paraWaveNo,
		&paraCommand1,
		&paraArgument1,
		&paraCommand2,
		&paraArgument2,
		&paraLenUnit,
		&paraMute,
		&paraGroup,
	&paraSeparator,
		&paraOffset,
		&paraLoopStart,
		&paraLoopEnd,
		&paraLoopMode,
		&paraCut,
		&paraLoopFit,
//		&paraLoopFitMode,
	&paraSeparator,
		&paraVolume,
		&paraVelocity,
		&paraHumanVel,
		&paraVolEnv,
		&paraVolEnvLen,
	&paraSeparator,
		&paraPan,
		&paraHumanPan,
		&paraPanEnv,
		&paraPanEnvLen,
	&paraSeparator,
		&paraTune,
		&paraHumanTune,
//		&paraTuneEnv,
//		&paraTuneEnvLen,
		NULL
	};

	paraSolo.MaxValue = paraSolo.DefValue = n;

	for (int p=n+2; p < n+2+NUMGLOBAL+NUMTRACK; p++)
	{
		*pParameters[p] = *pt[p-n-2];
	}
}


