/*
Copyright (c) 2002-2011, Edward Powley
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

#ifndef __BTDSYS_PEERADSR_PARAMS
#define __BTDSYS_PEERADSR_PARAMS

#define MAX_TRACKS 255

CMachineParameter const paraTrigger =
{
	pt_switch,			//Type
	"Trigger",			//Short name
	"Trigger (1=on 0=off)",		//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraEnvType =
{
	pt_byte,			//Type
	"Env Type",			//Short name
	"Envelope Type (0=ADSR 1=Custom 2=Wavetable)",		//Long name
	0,					//Min value
	2,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraEnvNum =
{
	pt_byte,			//Type
	"Env Num",			//Short name
	"Envelope Number (for custom/wavetable)",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraLength =
{
	pt_word,			//Type
	"Length",			//Short name
	"Length of custom/wavetable envelope (FFFE=Length Max attribute value)",		//Long name
	1,					//Min value
	0xFFFE,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x1000				//Default value
};

CMachineParameter const paraMin = 
{
	pt_word,			//Type
	"Min",				//Short name
	"Min value (0=0% 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraMax = 
{
	pt_word,			//Type
	"Max",				//Short name
	"Max value (0=0% 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0xFFFE				//Default value
};

CMachineParameter const paraAttack =
{
	pt_word,			//Type
	"Attack",			//Short name
	"Attack (FFFE=Length Max attribute value)",		//Long name
	1,					//Min value
	0xFFFE,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x1000				//Default value
};

CMachineParameter const paraDecay =
{
	pt_word,			//Type
	"Decay",			//Short name
	"Decay (FFFE=Length Max attribute value)",		//Long name
	1,					//Min value
	0xFFFE,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x1000				//Default value
};

CMachineParameter const paraSustainLevel =
{
	pt_word,			//Type
	"Sus Level",			//Short name
	"Sustain Level (0=0% 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

CMachineParameter const paraSustainLength =
{
	pt_word,			//Type
	"Sus Length",			//Short name
	"Sustain Length (FFFE=Length Max attribute value, FFFF=infinite)",		//Long name
	1,					//Min value
	0xFFFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x1000				//Default value
};

CMachineParameter const paraRelease =
{
	pt_word,			//Type
	"Release",			//Short name
	"Release (FFFE=Length Max attribute value)",		//Long name
	1,					//Min value
	0xFFFE,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x1000				//Default value
};

CMachineParameter const paraTrack =
{
	pt_byte,			//Type
	"Track",			//Short name
	"Track to send parameter to (0xFE=all)",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineAttribute const attrLengthMax =
{
	"Length Max (in ticks)",	//name
	1,		//min
	4096,		//max
	16		//def
};

CMachineAttribute const attrSubtickCtrl =
{
	"Subtick Control (1=on 0=off)",	//name
	0 ,		//min
	1,		//max
	1		//def
};

CMachineAttribute const attrMIDIChannel =
{
	"MIDI Channel (0=off, 17=all)",	//name
	0,		//min
	17,		//max
	0		//def
};

CMachineAttribute const attrMIDITrack =
{
	"MIDI Track",	//name
	0,				//min
	MAX_TRACKS-1,	//max
	0				//def
};

CMachineAttribute const attrMIDIWantNoteOffs =
{
	"MIDI want note offs (1=y 0=n)",	//name
	0,		//min
	1,		//max
	1		//def
};


enum
{
	NUMGLOBAL = 0,
	
	numTrigger = NUMGLOBAL,
	numEnvType,
	numEnvNum,
	numLength,
	numMin,
	numMax,
	numAttack,
	numDecay,
	numSustainLength,
	numSustainLevel,
	numRelease,
	numTrack,
	NUMTRACK,

	NUMATTRIBUTES = 5
};

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class gvals
{
};

class tvals
{
public:
	byte Trigger;
	byte EnvType;
	byte EnvNum;
	word Length;
	word Min;
	word Max;
	word Attack;
	word Decay;
	word SustainLength;
	word SustainLevel;
	word Release;
	byte Track;
};

class avals
{
public:
	int LengthMax;
	int SubtickCtrl;
	int MIDIChannel;
	int MIDITrack;
	int MIDIWantNoteOffs;
};

#pragma pack()

#endif