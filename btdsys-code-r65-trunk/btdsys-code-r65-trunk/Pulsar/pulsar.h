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

#ifndef __BTDSYS_REL_H
#define __BTDSYS_REL_H

#include "../mdk/mdk.h"
#include "InertiaParam.h"
#include "BandLimit.h"
#include "OscilloscopeDlg.h"

////////////////////////////////////////////////////////////////////////
// Param-related classes

#define MAX_OVERSAMPLE 32
#define MAX_OVERSAMPEXP 5
#define MAX_OVERLAP 32
#define ENVTABLE_LENGTH 128

extern CMachineAttribute const attrInBufSize;
extern CMachineParameter const paraInertia;
extern CMachineParameter const paraFreqSliderMode;
extern CMachineParameter const paraMinDFreq;
extern CMachineParameter const paraMinPFreq;
extern CMachineParameter const paraOversample;
extern CMachineParameter const paraDCRemoval;
extern CMachineParameter const paraOverlapping;
extern CMachineParameter const paraSynch;
extern CMachineParameter const paraFreqP;
extern CMachineParameter const paraPeriodP;
extern CMachineParameter const paraNoteP;
extern CMachineParameter const paraPParam;
extern CMachineParameter const paraFreqD;
extern CMachineParameter const paraOnOff;
extern CMachineParameter const paraBurst;
extern CMachineParameter const paraRest;
extern CMachineParameter const paraStochMask;
extern CMachineParameter const paraRandomPan;
extern CMachineParameter const paraEnvelope;
extern CMachineParameter const paraEnvWTWave;
extern CMachineParameter const paraEnvParam;
extern CMachineParameter const paraEnvNorm;
extern CMachineParameter const paraWave;
extern CMachineParameter const paraWStart;
extern CMachineParameter const paraWEnd;
extern CMachineParameter const paraVolume;
extern CMachineParameter const paraPan;
extern CMachineParameter const paraSeparator;
extern CMachineParameter const paraLFO1Period;
extern CMachineParameter const paraLFO1Shape;
extern CMachineParameter const paraLFO1WTWave;
extern CMachineParameter const paraLFO1Target;
extern CMachineParameter const paraLFO1TargetTrack;
extern CMachineParameter const paraLFO1Amount;
extern CMachineParameter const paraLFO2Period;
extern CMachineParameter const paraLFO2Shape;
extern CMachineParameter const paraLFO2WTWave;
extern CMachineParameter const paraLFO2Target;
extern CMachineParameter const paraLFO2TargetTrack;
extern CMachineParameter const paraLFO2Amount;

//extern CMachineAttribute const attrControlMode;

#pragma pack(1)

class gvals
{
public:
	word Inertia;
	byte FreqSliderMode;
	word MinPFreq;
	word MinDFreq;
	byte Oversample;
	byte DCRemoval;
};

class tvals
{
public:
	byte s0;
	byte Synch;
	byte Overlapping;
	word FreqP;
	byte PeriodP;
	byte NoteP;
	byte PParam;
	word FreqD;
	word Volume;
	byte Pan;
	byte Burst;
	byte Rest;
	word StochMask;
	word RandomPan;
	byte s3;
	byte OnOff;
#	ifndef PULSARFX
		byte Wave;
#	endif
	word WStart;
	word WEnd;
	byte Envelope;
	byte EnvWTWave;
	byte EnvParam;
	byte EnvNorm;
	byte s1;
	word LFO1Period;
	byte LFO1Shape;
	byte LFO1WTWave;
	byte LFO1Target;
	byte LFO1TargetTrack;
	word LFO1Amount;
	byte s2;
	word LFO2Period;
	byte LFO2Shape;
	byte LFO2WTWave;
	byte LFO2Target;
	byte LFO2TargetTrack;
	word LFO2Amount;
};

static const tvals def_tvals = {
	0,
	paraSynch.DefValue,
	paraOverlapping.DefValue,
	paraFreqP.DefValue,
	paraPeriodP.DefValue,
	paraNoteP.DefValue,
	paraPParam.DefValue,
	paraFreqD.DefValue,
	paraVolume.DefValue,
	paraPan.DefValue,
	paraBurst.DefValue,
	paraRest.DefValue,
	paraStochMask.DefValue,
	paraRandomPan.DefValue,
	0,
	paraOnOff.DefValue,
#	ifndef PULSARFX
		paraWave.DefValue,
#	endif
	paraWStart.DefValue,
	paraWEnd.DefValue,
	paraEnvelope.DefValue,
	paraEnvWTWave.DefValue,
	paraEnvParam.DefValue,
	paraEnvNorm.DefValue,
	0,
	paraLFO1Period.DefValue,
	paraLFO1Shape.DefValue,
	paraLFO1WTWave.DefValue,
	paraLFO1Target.DefValue,
	paraLFO1TargetTrack.DefValue,
	paraLFO1Amount.DefValue,
	0,
	paraLFO2Period.DefValue,
	paraLFO2Shape.DefValue,
	paraLFO2WTWave.DefValue,
	paraLFO2Target.DefValue,
	paraLFO2TargetTrack.DefValue,
	paraLFO2Amount.DefValue
};

class avals
{
public:
	int OffOnStop;
	int OnOffOnNote;
	int OversampleQuality;
//	int LowPFreq;
//	int LowDFreq;
	int LFOAmtMidPt;
#	ifdef PULSARFX
		int InBufSize;
#	endif
};

#pragma pack()

enum {
#	ifdef PULSARFX
		NUMA = 5,
#	else
		NUMA = 4,
#	endif
	numInertia = 0,
	numFreqSliderMode,
	numMinPFreq,
	numMinDFreq,
	numOversample,
	numDCRemoval,
	NUMG,

	numTrackSep = NUMG,
	numSynch,
	numOverlapping,
	numFreqP,
	numPeriodP,
	numNoteP,
	numPParam,
	numFreqD,
	numVolume,
	numPan,
	numBurst,
	numRest,
	numStochMask,
	numRandomPan,
	numSeparator3,
	numOnOff,
#	ifndef PULSARFX
		numWave,
#	endif
	numWStart,
	numWEnd,
	numEnvelope,
	numEnvWTWave,
	numEnvParam,
	numEnvNorm,
	numSeparator1,
	numLFO1Period,
	numLFO1Shape,
	numLFO1WTWave,
	numLFO1Target,
	numLFO1TargetTrack,
	numLFO1Amount,
	numSeparator2,
	numLFO2Period,
	numLFO2Shape,
	numLFO2WTWave,
	numLFO2Target,
	numLFO2TargetTrack,
	numLFO2Amount,
	NUMPARA,
	NUMT = NUMPARA - NUMG,

	lfoOff = 0,
	lfoFreqP,
	lfoFreqD,
	lfoWStart,
	lfoWEnd,
	lfoStochMask,
	lfoRandomPan,
	lfoVolume,
	lfoPan,
	lfoEnvParam,
	lfoBurst,
	lfoRest,
	lfoOverlap,
	lfoLFO1Period,
	lfoLFO1Amount,
	NUMLFOTARGETS,
};

////////////////////////////////////////////////////////////////////////
// The main machine interface class

class mi;

class Track
{
	friend class mi;
public:
	Track();
	~Track();

	void Tick();
	bool Work(float *psamples, const float *pinput, int numsamples, int const mode);

	tvals *tval;
	int nTrack;
	mi *pmi;

	void DBG_WriteEnvFn();

	COscilloscopeDlg *pOscDlg;

	void UpdateOscDlg(float fLenP, float fLenD, 
					  float fWStart, float fWEnd, 
#					ifdef PULSARFX
					  float *pSamples, int numSamples,
#					else
					  short *pSamples, int numSamples,
#					endif
					  bool bStereo);


private:
	InertiaParam ipFreqP;	//Hz
	InertiaParam ipPeriodP;	//Ticks
	byte nPParam;
	InertiaParam ipFreqD;	//Hz
	bool bOn;
	bool bHold;
	int nHoldPulsaret;
	byte nWave;
	InertiaParam ipWStart;	//0-1
	InertiaParam ipWEnd;	//0-1
	InertiaParam ipBurst;
	InertiaParam ipRest;
//	byte nBurst;
//	byte nRest;
	InertiaParam ipStochMask;//0-1
	InertiaParam ipRandomPan;//0-1
	InertiaParam ipVolume;	//0-1
	InertiaParam ipPan;		//0-2

	int nBurstCounter;
	bool bStochMask;
	bool bPulsaretPlaying[MAX_OVERLAP];
	float fPulsaretPan[MAX_OVERLAP];
	float fPhase[MAX_OVERLAP];
	float fOverallPhase;
//	int nOverlap;
	InertiaParam ipOverlap;
	int nRecentPulsaret;

	int nEnvShape;
	int nEnvWTWave;
	bool bEnvNorm;
	InertiaParam ipEnvParam; //0-1
	float fEnvParam;
	float pfEnvTable[ENVTABLE_LENGTH+4];
	void RecalcEnvTable();

	LFO lfo1;
	LFO lfo2;
	int nLFO1Target;
	int nLFO1Track;
	int nLFO2Target;
	int nLFO2Track;
	InertiaParam ipLFO1Amount;
	InertiaParam ipLFO1Period;
	InertiaParam ipLFO2Amount;
	InertiaParam ipLFO2Period;

	void ClearLFO1Target();
	void SetLFO1Target();
	void ClearLFO2Target();
	void SetLFO2Target();

	bool bUpdateOscDlg;

	float *pfInBuffer;
	bool bInBufferOverflow;
	float *pfPrBuffer[MAX_OVERLAP];
	int nInBufferHead;
	int nPrBufferLength[MAX_OVERLAP];

	void WorkPulsaret(int j, int nStartSample, float *psamples, const float *pinput, int numsamples, int const mode);
};

class miex : public CMDKMachineInterfaceEx
{
public:
	mi *pmi;

	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);
	virtual char const *DescribeParam(int const param);
};

class mi : public CMDKMachineInterface
{
	friend class Track;
public:
	// standard
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop();
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode) { return false; };
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void Command(const int i);
	virtual void OutputModeChanged(bool stereo) {}
	virtual void SetNumTracks(const int n);
	bool DblClickEvent(void *p);
	virtual void AttributesChanged();
	miex ex;
	gvals gval;
	tvals tval[254];
	avals aval;

	CMachine *thismac;
	int numTracks;

	Track tracks[254];

	COLORREF colVisBG, colVisWave, colVisEnv, colVisEnvWave, colVisOverlap;

	int bShowGraphicalSeparator;

private:
	int FreqToParam(int mode, int oldmode, float f, float linmult, float logc, float oldlinmult, float oldlogc);

	bool bFirstTick;

	int nInertiaLen;
	int nOversample, nOversampExp;
	bool bRemoveDC;
	float dcin, dcout;

	CHalfBandFilter hbFilter[MAX_OVERSAMPEXP];

	void HalfBuffer(float *buf, int nLen, int nFilter);

	int nFreqSliderMode;

	float fLogPFreqC;
	float fLogDFreqC;
	float fLinPFreqMult;
	float fLinDFreqMult;

	float fLFOAmtExp;
	bool bLFOAmtExpInvert;

	void CustomiseColour(COLORREF &c, char *regname);

	int nMaxWorkBlockSize;
	float fMinLFOPeriod;

};











#endif
