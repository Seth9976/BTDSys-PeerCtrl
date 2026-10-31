/*
Copyright (c) 2004-2006, Edward Powley
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
#include "pulsar.h"
#include "iwaves.h"

CMachineParameter const paraInertia =
{
	pt_word,			//Type
	"Inertia",			//Short name
	"Inertia in tick/16",		//Long name
	0,					//Min value
	256*16,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraOversample =
{
	pt_byte,			//Type
	"Oversample",		//Short name
	"Oversample 2^n",	//Long name
	0,					//Min value
	MAX_OVERSAMPEXP,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraDCRemoval =
{
	pt_switch,			//Type
	"DC removal",		//Short name
	"DC removal",		//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraFreqSliderMode =
{
	pt_byte,			//Type
	"Freq Mode",		//Short name
	"Frequency Slider Mode",		//Long name
	0,					//Min value
	1,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraMinPFreq =
{
	pt_word,			//Type
	"Min P Freq",		//Short name
	"Minimum P frequency",		//Long name
	1,					//Min value
	5000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	250					//Default value
};

CMachineParameter const paraMinDFreq =
{
	pt_word,			//Type
	"Min D Freq",		//Short name
	"Minimum D frequency",		//Long name
	1,					//Min value
	5000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	250					//Default value
};

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

CMachineParameter const paraOverlapping =
{
	pt_byte,			//Type
	"Overlap",			//Short name
	"Overlapping",		//Long name
	1,					//Min value
	MAX_OVERLAP - 1,	//Max value
	0,					//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraFreqP =
{
	pt_word,			//Type
	"P freq",			//Short name
	"P frequency in Hz/4",		//Long name
	1,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	440*4				//Default value
};

CMachineParameter const paraPeriodP =
{
	pt_byte,			//Type
	"P period",			//Short name
	"P period in tick/16",		//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16					//Default value
};

CMachineParameter const paraNoteP =
{
	pt_note,			//Type
	"P note",			//Short name
	"P note",			//Long name
	NOTE_MIN,			//Min value
	NOTE_MAX,			//Max value
	NOTE_NO,			//No value
	0,					//Flags
	NOTE_NO				//Default value
};

CMachineParameter const paraPParam =
{
	pt_byte,			//Type
	"P param",			//Short name
	"P param",			//Long name
	0,					//Min value
	3,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraFreqD =
{
	pt_word,			//Type
	"D freq",			//Short name
	"D frequency in Hz/4",		//Long name
	1,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	1000*4				//Default value
};

CMachineParameter const paraSynch =
{
	pt_switch,			//Type
	"Synch",			//Short name
	"1 = Synchronise",	//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	0,					//Flags
	SWITCH_NO			//Default value
};

CMachineParameter const paraOnOff =
{
	pt_switch,			//Type
#	ifdef PULSARFX
		"Freeze",			//Short name
		"Freeze",			//Long name
#	else
		"On/Off",			//Short name
		"On/Off",			//Long name
#	endif
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	SWITCH_OFF			//Default value
};

#ifndef PULSARFX
	CMachineParameter const paraWave =
	{
		pt_byte,			//Type
		"Wave",				//Short name
		"Wave for pulsaret",		//Long name
		WAVE_MIN,			//Min value
		WAVE_MAX + 6 + NUM_INTERNAL_WAVES,				//Max value
		WAVE_NO,			//No value
		MPF_STATE | MPF_WAVE,	//Flags
		201					//Default value
	};
#endif

CMachineParameter const paraWStart =
{
	pt_word,			//Type
	"Start",			//Short name
	"Start posn in wave",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraWEnd =
{
	pt_word,			//Type
	"End",				//Short name
	"End posn in wave",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0xFFFE				//Default value
};

CMachineParameter const paraEnvelope =
{
	pt_byte,			//Type
	"Envelope",			//Short name
	"Pulsaret envelope shape",		//Long name
	0,					//Min value
	6,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraEnvWTWave =
{
	pt_byte,			//Type
	"Env Wave",			//Short name
	"Pulsaret envelope wavetable wave (if applicable)",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_STATE | MPF_WAVE,			//Flags
	1					//Default value
};

CMachineParameter const paraEnvParam =
{
	pt_byte,			//Type
	"Env Param",		//Short name
	"Pulsaret envelope parameter",		//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x40				//Default value
};

CMachineParameter const paraEnvNorm =
{
	pt_switch,			//Type
	"Env Norm",			//Short name
	"Normalise pulsaret envelope",		//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	SWITCH_OFF			//Default value
};

CMachineParameter const paraBurst =
{
	pt_byte,			//Type
	"Burst",			//Short name
	"Burst length",		//Long name
	1,					//Min value
	128,				//Max value
	255,				//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraRest =
{
	pt_byte,			//Type
	"Rest",				//Short name
	"Rest length",		//Long name
	0,					//Min value
	128,				//Max value
	255,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraStochMask =
{
	pt_word,			//Type
	"Stoch Mask",		//Short name
	"Stochastic Masking",	//Long name
	0,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

CMachineParameter const paraRandomPan =
{
	pt_word,			//Type
	"Random Pan",		//Short name
	"Random stereo deviation",	//Long name
	0,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraVolume =
{
	pt_word,			//Type
	"Volume",			//Short name
	"Volume (8000=100%)",			//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

CMachineParameter const paraPan =
{
	pt_byte,			//Type
	"Pan",				//Short name
	"Pan",				//Long name
	1,					//Min value
	0xFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraLFO1Period =
{
	pt_word,			//Type
	"LFO1 Period",		//Short name
	"LFO1 Period in tick/16",		//Long name
	1,					//Min value
	256 * 16,			//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16 * 16				//Default value
};

CMachineParameter const paraLFO1Shape =
{
	pt_byte,			//Type
	"LFO1 Shape",		//Short name
	"LFO1 Shape",		//Long name
	0,					//Min value
	8,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO1WTWave =
{
	pt_byte,			//Type
	"LFO1 Wave",		//Short name
	"LFO1 Wavetable Wave (if applicable)",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_STATE | MPF_WAVE,			//Flags
	1					//Default value
};

CMachineParameter const paraLFO1Target =
{
	pt_byte,			//Type
	"LFO1 Target",		//Short name
	"LFO1 Target",		//Long name
	0,					//Min value
	NUMLFOTARGETS-3,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO1TargetTrack =
{
	pt_byte,			//Type
	"LFO1 Track",		//Short name
	"LFO1 Target Track",	//Long name
	0,					//Min value
	254,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO1Amount =
{
	pt_word,			//Type
	"LFO1 Amount",		//Short name
	"LFO1 Amount",		//Long name
	0,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO2Period =
{
	pt_word,			//Type
	"LFO2 Period",		//Short name
	"LFO2 Period in tick/16",		//Long name
	1,					//Min value
	256 * 16,			//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16 * 16				//Default value
};

CMachineParameter const paraLFO2Shape =
{
	pt_byte,			//Type
	"LFO2 Shape",		//Short name
	"LFO2 Shape",		//Long name
	0,					//Min value
	8,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO2WTWave =
{
	pt_byte,			//Type
	"LFO2 Wave",		//Short name
	"LFO2 Wavetable Wave (if applicable)",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_STATE | MPF_WAVE,			//Flags
	1					//Default value
};

CMachineParameter const paraLFO2Target =
{
	pt_byte,			//Type
	"LFO2 Target",		//Short name
	"LFO2 Target",		//Long name
	0,					//Min value
	NUMLFOTARGETS-1,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO2TargetTrack =
{
	pt_byte,			//Type
	"LFO2 Track",		//Short name
	"LFO2 Target Track",	//Long name
	0,					//Min value
	254,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLFO2Amount =
{
	pt_word,			//Type
	"LFO2 Amount",		//Short name
	"LFO2 Amount",		//Long name
	0,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineAttribute const attrInBufSize =
{
	"Input buffer size",//name
	1000,				//min
	44100*60,			//max
	44100*8				//def
};

CMachineAttribute const attrOffOnStop =
{
	"Off on stop",		//name
	0,					//min
	1,					//max
	1					//def
};

#ifndef PULSARFX
	CMachineAttribute const attrOnOffOnNote =
	{
		"Note controlled on/off",		//name
		0,					//min
		1,					//max
		1					//def
	};
#else
	CMachineAttribute const attrOnOffOnNote =
	{
		"Note controlled freeze",		//name
		0,					//min
		1,					//max
		0					//def
	};
#endif

CMachineAttribute const attrLowPFreq =
{
	"Low P freq for log mode, in Hz/1000",		//name
	1,					//min
	20000,				//max
	500					//def
};

CMachineAttribute const attrLowDFreq =
{
	"Low D freq for log mode, in Hz/1000",		//name
	1,					//min
	20000,				//max
	500					//def
};

CMachineAttribute const attrLFOAmtMidPt =
{
	"LFO amount midpoint * 1000",		//name
	1,					//min
	999,				//max
	300					//def
};

CMachineAttribute const attrOversampleQuality =
{
	"Oversample quality (1=low 3=high)",		//name
	1,					//min
	3,					//max
	2					//def
};

CMachineParameter const *pParameters[] = {
	&paraInertia,
	&paraFreqSliderMode,
	&paraMinPFreq,
	&paraMinDFreq,
	&paraOversample,
	&paraDCRemoval,

	&paraSeparator,
	&paraSynch,
	&paraOverlapping,
	&paraFreqP,
	&paraPeriodP,
	&paraNoteP,
	&paraPParam,
	&paraFreqD,	
	&paraVolume,
	&paraPan,
	&paraBurst,
	&paraRest,	
	&paraStochMask,
	&paraRandomPan,
	&paraSeparator,
	&paraOnOff,	
#	ifndef PULSARFX
		&paraWave,
#	endif
	&paraWStart,
	&paraWEnd,
	&paraEnvelope,
	&paraEnvWTWave,
	&paraEnvParam,
	&paraEnvNorm,
	&paraSeparator,
	&paraLFO1Period,
	&paraLFO1Shape,
	&paraLFO1WTWave,
	&paraLFO1Target,
	&paraLFO1TargetTrack,
	&paraLFO1Amount,
	&paraSeparator,
	&paraLFO2Period,
	&paraLFO2Shape,
	&paraLFO2WTWave,
	&paraLFO2Target,
	&paraLFO2TargetTrack,
	&paraLFO2Amount,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	&attrOffOnStop,
	&attrOnOffOnNote,
	&attrOversampleQuality,
//	&attrLowPFreq,
//	&attrLowDFreq,
	&attrLFOAmtMidPt,
#	ifdef PULSARFX
		&attrInBufSize,
#	endif
	NULL
};

CMachineInfo const MacInfo =
{
#	ifdef PULSARFX
		MT_EFFECT,						//Machine type
		MI_VERSION,						//Interface version
		MIF_MONO_TO_STEREO | MIF_DOES_INPUT_MIXING,				//Flags
#	else
		MT_GENERATOR,					//Machine type
		MI_VERSION,						//Interface version
		MIF_MONO_TO_STEREO,				//Flags
#	endif
	1,								//Min tracks
	254,							//Max tracks
	NUMG,							//Number of global parameters
	NUMT,							//Number of track parameters
	pParameters,					//Pointer to parameters
	NUMA,							//Number of attributes
	pAttributes,					//Pointer to attributes
#	ifdef PULSARFX
		"BTDSys PulsarFX",				//Full name
		"PulsarFX",						//Short name
#	else
		"BTDSys Pulsar",				//Full name
		"Pulsar",						//Short name
#	endif
	"Ed Powley (BTDSys)",			//Author
	"About...\n"
	"/Visualisation\n"
	"/Customise Visualisation Colours"	//Commands
};

DLL_EXPORTS
