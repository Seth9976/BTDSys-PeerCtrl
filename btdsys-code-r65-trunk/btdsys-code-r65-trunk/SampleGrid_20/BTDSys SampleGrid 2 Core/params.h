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

// SampleGrid
// Parameter definitions

#ifndef BTDSYS_SAMPLEGRID_PARAMS
#define BTDSYS_SAMPLEGRID_PARAMS

#include "../../mdk/mdk.h"
#include "sgrid.h"

extern CMachineAttribute const attrRampLen;
extern CMachineAttribute const attrRampVolume;
extern CMachineAttribute const attrHumanMode;
extern CMachineAttribute const attrInertiaVolume;
extern CMachineAttribute const attrInertiaPan;
extern CMachineAttribute const attrInertiaTune;
extern CMachineAttribute const attrChannels;

////////////////////////////////////////////////////////////////////////

extern CMachineAttribute const *pAttributes[];

#pragma pack(1)

class avals
{
public:
	int RampLen;
	int RampVolume;
	int HumanMode;
	int InertiaVolume;
	int InertiaPan;
	int InertiaTune;
	int Channels;
    int MIDIRecord;
    int MIDIRecordOffs;
    int MIDIRecordDelays;
};

#pragma pack()

#define NUMATTR	10



#define NUM_OUTPUTS 8
#define NUM_GROUPS 16

//GLOBAL//////////////////////////////////////////////////
extern CMachineParameter paraSolo, paraTrackTrigger; 

extern CMachineParameter const 
	paraSeparator,
	paraFirstWaveNo,
	paraDivider, 
	paraTriggerType, 
    paraGlobalLenUnit,
	paraShuffleSize,
	paraShuffleStep,
	paraShuffleRandom,
	paraShuffleReset,
	paraInertia,

//TRACK//////////////////////////////////////////////////
	paraWaveNo,
	paraOffset,
	paraLoopStart,
	paraLoopEnd,
	paraLoopMode,
	paraCut,
	paraCommand1,
	paraArgument1,
	paraCommand2,
	paraArgument2,
	paraLenUnit,
	paraMute,
	paraVelocity,
	paraVolume,
	paraPan,
	paraTune,
	paraLoopFit,
	paraLoopFitMode,
	paraHumanVel,
	paraHumanPan,
	paraHumanTune,
	paraVolEnv,
	paraVolEnvLen,
	paraPanEnv,
	paraPanEnvLen,
	paraTuneEnv,
	paraTuneEnvLen,
	paraGroup;

enum
{
	numDivider2 = 0,
	numTriggerType,
	numSolo,
	numGlobalVolume,
	numGlobalVelocity,
	numGlobalHumanVel,
	numGlobalPan,
	numGlobalHumanPan,
	numGlobalTune,
	numGlobalHumanTune,
    numGlobalLenUnit,
	numShuffleSize,
	numShuffleStep,
	numShuffleRandom,
	numShuffleReset,
	numInertia,
	NUMGLOBAL,

//	numSeparator = NUMGLOBAL,
	numTrackTrigger = NUMGLOBAL,
numSeparator01,
	numWaveNo,
	numCommand1,
	numArgument1,
	numCommand2,
	numArgument2,
	numLenUnit,
	numMute,
	numGroup,
numSeparator02,
	numOffset,
	numLoopStart,
	numLoopEnd,
	numLoopMode,
	numCut,
	numLoopFit,
//	numLoopFitMode,
numSeparator03,
	numVolume,
	numVelocity,
	numHumanVel,
	numVolEnv,
	numVolEnvLen,
numSeparator04,
	numPan,
	numHumanPan,
	numPanEnv,
	numPanEnvLen,
numSeparator05,
	numTune,
	numHumanTune,
	NUMPARAM,
	numTuneEnv,
	numTuneEnvLen,
	NUMTRACK = NUMPARAM - NUMGLOBAL,
};

//extern CMachineParameter const *pParameters[NUMGLOBAL + NUMTRACK + 1];

#pragma pack(1)

class gvals
{
public:
	byte Triggers[MAXTRIGGERS + 2]; //right justified
	byte Divider2;
	byte TriggerType;
	byte Solo;
	byte GlobalVolume;
	byte GlobalVelocity;
	byte GlobalHumanVel;
	byte GlobalPan;
	byte GlobalHumanPan;
	byte GlobalTune;
	byte GlobalHumanTune;
    byte GlobalLenUnit;
	byte ShuffleSize;
	byte ShuffleStep;
	byte ShuffleRandom;
	byte ShuffleReset;
	word Inertia;
};

class tvals
{
public:
	byte TrackTrigger;
byte Separator01;
	byte WaveNo;
	byte Command1;
	byte Argument1;
	byte Command2;
	byte Argument2;
	byte LenUnit;
	byte Mute;
	byte Group;
byte Separator02;
	word Offset;
	word LoopStart;
	word LoopEnd;
	byte LoopMode;
	byte Cut;
	word LoopFit;
//	byte LoopFitMode;
byte Separator03;
	byte Volume;
	byte Velocity;
	byte HumanVel;
	byte VolEnv;
	byte VolEnvLen;
byte Separator04;
	byte Pan;
	byte HumanPan;
	byte PanEnv;
	byte PanEnvLen;
byte Separator05;
	byte Tune;
	byte HumanTune;
//	byte TuneEnv;
//	byte TuneEnvLen;
};

#define TuneEnv Separator01
#define TuneEnvLen Separator02

#pragma pack()

#endif
