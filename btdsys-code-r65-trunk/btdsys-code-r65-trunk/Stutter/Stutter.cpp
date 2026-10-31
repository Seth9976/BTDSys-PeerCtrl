/*
Stutter.cpp: the one and only source file for this machine
Copyright (c) 2002-2004, Edward Powley
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

/*
Created on 02-Jun-02
Released:
  Buzzmachines.com messageboard (alpha) 04-Jun-02
  Buzzmachines.com messageboard (alpha) 05-Jun-02
  Buzzmachines.com messageboard (alpha) 06-Jun-02
  v1.0 (Buzzmachines.com) - 13-Jun-02
*/

#include "../mdk/mdk.h"
#include <windows.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

//#define ALPHA_TEST_VERSION

#define MAX_TRACKS	8


////////////////////////////////////////////////////////////////////////

CMachineParameter const paraWaveNo =
{
	pt_byte,			//Type
	"Wave Number",		//Short name
	"Wave Number",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_WAVE | MPF_STATE,	//Flags
	1					//Default value
};

CMachineParameter const paraTrigger =
{
	pt_switch,			//Type
	"Trigger",			//Short name
	"Trigger (0=off, 1=on)",		//Long name
	SWITCH_OFF,			//Min value
	SWITCH_ON,			//Max value
	SWITCH_NO,			//No value
	0,					//Flags
	SWITCH_OFF			//Default value
};

CMachineParameter const paraStart =
{
	pt_word,			//Type
	"Loop Start",		//Short name
	"Loop Start (0=0%, FFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraEnd =
{
	pt_word,			//Type
	"End/Length",	//Short name
	"Loop End (0=0%, FFFE=100%) or Loop Length (0=-100%, 8000=0%, FFFE=+100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0xFFFE				//Default value
};

CMachineParameter const paraInertiaA =
{
	pt_word,			//Type
	"Inertia A",			//Short name
	"Inertia A (Loop Start/End) (0=off, 1=0.1 ticks, 500=128.0 ticks)",		//Long name
	0,					//Min value
	0x500,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLoopDir =
{
	pt_byte,			//Type
	"Loop Dir",	//Short name
	"Loop Direction (0=forward, 1=reverse, 2=pingpong, 3=random)",	//Long name
	0,					//Min value
	3,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraLoopTime =
{
	pt_word,			//Type
	"Loop Time",		//Short name
	"Loop Time (0 = 100% rate, >0 = time in tick/256)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraRate =
{
	pt_word,			//Type
	"Rate",				//Short name
	"Playback Rate (0 = 25%, 4000=50%, 8000=100%, C000=200%, FFFE=400%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

CMachineParameter const paraRatePar =
{
	pt_byte,			//Type
	"Rate Param",		//Short name
	"Rate Determination Parameter (0=Loop Time, 1=Rate)",		//Long name
	0,					//Min value
	1,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraVolume =
{
	pt_byte,			//Type
	"Volume",			//Short name
	"Volume (0=0%, 80=100%, FE=~198%)",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

CMachineParameter const paraXFade =
{
	pt_word,			//Type
	"XFade",			//Short name
	"Crossfade (0=off, 1-100=no of samples, 101=1ms, 102=2ms... 4E8=1000ms)",		//Long name
	0,					//Min value
	0x4E8,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraInertiaB =
{
	pt_word,			//Type
	"Inertia B",			//Short name
	"Inertia B (Loop Time, Rate, Volume) (0=off, 1=0.1 ticks, 500=128.0 ticks)",		//Long name
	0,					//Min value
	0x500,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineAttribute const attrEndLength =
{
	"Mode (0=End, 1=Length)",		//Name
	0,						//Min
	1,						//Max
	0						//Default
};

CMachineAttribute const attrLoopLenMode =
{
	"Time Rel To (0=Loop, 1=All)",
	0,
	1,
	0
};

////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraWaveNo,
	&paraTrigger,
	&paraStart,
	&paraEnd,
	&paraInertiaA,
	&paraLoopDir,
	&paraLoopTime,
	&paraRate,
	&paraRatePar,
	&paraVolume,
	&paraXFade,
	&paraInertiaB
};

CMachineAttribute const *pAttributes[] = {
	&attrEndLength,
	&attrLoopLenMode
};

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class tvals
{
public:
	byte WaveNo;
	byte Trigger;
	word Start;
	word End;
	word InertiaA;
	byte LoopDir;
	word LoopTime;
	word Rate;
	byte RatePar;
	byte Volume;
	word XFade;
	word InertiaB;
};

class avals
{
public:
	int EndLength;
	int LoopLenMode;
};

#pragma pack()

////////////////////////////////////////////////////////////////////////

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_PLAYS_WAVES | MIF_MONO_TO_STEREO,	//Flags
	1,								//Min tracks
	MAX_TRACKS,						//Max tracks
	0,								//Number of global parameters
	12,								//Number of track parameters
	pParameters,					//Pointer to parameters
	2,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys Stutter",				//Full name
	"Stutter",						//Short name
	"Edward Powley (BTDSys)",		//Author
	"About..."						//Commands
};

////////////////////////////////////////////////////////////////////////

class mi;

#pragma pack(8)

//LoopDir1 values
#define LOOPDIR_FWD			1
#define LOOPDIR_REV			2

//LoopDir2 values
#define LOOPDIR_STRAIGHT	1
#define LOOPDIR_PINGPONG	2
#define LOOPDIR_RANDOM		3

//RateMethod values
#define RATEMTH_TIME		1
#define RATEMTH_RATE		2

class CTrack
{
public:
	void Reset();
	void ChangePosStep();
	void Tick(tvals const &tv);
	void Stop();
//	void AboutBox();
	void EndOfLoopReached(int const StartSamp, int const EndSamp);
	void UpdateInertia();
	inline bool Generate(float *psamples, int numsamples);
public:
	mi *pmi;
public:
	byte WaveNo;
	bool Playing, At100;
	float LoopPt1, LoopPt2, LoopStart, LoopEnd, CurPos, PosStep, Volume;
	byte LoopDir1, LoopDir2;
	float LoopTime, Rate;
	byte RateMethod;
	int PosStepUpdateCounter;
	word EndSliderPos;
	
	bool DoRamping, IsRamping;
	float RampStep, RampVol, RampPos;
	byte RampDir;

	bool InertiaAOn, InertiaBOn;
	float InertiaALength, InertiaBLength;

	float IStartStep,	IStartTarget;	bool IStartGliding;
	float IEndStep,		IEndTarget;		bool IEndGliding;
	float ITimeStep,	ITimeTarget;	bool ITimeGliding;
	float IRateStep,	IRateTarget;	bool IRateGliding;
	float IVolStep,		IVolTarget;		bool IVolGliding;

	int InertiaUpdateCounter;
};

#define PosStepUpdateCounterLimit 10
#define InertiaUpdateCounterLimit 100

#pragma pack()

////////////////////////////////////////////////////////////////////////

class mi : public CMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop();
	virtual void Init(CMachineDataInput * const pi);
	virtual bool WorkMonoToStereo(float *pin, float *pout, int numsamples, int const mode);
	virtual void Command(int const i);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void SetNumTracks(int const n) { numTracks = n; }
	virtual void AttributesChanged();

public:
	int numTracks;
	CTrack Tracks [MAX_TRACKS];

	tvals tval [MAX_TRACKS];
	avals aval;
};

DLL_EXPORTS


////////////////////////////////////////////////////////////////////////

// returns random value between 0 and 1
// from Waveout.cpp
inline double frand()
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (double)stat * (1.0 / 0x7fffffff);
}

////////////////////////////////////////////////////////////////////////

mi::mi() { 
	TrackVals = &tval;
	AttrVals = (int*) &aval;

	numTracks = MAX_TRACKS;

}

mi::~mi() {}

////////////////////////////////////////////////////////////////////////

void CTrack::Reset()
//Reset a track (initialise all values)
{
	WaveNo			= 1;
	Playing			= false;
	At100			= true;
	LoopPt1			= 0.0f;
	LoopPt2			= 1.0f;
	LoopStart		= 0.0f; 
	LoopEnd			= 1.0f; 
	CurPos			= 0.0f; 
	PosStep			= 1.0f;
	LoopDir1		= LOOPDIR_FWD; 
	LoopDir2		= LOOPDIR_STRAIGHT;
	LoopTime		= 0.0f;
	Rate			= 1.0f;
	RateMethod		= RATEMTH_TIME;
	Volume			= 1.0f;
	EndSliderPos	= 0xFFFE;

	DoRamping	= true;
	IsRamping	= false;
	RampStep	= 0.1f;
	RampVol		= 1.0f;
	RampPos		= 0.0f;
	RampDir		= LOOPDIR_FWD;

	InertiaAOn = InertiaBOn = false;
	InertiaALength = InertiaBLength = 1.0f;

	IStartGliding	= false;
	IEndGliding		= false;
	ITimeGliding	= false;
	IRateGliding	= false;
	IVolGliding		= false;

	PosStepUpdateCounter = 0;
	InertiaUpdateCounter = 0;
}

////////////////////////////////////////////////////////////////////////

void mi::Init (CMachineDataInput * const pi)
{
#ifdef ALPHA_TEST_VERSION
	Command(0); //put up an annoying message box
#endif
	for (int t=0; t < MAX_TRACKS; t++)
	{
		Tracks[t].Reset();
		Tracks[t].pmi = this;
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::ChangePosStep()
//Called if PosStep needs changing (ie the playback rate changed)
{
	if (LoopPt1 < LoopPt2)
	{
		LoopStart	= LoopPt1;
		LoopEnd		= LoopPt2;
	}
	else
	{
		LoopStart	= LoopPt2;
		LoopEnd		= LoopPt1;
	}

	if (pmi->pCB->GetWaveLevel(WaveNo,0) != NULL) //Sample exists
	{
		if (RateMethod == RATEMTH_TIME)
		{
			if (At100)
			{
				PosStep = float(pmi->pCB->GetWaveLevel(WaveNo,0)->SamplesPerSec) 
						/ float(pmi->pMasterInfo->SamplesPerSec);
			}
			else
			{
				float l;
				if (pmi->aval.LoopLenMode == 0)
					l = (LoopEnd - LoopStart) 
						* pmi->pCB->GetWaveLevel(WaveNo,0)->numSamples;
				else
					l = float(pmi->pCB->GetWaveLevel(WaveNo,0)->numSamples);

				float t = LoopTime * pmi->pMasterInfo->SamplesPerTick;

				PosStep = l/t;
			}
		}
		else
		{
			PosStep = float(pmi->pCB->GetWaveLevel(WaveNo,0)->SamplesPerSec) 
					/ float(pmi->pMasterInfo->SamplesPerSec)
					* Rate;
		}
	}

	else PosStep = 1.0f;

	if (LoopPt1 > LoopPt2) PosStep = -PosStep;

	PosStepUpdateCounter = 0; //No need to update for a while
}

////////////////////////////////////////////////////////////////////////

void CTrack::Tick(const tvals &tv)
{
	bool ShouldChangePosStep = false;

	if (tv.InertiaA != paraInertiaA.NoValue)
	{
		if (tv.InertiaA == 0) InertiaAOn = false;
		else
		{
			InertiaAOn = true;
			InertiaALength = float(tv.InertiaA) / 10.0f;
		}
	}

	if (tv.InertiaB != paraInertiaB.NoValue)
	{
		if (tv.InertiaB == 0) InertiaBOn = false;
		else
		{
			InertiaBOn = true;
			InertiaBLength = float(tv.InertiaB) / 10.0f;
		}
	}

	if (tv.WaveNo != paraWaveNo.NoValue)
		WaveNo = tv.WaveNo;

	if (tv.Trigger != paraTrigger.NoValue)
	{
		if (tv.Trigger == SWITCH_ON)
		{
			Playing = true;
			CurPos = LoopStart;
		}
		else Playing = false;
	}

	if (tv.Start != paraStart.NoValue)
	{
		if (InertiaAOn)
		{
			IStartGliding = true;
			IStartTarget = tv.Start / 65536.0f;
			IStartStep = (IStartTarget - LoopPt1)
				/ (pmi->pMasterInfo->SamplesPerTick * InertiaALength);

			if (pmi->aval.EndLength == 1)
			{
				IEndGliding = true;
				IEndTarget = IStartTarget + (EndSliderPos - 32768) / 32768.0f;
//				IEndTarget = IStartTarget + (LoopPt2 - LoopPt1);
				if (IEndTarget > 1.0f) IEndTarget = 1.0f;
				if (IEndTarget < 0.0f) IEndTarget = 0.0f;
				IEndStep = (IEndTarget - LoopPt2)
					/ (pmi->pMasterInfo->SamplesPerTick * InertiaALength);
			}
		}
		else
		{
			LoopPt1 = tv.Start / 65536.0f;

			if (pmi->aval.EndLength == 1)
			{
				LoopPt2 = LoopPt1 + (EndSliderPos - 32768) / 32768.0f;
				if (LoopPt2 > 1.0f) LoopPt2 = 1.0f;
				if (LoopPt2 < 0.0f) LoopPt2 = 0.0f;
			}

			ShouldChangePosStep = true;
		}
	}
	if (tv.End != paraEnd.NoValue)
	{
		EndSliderPos = tv.End;

		if (InertiaAOn)
		{
			IEndGliding = true;
			if (pmi->aval.EndLength == 0)
				IEndTarget = tv.End / 65536.0f;
			else
			{
				if (IStartGliding)
					IEndTarget = IStartTarget + (tv.End - 32768) / 32768.0f;
				else
					IEndTarget = LoopPt1 + (tv.End - 32768) / 32768.0f;

				if (IEndTarget > 1.0f) IEndTarget = 1.0f;
				if (IEndTarget < 0.0f) IEndTarget = 0.0f;
			}

			IEndStep = (IEndTarget - LoopPt2)
				/ (pmi->pMasterInfo->SamplesPerTick * InertiaALength);
		}
		else
		{
			if (pmi->aval.EndLength == 0)
				LoopPt2 = tv.End / 65536.0f;
			else
			{
				if (IStartGliding)
					LoopPt2 = IStartTarget + (tv.End - 32768) / 32768.0f;
				else
					LoopPt2 = LoopPt1 + (tv.End - 32768) / 32768.0f;

				if (LoopPt2 > 1.0f) LoopPt2 = 1.0f;
				if (LoopPt2 < 0.0f) LoopPt2 = 0.0f;
			}

			ShouldChangePosStep = true;
		}
	}

	if (tv.LoopDir != paraLoopDir.NoValue)
	{
		switch(tv.LoopDir){
		case 0:
			LoopDir1 = LOOPDIR_FWD;
			LoopDir2 = LOOPDIR_STRAIGHT;
			break;
		case 1:
			LoopDir1 = LOOPDIR_REV;
			LoopDir2 = LOOPDIR_STRAIGHT;
			break;
		case 2:
			LoopDir1 = LOOPDIR_FWD;
			LoopDir2 = LOOPDIR_PINGPONG;
			break;
		case 3:
			LoopDir1 = LOOPDIR_FWD;
			LoopDir2 = LOOPDIR_RANDOM;
			break;
		}
		ShouldChangePosStep = true;
	}
	
	if (tv.LoopTime != paraLoopTime.NoValue)
	{
		if (tv.LoopTime == 0) At100 = true;
		else At100 = false;

		if (InertiaBOn)
		{
			ITimeGliding = true;
			ITimeTarget = tv.LoopTime / 256.0f;
			ITimeStep = (ITimeTarget - LoopTime)
				/ (pmi->pMasterInfo->SamplesPerTick * InertiaBLength);
		}
		else
		{
			LoopTime = tv.LoopTime / 256.0f;
			ShouldChangePosStep = true;
		}
	}

	if (tv.Rate != paraRate.NoValue)
	{
		if (InertiaBOn)
		{
			IRateGliding = true;
			IRateTarget = powf(2, float(tv.Rate - 32768) / 16384.0f);
			IRateStep = (IRateTarget - Rate)
				/ (pmi->pMasterInfo->SamplesPerTick * InertiaBLength);
		}
		else
		{
			Rate = powf(2, float(tv.Rate - 32768) / 16384.0f);
			ShouldChangePosStep = true;
		}
	}

	if (tv.RatePar != paraRatePar.NoValue)
	{
		if (tv.RatePar == 0) RateMethod = RATEMTH_TIME;
		else RateMethod = RATEMTH_RATE;
		ShouldChangePosStep = true;
	}

	if (tv.Volume != paraVolume.NoValue)
	{
		if (InertiaBOn)
		{
			IVolGliding = true;
			IVolTarget = tv.Volume / 128.0f;
			IVolStep = (IVolTarget - Volume)
				/ (pmi->pMasterInfo->SamplesPerTick * InertiaBLength);
		}
		else
			Volume = tv.Volume / 128.0f;
	}

	if (tv.XFade != paraXFade.NoValue)
	{
		if (tv.XFade == 0) DoRamping = false;
		else
		{
			DoRamping = true;

			int RampSize;

			if (tv.XFade <= 0x100) RampSize = tv.XFade; //samples
			else RampSize = (tv.XFade - 0x100)
				* pmi->pMasterInfo->SamplesPerSec / 1000; //milliseconds

			RampStep = 1.0f / float(RampSize);
		}
	}

	if (ShouldChangePosStep) ChangePosStep();
}

////////////////////////////////////////////////////////////////////////

void mi::Tick() 
{
	for (int t=0; t < numTracks; t++)
		Tracks[t].Tick(tval[t]);
}

void mi::AttributesChanged()
{
	for (int t=0; t < numTracks; t++)
		Tracks[t].ChangePosStep();
		
}

////////////////////////////////////////////////////////////////////////

void CTrack::Stop()
{
	Playing = false;
}

////////////////////////////////////////////////////////////////////////

void mi::Stop() 
{
	for (int t=0; t < numTracks; t++)
		Tracks[t].Stop();
}

////////////////////////////////////////////////////////////////////////

void CTrack::EndOfLoopReached(int const StartSamp, int const EndSamp)
//Do stuff when the end of the loop is reached (go back to start, change
//direction, etc etc.
{
	if (DoRamping) //Ramping occurs at the end of a loop
	{
		IsRamping	= true;
		RampVol		= 1.0f;
		RampPos		= CurPos;
		RampDir		= LoopDir1;
	}

	switch (LoopDir2) {
	case LOOPDIR_STRAIGHT:
		if (CurPos > EndSamp)   CurPos = float(StartSamp);
		if (CurPos < StartSamp) CurPos = float(EndSamp);
		break;
	case LOOPDIR_PINGPONG:
		if (LoopDir1 == LOOPDIR_FWD)
		{
			LoopDir1 = LOOPDIR_REV;

			//Check that we will be in the right place, given that start>end
			//gives reversed playback
			if (LoopPt1 < LoopPt2) CurPos = float(EndSamp);
			else CurPos = float(StartSamp);
		}
		else 
		{
			LoopDir1 = LOOPDIR_FWD;
			if (LoopPt1 > LoopPt2) CurPos = float(EndSamp);
			else CurPos = float(StartSamp);
		}
		break;
	case LOOPDIR_RANDOM:
		if (frand() < 0.5f) //50/50 chance (hopefully)
		{
			LoopDir1 = LOOPDIR_FWD;
			if (LoopPt1 > LoopPt2) CurPos = float(EndSamp);
			else CurPos = float(StartSamp);
		}
		else
		{
			LoopDir1 = LOOPDIR_REV;
			if (LoopPt1 < LoopPt2) CurPos = float(EndSamp);
			else CurPos = float(StartSamp);
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::UpdateInertia()
{
	if (IStartGliding)
	{
		LoopPt1 += (IStartStep * InertiaUpdateCounterLimit);
		if (((LoopPt1 > IStartTarget) && (IStartStep > 0))
			|| ((LoopPt1 < IStartTarget) && (IStartStep < 0)))
			LoopPt1 = IStartTarget;
		if (LoopPt1 == IStartTarget) IStartGliding = false;

		ChangePosStep();
	}

	if (IEndGliding)
	{
		LoopPt2 += (IEndStep * InertiaUpdateCounterLimit);
		if (((LoopPt2 > IEndTarget) && (IEndStep > 0))
			|| ((LoopPt2 < IEndTarget) && (IEndStep < 0)))
			LoopPt2 = IEndTarget;
		if (LoopPt2 == IEndTarget) IEndGliding = false;

		ChangePosStep();
	}

	if (ITimeGliding)
	{
		LoopTime += (ITimeStep * InertiaUpdateCounterLimit);
		if (((LoopTime > ITimeTarget) && (ITimeStep > 0))
			|| ((LoopTime < ITimeTarget) && (ITimeStep < 0)))
			LoopTime = ITimeTarget;
		if (LoopTime == ITimeTarget) ITimeGliding = false;

		ChangePosStep();
	}

	if (IRateGliding)
	{
		Rate += (IRateStep * InertiaUpdateCounterLimit);
		if (((Rate > IRateTarget) && (IRateStep > 0))
			|| ((Rate < IRateTarget) && (IRateStep < 0)))
			Rate = IRateTarget;
		if (Rate == IRateTarget) IRateGliding = false;

		ChangePosStep();
	}

	if (IVolGliding)
	{
		Volume += (IVolStep * InertiaUpdateCounterLimit);
		if (((Volume > IVolTarget) && (IVolStep > 0))
			|| ((Volume < IVolTarget) && (IVolStep < 0)))
			Volume = IVolTarget;
		if (Volume == IVolTarget) IVolGliding = false;
	}


	InertiaUpdateCounter = 0;
}




//Following three functions return an antialiased sample 
//from a mono buffer and left/right of stereo, respectively.

inline float GetSampleM(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos); //how far 'into' the sample we are
	return (pSamples[int(Pos)] * (1-frac)) 
		 + (pSamples[int(Pos)+1] * frac);
}

inline float GetSampleL(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos);
	return (pSamples[int(Pos)*2] * (1-frac)) 
		 + (pSamples[int(Pos)*2+2] * frac);
}

inline float GetSampleR(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos);
	return (pSamples[int(Pos)*2+1] * (1-frac)) 
		 + (pSamples[int(Pos)*2+3] * frac);
}

////////////////////////////////////////////////////////////////////////

bool CTrack::Generate(float *psamples, int numsamples)
{
	if (Playing)
	{
		if (pmi->pCB->GetWaveLevel(WaveNo,0) == NULL) //wave exists
			return false;
		else
		{
			float LSamp, RSamp;

			int IsStereo = (pmi->pCB->GetWave(WaveNo)->Flags & WF_STEREO);
			CWaveLevel const *pLevel = pmi->pCB->GetWaveLevel(WaveNo,0);

			PosStepUpdateCounter++;
			if (PosStepUpdateCounter > PosStepUpdateCounterLimit)
				ChangePosStep();

			int StartSamp = int(LoopStart * pLevel->numSamples);
			int EndSamp   = int(LoopEnd   * pLevel->numSamples);
			if ((CurPos > EndSamp) || (CurPos < StartSamp))
				EndOfLoopReached(StartSamp, EndSamp);

			//Two separate loops for mono and stereo samples
			if (IsStereo)
			{
				for (int i=0; i < numsamples*2; i+=2)
				{
					InertiaUpdateCounter++;
					if (InertiaUpdateCounter > InertiaUpdateCounterLimit)
						UpdateInertia();

					LSamp = GetSampleL(CurPos, pLevel->pSamples);
					RSamp = GetSampleR(CurPos, pLevel->pSamples);
					
					if (LoopDir1 == LOOPDIR_FWD) CurPos += PosStep;
					else CurPos -= PosStep;

					if (IsRamping)
					{
/*						if ((RampVol <= 0) || (RampPos < 0)  //ramp finished
							|| (RampPos > pLevel->numSamples))
							IsRamping = false;
*/						if (RampVol <= 0) IsRamping = false;
						else
						{
							if (RampPos < 0) RampPos = (float)(pLevel->numSamples);
							if (RampPos > pLevel->numSamples) RampPos = 0.0f;

							LSamp = (LSamp * (1.0f - RampVol)) + 
								(GetSampleL(RampPos, pLevel->pSamples) * RampVol);
							RSamp = (RSamp * (1.0f - RampVol)) + 
								(GetSampleR(RampPos, pLevel->pSamples) * RampVol);

							if (RampDir == LOOPDIR_FWD) RampPos += PosStep;
							else RampPos -= PosStep;

							RampVol -= RampStep;
						}
					}

					if ((CurPos > EndSamp) || (CurPos < StartSamp))
						EndOfLoopReached(StartSamp, EndSamp);

					psamples[i  ] += (LSamp * Volume); //Mix with other 
					psamples[i+1] += (RSamp * Volume); //tracks' output
				}
			}
			else
			{
				for (int i=0; i < numsamples*2; i+=2)
				{
					InertiaUpdateCounter++;
					if (InertiaUpdateCounter > InertiaUpdateCounterLimit)
						UpdateInertia();

					LSamp = GetSampleM(CurPos, pLevel->pSamples);
					//'LSamp' is used for both samples here

					if (LoopDir1 == LOOPDIR_FWD) CurPos += PosStep;
					else CurPos -= PosStep;

					if (IsRamping)
					{
						RampVol -= RampStep;

/*						if ((RampVol <= 0) || (RampPos < 0)  //ramp finished
							|| (RampPos > pLevel->numSamples))
							IsRamping = false;
*/						if (RampVol <= 0) IsRamping = false;
						else
						{
							if (RampPos < 0) RampPos = (float)(pLevel->numSamples);
							if (RampPos > pLevel->numSamples) RampPos = 0.0f;
							
							LSamp = (LSamp * (1.0f - RampVol)) + 
								(GetSampleM(RampPos, pLevel->pSamples) * RampVol);

							if (RampDir == LOOPDIR_FWD) RampPos += PosStep;
							else RampPos -= PosStep;
						}
					}

					if ((CurPos > EndSamp) || (CurPos < StartSamp))
						EndOfLoopReached(StartSamp, EndSamp);

					psamples[i  ] += (LSamp * Volume);
					psamples[i+1] += (LSamp * Volume);		
				}
			}
			if (Volume != 0.0f) return true;
			else return false;
		}
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////

bool mi::WorkMonoToStereo(float *pin, float *pout, int numsamples, int const mode)
{
	bool DidWork = false;

	for (int i=0; i < numsamples*2; i++) pout[i] = 0;

	for (int c=0; c < numTracks; c++)
		if (Tracks[c].Generate(pout, numsamples)) DidWork = true;

	return DidWork; //return false if none of the tracks did work
}

////////////////////////////////////////////////////////////////////////

/*
void CTrack::AboutBox()
{
	static char txt[255];
	sprintf (txt, "Wave no %i\nLength %i\nFlags %i\nPointer %i",
		WaveNo,
		pmi->pCB->GetWaveLevel(WaveNo,0)->numSamples,
		pmi->pCB->GetWave(WaveNo)->Flags,
		int(pmi->pCB->GetWaveLevel(WaveNo,0)));

	
	sprintf (txt, "Wave no %i", WaveNo);
	MessageBox (NULL,txt,"About",MB_OK|MB_SYSTEMMODAL);
	sprintf (txt, "Pointer %i", int(pmi->pCB->GetWaveLevel(WaveNo,0)));
	MessageBox (NULL,txt,"About",MB_OK|MB_SYSTEMMODAL);
	sprintf (txt, "Length %i", pmi->pCB->GetWaveLevel(WaveNo,0)->numSamples);
	MessageBox (NULL,txt,"About",MB_OK|MB_SYSTEMMODAL);
	sprintf (txt, "Flags %i", pmi->pCB->GetWave(WaveNo)->Flags);
	
	
	if (InertiaAOn)
		MessageBox (NULL,"Tis on","About",MB_OK|MB_SYSTEMMODAL);
	else
		MessageBox (NULL,"Tis not on","About",MB_OK|MB_SYSTEMMODAL);


	sprintf (txt, "%.5f", RampPos);
	MessageBox (NULL,txt,"About",MB_OK|MB_SYSTEMMODAL);

}
*/

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i) 
{
//	Tracks[0].AboutBox();
/*	static char txt[255];
	int above=0;
	for (int i2=1; i2<=10000; i2++) if (frand() > 0.5f) above++;
//	sprintf (txt, "%.8f is a random number", frand());
	sprintf (txt, "%i out of 10000", above);
	MessageBox (NULL,txt,"About",MB_OK|MB_SYSTEMMODAL);
*/

	//About box

#ifdef ALPHA_TEST_VERSION
	MessageBox (NULL,
		"Thanks for downloading this machine.\n"
		"Please note it is an ALPHA TEST VERSION, as such please do not:\n"
		"* Use ith in songs\n"
		"* Distribute it\n"
		"* Rely on it not to crash\n"
		"However, please do:\n"
		"* Test it\n"
		"* Email any comments/suggestions/bugs to e@btd2001.freeserve.co.uk\n"
		"Thanks!",
		"BTDSys Stutter",MB_OK|MB_SYSTEMMODAL);
#else
	MessageBox (NULL,
		"BTDSys Stutter v1.1\n"
		"©2002 Edward Powley (BTDSys)\n"
		"Email comments, suggestions and bugs to e@btd2001.freeserve.co.uk",
		"BTDSys Stutter",MB_OK|MB_SYSTEMMODAL);
#endif


}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];
	switch (param){
	case 0: //WaveNo
		sprintf(txt, "%i", value);
		return txt;
		break;

	case 1: //Trigger
		if (value == SWITCH_ON)
			return "On";
		else
			return "Off";
		break;

	case 2: //Start
		sprintf(txt, "%.1f%%", value / 655.34f);
		return txt;
		break;

	case 3: //End
		if (aval.EndLength == 0)	
			sprintf(txt, "%.1f%%", value / 655.34f);
		else
			sprintf(txt, "%.1f%%", (value - 32768) / 327.68f);
		return txt;
		break;

	case 5: //LoopDir
		switch (value){
		case 0: return "Forward"; break;
		case 1: return "Reverse"; break;
		case 2: return "Pingpong";break;
		case 3: return "Random";  break;
		default: return NULL;
		}
		break;

	case 6: //LoopTime
		if (value == 0)
			return "100% rate";
		else {
			int whole = int(value / 256);
			int frac  = value % 256;
			sprintf(txt, "%i %i/256", whole, frac);
			return txt;
		}
		break;

	case 7: //Rate
		sprintf(txt, "%.1f%%", powf(2, (float)(value-32768) / 16384.0f) * 100);
		return txt;
		break;

	case 8: //RatePar
		if (value == 0)
			return "Loop Time";
		else
			return "Rate";
		break;

	case 9: //Volume
		sprintf(txt, "%.1f%%", value / 1.28f);
		return txt;
		break;

	case 10: //XFade (0=off, 1-100=no of samples, 101=1ms, 102=2ms... 4E8=1000ms
		if (value == 0) return "Off";
		if ((value >=1) && (value <= 0x100))
		{
			sprintf(txt, "%i samp", value);
			return txt;
		}
		if (value >= 0x101)
		{
			sprintf(txt, "%i ms", value - 0x100);
			return txt;
		}
		else
			return NULL;
		break;

	case 4:  //Inertia A
	case 11: //Inertia B
		if (value == 0) return "Off";
		else
		{
			sprintf(txt, "%.1f ticks", (float)(value)/10.0f);
			return txt;
		}
		break;

	default:
		return NULL;
	}
}

////////////////////////////////////////////////////////////////////////

#pragma optimize ("", on)

