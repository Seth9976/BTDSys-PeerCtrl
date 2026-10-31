/*
Copyright (c) 2002-2008, Edward Powley
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

#include <windows.h>
#include "../mdk/MachineInterface.h"
#include <float.h>
#include <stdlib.h>
#include "resource.h"
#include <math.h>
#include "../peerlib2/peerctrl.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

#define TWICEPI	6.28318530717958647692528676655901f
#define ONCEPI	3.1415926535897932384626433832795f

#define UPDATE_PHASESTEP	PhaseStep = TWICEPI / PeriodCurrent

////////////////////////////////////////////////////////////////////////

CMachineParameter const paraOnOff =
{
	pt_switch,			//Type
	"On/Off",			//Short name
	"1=On, 0=Off",		//Long name
	SWITCH_OFF,			//Min value
	SWITCH_ON,			//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	SWITCH_OFF			//Default value
};

CMachineParameter const paraGlobalAmp =
{
	pt_word,			//Type
	"Amp",				//Short name
	"Global Amp Multiplier (0=0%, 8000=100%, FFFE=200%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
};

CMachineParameter const paraPhase =
{
	pt_word,			//Type
	"Phase",			//Short name
	"Manual Phase (0=0%, 8000=100%)",		//Long name
	0,					//Min value
	0x7FFF,				//Max value
	0xFFFE,				//No value
	0,					//Flags
	0					//Default value
};

CMachineParameter const paraShape =
{
	pt_byte,			//Type
	"Wave Shape",		//Short name
	"Wave Shape (0=Sine, 1=Square, 2=Tri, 3=Ramp Up, 4=Ramp Dn, 5=Random, 6=Wander, 7=Wavetable, 8=Min, 9=Max, 10=Center)",		//Long name
	0,					//Min value
	10,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraWaveNo =
{
	pt_byte,			//Type
	"Wave Number",		//Short name
	"Wavetable Wave Number",		//Long name
	WAVE_MIN,			//Min value
	WAVE_MAX,			//Max value
	WAVE_NO,			//No value
	MPF_WAVE | MPF_STATE,	//Flags
	1					//Default value
};

CMachineParameter const paraMinDiff =
{
	pt_word,			//Type
	"Min Diff",			//Short name
	"Minimum difference for random values (0=no min, 0x8000=50% of max-min)",	//Long name
	0,					//Min value
	0x8000,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};


CMachineParameter const paraPeriod =
{
	pt_word,			//Type
	"Rate",				//Short name
	"Rate (in tick/16 or ms or Hz/4096)",		//Long name
	1,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x100				//Default value
};

CMachineParameter const paraPeriodUnit =
{
	pt_byte,			//Type
	"Rate Unit",		//Short name
	"Rate Unit (0=tick/16, 1=ms, 2=Hz/4096)",		//Long name
	0,					//Min value
	2,					//Max value
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

CMachineParameter const paraMin =
{
	pt_word,			//Type
	"Min",				//Short name
	"Min param value (0=0%, 0xFFFE=100%)",		//Long name
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
	"Max param value (0=0%, 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0xFFFE				//Default value
};

CMachineParameter const paraCenter =
{
	pt_word,			//Type
	"Center",			//Short name
	"Center value (0=0%, 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x7FFF				//Default value
};

CMachineParameter const paraAmpl =
{
	pt_word,			//Type
	"Amplitude",		//Short name
	"Amplitude (1=-100%, 0x8000=0%, FFFF=+100%)",		//Long name
	1,					//Min value
	0xFFFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0xC000				//Default value
};

CMachineParameter const paraExtentMode =
{
	pt_byte,			//Type
	"Extent Mode",		//Short name
	"Extent Setting Mode (0=Min/Max, 1=Center/Ampl)",		//Long name
	0,					//Min value
	1,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraUpdFreq =
{
	pt_byte,			//Type
	"Update Freq",		//Short name
	"Update Frequency (in ticks)",		//Long name
	0,					//Min value
	16,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	1					//Default value
};

CMachineParameter const paraTrackPhase =
{
	pt_word,			//Type
	"Phase",			//Short name
	"Phase Offset (1=-50%, 0x8000=0%, FFFF=+50%)",		//Long name
	1,					//Min value
	0xFFFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0x8000				//Default value
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

CMachineAttribute const attrHeedStopBtn =
{
	"Heed Stop Btn (1=Y 0=N)",	//Name
	0,							//Min
	1,							//Max
	0							//Default
};

////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraOnOff,
	&paraGlobalAmp,
	&paraPhase,
	&paraShape,
	&paraWaveNo,
	&paraMinDiff,
	&paraPeriod,
	&paraPeriodUnit,
	&paraInertia,

	&paraMin,
	&paraMax,
	&paraCenter,
	&paraAmpl,
	&paraExtentMode,
	&paraUpdFreq,
	&paraTrackPhase,
	&paraTrack
};

CMachineAttribute const *pAttributes[] = {
	&attrHeedStopBtn,
};

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class tvals
{
public:
	word Min;
	word Max;
	word Center;
	word Ampl;
	byte ExtentMode;
	byte UpdFreq;
	word TrackPhase;
	byte Track;
};

class gvals
{
public:
	byte OnOff;
	word GlobalAmp;
	word Phase;
	byte Shape;
	byte WaveNo;
	word MinDiff;
	word Period;
	byte PeriodUnit;
	word Inertia;
};

class avals
{
public:
	int HeedStopBtn;
};

#pragma pack()

////////////////////////////////////////////////////////////////////////

#define MAX_TRACKS	255

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT|MIF_CONTROL_MACHINE,					//Flags
	1,								//Min tracks
	MAX_TRACKS,						//Max tracks
	9,								//Number of global parameters
	8,								//Number of track parameters
	pParameters,					//Pointer to parameters
	1,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"Peer LFO",						//Full name
	"PeerLFO",						//Short name
	"BTDSys / IX",					//Author
	"/Assign Parameter\n"
	"/Copy Center/Ampl To Min/Max\n"
	"/Copy Min/Max To Center/Ampl\n"	
	"Show PAL...\n"
	"About..."						//Commands                                  
};

////////////////////////////////////////////////////////////////////////

#pragma pack(8)

class mi;
class CTrack;

class MyPeerCtrl : public CPeerCtrl
{
public:
	CTrack* m_track;

	virtual void OnChangeAssignment(PeerEvent trigger);
};

class CTrack
{
public:
	void Reset();
	void Tick(tvals &tv);
	void UpdateMinMax();
	void UpdateInertia();
	void StopInertia();
	bool CheckMachine();
	void DoLFO(float CurrentValue);
	bool DoEndAssignDlg (HWND hDlg, bool ShouldSave);
	void CA2MM();
	void MM2CA();

	mi *pmi;
	int TrackNumber;

	MyPeerCtrl m_ctrl;
	int BusTrack;
	int Min, Max;
	int BusMin, BusMax;
	int pMin, pMax, pCenter, pAmpl;
	byte ExtentMode;

	bool MinSliding;
	int MinTarget;
	float MinStep;
	bool MaxSliding;
	int MaxTarget;
	float MaxStep;

	float PhaseOffset;

	byte UpdInt;
	byte UpdCount;

	float LastValue;

	int rcount;
	int lastout;

};

#pragma pack()

////////////////////////////////////////////////////////////////////////

class mi : public CMachineInterface, public CMachineInterfaceEx
{
public:
	mi();
	virtual ~mi();

	virtual void DoAssignPar (int TrackNum);
//	virtual void DoEndAssignDlg (HWND hDlg);
	virtual void UpdateInertia();
	virtual void DoLFO(bool Reset);
	virtual inline float GetWaveValue(byte WaveType, float WavePhase);
	virtual inline float GetNewRandomValue(float Last);

public:
	virtual void Tick();
	virtual void Stop();
	virtual void Init(CMachineDataInput * const pi);
	virtual bool Work(float *psamples, int numsamples, int const mode);
	virtual void Command(int const i);
	virtual void Save(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void SetNumTracks(int const n) { numTracks = n; }

public:
	virtual void ImportFinished();
	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);

public:

	int numTracks;
	CTrack Tracks [MAX_TRACKS];

	tvals tval [MAX_TRACKS];
	gvals gval;
	avals aval;

public:
	CMachine *ThisMac;

	bool Playing;
	float Phase, PhaseStep;
	byte Shape;
	byte WaveNo;
	float RandomLast2, RandomLast, RandomNext, RandomNext2;
	float MinDiff;
	bool Reset;

	bool InertiaActive;
	int SlideLength;
	bool PeriodSliding;
	float PeriodCurrent, PeriodTarget;
	float PeriodStep;
	bool GlobalAmpSliding;
	float GlobalAmpCurrent, GlobalAmpTarget, GlobalAmpStep;

	byte PeriodUnit;
	
	bool Initialising;

	int m_hostversion;
	int rcount;

	CSubTickInfo const *m_pSubtickInfo;
};

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////

inline float frand()
// returns random value between 0 and 1
// from Waveout.cpp
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

////////////////////////////////////////////////////////////////////////

mi::mi() 
//Constructor
{
	rcount = 1;

	GlobalVals = &gval;
	TrackVals = &tval;
	AttrVals = (int*) &aval;

	for (int c=0; c < MAX_TRACKS; c++)
	{
		Tracks[c].pmi = this;
		Tracks[c].TrackNumber = c;
	}
}

mi::~mi() 
//Destructor
{
}

////////////////////////////////////////////////////////////////////////

void CTrack::Reset()
//Initialise track-related values
{
	m_ctrl.m_track = this;
	m_ctrl.Init(pmi->ThisMac, pmi);
	m_ctrl.SetLabelAsTrackNumber(TrackNumber);

	UpdCount = 100; //force update
	UpdInt = 1;

	MinSliding = false;
	MaxSliding = false;

	pMin = paraMin.DefValue;
	pMax = paraMax.DefValue;
	Min = pMin;
	Max = pMax;
	UpdateMinMax();

	pCenter = paraCenter.DefValue;
	pAmpl = (paraAmpl.DefValue - 0x8000) * 2;
	ExtentMode = 0;

	PhaseOffset = 0.0f;
	LastValue = -1.0f;

	BusTrack = 0;
}

void MyPeerCtrl::OnChangeAssignment(PeerEvent trigger)
{
	m_track->UpdateMinMax();
}

////////////////////////////////////////////////////////////////////////

void mi::Init (CMachineDataInput * const pi)
//Initialise machine and load data from file
{
	pCB->SetMachineInterfaceEx(this);

	Playing = false;
	Phase = 0.0f;
	PeriodSliding = false;
	GlobalAmpSliding = false;
	ThisMac = pCB->GetThisMachine();

	for (int c=0; c < MAX_TRACKS; c++)
	{
		Tracks[c].Reset();
	}

	CPeerCtrl::SetEventHandlers(pCB);

	RandomLast2 = frand();
	RandomLast  = frand();
	RandomNext  = frand();
	RandomNext2 = frand();

	Initialising = true;

	if (pi != NULL) //Read data from file
	{
		byte dummy; pi->Read(dummy); // Handle fake MDK version byte

		byte Version;
		pi->Read(Version);
		if (Version == 2)
		{
			byte NumTracks;
			pi->Read(NumTracks);

			for (int t=0; t < NumTracks; t++)
			{
				byte HasPar;
				pi->Read(HasPar);
				if (HasPar != 0)
				{
					char macname[256];
					pi->ReadString(macname, 256);
					int paramn, group;
					pi->Read(paramn);
					pi->Read(group);
					Tracks[t].m_ctrl.PushFileData(macname, paramn, group);
				}
				else
					Tracks[t].m_ctrl.PushFileData(NULL, 0, 0);
			}
		}
		else
		{
			MessageBox (NULL, "Incompatible data", "BTDSys Peer LFO", MB_OK|MB_SYSTEMMODAL);
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Save (CMachineDataOutput * const po)
//Save data to file
{
	po->Write((byte)0); // Fake MDK version byte

	po->Write(byte(2));	//version
	po->Write(byte(numTracks));
	for (int t=0; t < numTracks; t++)
	{
		if (Tracks[t].m_ctrl.GotParam())
		{
			po->Write(byte(0xFF));
			po->Write(Tracks[t].m_ctrl.GetMachineName());
			po->Write((int)Tracks[t].m_ctrl.GetParamNumber_Grouped());
			po->Write((int)Tracks[t].m_ctrl.GetGroup());
		}
		else
			po->Write(byte(0));
	}
}

////////////////////////////////////////////////////////////////////////

bool mi::Work(float *psamples, int numsamples, const int mode)
//This is the most sensible place to set Initialising to false, since it
//(probably) won't be called until all machines are present and correct.
{
	Initialising = false;

	m_pSubtickInfo = pCB->GetSubTickInfo();

	int pos = m_pSubtickInfo ? m_pSubtickInfo->PosInSubTick : pMasterInfo->PosInTick;

	if (pos == 0)
	{
		CPeerCtrl::BeginImmediateBatch();
		DoLFO(Reset);
		CPeerCtrl::EndImmediateBatch();
	}

	return false;
}

////////////////////////////////////////////////////////////////////////

void CTrack::UpdateMinMax()
//If we need to update min and max values for parameter
{
	if (m_ctrl.GotParam())
	{
		float tcent = float(Max + Min) / 2.0f;
		float tampl = float(Max - Min) / 2.0f;
		float numin = tcent - (tampl * pmi->GlobalAmpCurrent);
		float numax = tcent + (tampl * pmi->GlobalAmpCurrent);

		const CMachineParameter* BusPar = m_ctrl.GetParamInfo();

		if (BusPar->Type != pt_switch)
		{
			BusMin = int(numin / 65534.0f
						* float(BusPar->MaxValue - BusPar->MinValue + 1))
						+ BusPar->MinValue;
			BusMax = int(numax / 65534.0f
						* float(BusPar->MaxValue - BusPar->MinValue + 1))
						+ BusPar->MinValue;
		}
		else
		{
			BusMin = int(numin / 32768.0f);
			BusMax = int(numax / 32768.0f);
		}
	}
}

////////////////////////////////////////////////////////////////////////

float mi::GetWaveValue (byte WaveType, float WavePhase)
//Get a sample from a particular type of wave
//range 0 to 1
{
	switch (WaveType)
	{
	case 0: //sine
		return sinf(WavePhase) * 0.5f + 0.5f;
	case 1: //square
		return (WavePhase < ONCEPI) ? 0.0f : 1.0f;
	case 2: //tri
		return (WavePhase < ONCEPI)
			? ( WavePhase            / ONCEPI)
			: ((TWICEPI - WavePhase) / ONCEPI);
	case 3: //rampup
		return ( WavePhase            / TWICEPI);
	case 4: //rampdn
		return ((TWICEPI - WavePhase) / TWICEPI);
	case 5: //random
		return RandomNext; // Used to be frand() but was deliberately never called
	case 6: //wander
		return RandomLast + (RandomNext - RandomLast) * WavePhase / TWICEPI;
	case 7: //wavetable
		{
			if (pCB->GetWaveLevel(WaveNo,0) != NULL) //wave exists
			{
				int IsStereo = (pCB->GetWave(WaveNo)->Flags & WF_STEREO);
				CWaveLevel const *pLevel = pCB->GetWaveLevel(WaveNo,0);
				if (pLevel->pSamples != NULL)
				{
					int TheSamp = int(pLevel->numSamples * float(WavePhase / TWICEPI));

					if (IsStereo)
						return (pLevel->pSamples[TheSamp*2] / 65536.0f) + 0.5f;
					else
						return (pLevel->pSamples[TheSamp  ] / 65536.0f) + 0.5f;
				}
				else
					return 0.0f;
			}
			else //wave don't exist
				return 0.0f;
		}
	case 8: //Min
		return 0.0f;
	case 9: //Max
		return 1.0f;
	case 10: //Center
		return 0.5f;
	default:
		return 0.0f;
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::UpdateInertia()
//Update inertia things at the track level
{
	if (MinSliding)
	{
		Min += int(MinStep * float(pmi->pMasterInfo->SamplesPerTick));

		if (((MinStep > 0) && (Min >= MinTarget))
			|| ((MinStep < 0) && (Min <= MinTarget)))
		{
			Min = MinTarget;
			MinSliding = false;
		}

		UpdateMinMax();
	}

	if (MaxSliding)
	{
		Max += int(MaxStep * float(pmi->pMasterInfo->SamplesPerTick));

		if (((MaxStep > 0) && (Max >= MaxTarget))
			|| ((MaxStep < 0) && (Max <= MaxTarget)))
		{
			Max = MaxTarget;
			MaxSliding = false;
		}

		UpdateMinMax();
	}
}

////////////////////////////////////////////////////////////////////////

void mi::UpdateInertia()
//Update inertia things at the machine level
{
	//Assume that InertiaActive true
	
	if (PeriodSliding)
	{
		PeriodCurrent += PeriodStep * float(pMasterInfo->SamplesPerTick);

		if (((PeriodStep > 0) && (PeriodCurrent >= PeriodTarget))
			|| ((PeriodStep < 0) && (PeriodCurrent <= PeriodTarget)))
		{
			PeriodCurrent = PeriodTarget;
			PeriodSliding = false;
		}

		UPDATE_PHASESTEP;
	}

	if (GlobalAmpSliding)
	{
		GlobalAmpCurrent += GlobalAmpStep * float(pMasterInfo->SamplesPerTick);

		if (((GlobalAmpStep > 0) && (GlobalAmpCurrent >= GlobalAmpTarget))
			|| ((GlobalAmpStep < 0) && (GlobalAmpCurrent <= GlobalAmpTarget)))
		{
			GlobalAmpCurrent = GlobalAmpTarget;
			GlobalAmpSliding = false;
		}
		
		for (int t=0; t < numTracks; t++)
			Tracks[t].UpdateMinMax();	
	}

	for (int t=0; t < numTracks; t++)
		Tracks[t].UpdateInertia();
}

////////////////////////////////////////////////////////////////////////

void CTrack::StopInertia()
//Halt any inertia things that are happening at the track level
//Machine level inertia is stopped within mi::Tick()
{
	if (MinSliding)
	{
		Min = MinTarget;
		MinSliding = false;
		UpdateMinMax();
	}
	if (MaxSliding)
	{
		Max = MaxTarget;
		MaxSliding = false;
		UpdateMinMax();
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::DoLFO(float CurrentValue)
//Track-level LFO stuff: sending values
{
	if(UpdInt > 0 && pmi->pMasterInfo->PosInTick == 0)
	{
		UpdCount++;
	}

	if (UpdCount >= UpdInt)
	{
		UpdCount = 0;

		if (m_ctrl.GotParam() && ((CurrentValue != -1) || (PhaseOffset != 0.0f)))
		{
			const CMachineParameter* BusPar = m_ctrl.GetParamInfo();

			int pval;

			if (PhaseOffset == 0.0f) //random
			{
				pval = int(
					CurrentValue * float(BusMax - BusMin + 1))
					+ BusMin;
			}
			else
			{
				float MyPhase = pmi->Phase + PhaseOffset;
				float MyValue;
				if (pmi->Shape == 6) //wander
				{
					if (MyPhase < 0.0f)
					{
						MyValue = pmi->RandomLast2 
							+ (pmi->RandomLast - pmi->RandomLast2) 
							* (MyPhase + TWICEPI) / TWICEPI;
					}
					else if (MyPhase > TWICEPI)
					{
						MyValue = pmi->RandomNext 
							+ (pmi->RandomNext2 - pmi->RandomNext) 
							* (MyPhase - TWICEPI) / TWICEPI;
					}
					else
					{
						MyValue = pmi->RandomLast 
							+ (pmi->RandomNext - pmi->RandomLast) 
							* MyPhase / TWICEPI;
					}
				}
				else if (pmi->Shape == 5) //random
				{
					if (MyPhase < 0.0f)
						MyValue = pmi->RandomLast;
					else if (MyPhase > TWICEPI)
						MyValue = pmi->RandomNext2;
					else
						MyValue = pmi->RandomNext;

					if (MyValue != LastValue)
						LastValue = MyValue;
					else
						MyValue = -1;
				}
				else
				{
					if (MyPhase < 0.0f) MyPhase += TWICEPI;
					if (MyPhase > TWICEPI) MyPhase -= TWICEPI;
					MyValue = pmi->GetWaveValue(pmi->Shape, MyPhase);
				}

				if (MyValue == -1)
					pval = -1;
				else
					pval = int(MyValue * float(BusMax - BusMin + 1)) + BusMin;
			}

			// Prevent sending the same value repeatedly but only in subtick mode (for backwards compatibility)
			if (pmi->m_pSubtickInfo)
			{
				// for random, we might want to allow duplicate value so check whether a new value has been picked
				if (pmi->Shape == 5)
				{
					if(pmi->rcount == rcount) return;
					rcount = pmi->rcount;
				}
				else // for other shapes just check for duplicate value
				{
					if(pval == lastout) return;
					lastout = pval;
				}
			}

			if (pval != -1)
			{
				if (BusMax > BusMin)
				{
					if (pval > BusMax) pval = BusMax;
					if (pval < BusMin) pval = BusMin;
				}
				else
				{
					if (pval < BusMax) pval = BusMax;
					if (pval > BusMin) pval = BusMin;
				}

				if (pval < BusPar->MinValue) pval = BusPar->MinValue;
				if (pval > BusPar->MaxValue) pval = BusPar->MaxValue;

				if (m_ctrl.GetGroup() == GROUP_GLOBAL)
					m_ctrl.ControlChange_Immediate(pval);
				else //Track
				{
					if (BusTrack != -1) //Not all tracks
						m_ctrl.ControlChange_Immediate(BusTrack, pval);
					else //All tracks
					{
						int ntracks = pmi->pCB->GetNumTracks(m_ctrl.GetMachine());
						for (int t=0; t < ntracks; t++)
							m_ctrl.ControlChange_Immediate(t, pval);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////

float mi::GetNewRandomValue(float Last)
//Get a new random value, given it has to be at least MinDiff from the last
{
	float t;
	int count = 0;
	
	do 
	{
		t = frand(); 
		count++;
	} 
	while ((fabsf(Last-t) < MinDiff) && (count < 100));
	//if we don't get one within 100 tries we never will so sod it

	rcount++;

/*	char txt[256];
	sprintf(txt,"GetNewRandomValue got %.2f",t);
	OutputDebugString(txt);
*/
	return t;
}

////////////////////////////////////////////////////////////////////////

void mi::DoLFO(bool Reset)
//Machine-level LFO stuff: choosing a value
{
	if (Playing && !Initialising) //Do the actual stuff
	{
		float CurrentValue = -1;

		char txt[256];
		_snprintf(txt, 256, "mi::DoLFO old phase %.2f, ",Phase);

		float OldPhase = Phase;

		if (PeriodUnit == 0) //tick/16
			PhaseStep = TWICEPI / (PeriodCurrent * pMasterInfo->SamplesPerTick / 16);

		Phase += (PhaseStep * (m_pSubtickInfo ? m_pSubtickInfo->SamplesPerSubTick : pMasterInfo->SamplesPerTick));

		if (Phase >= TWICEPI) Reset = true;
//		Phase = fmodf(Phase, TWICEPI);
		while (Phase > TWICEPI) {Phase -= TWICEPI; Reset = true;}

//		if (Phase < OldPhase) Reset = true;

//		sprintf(txt,"%snew phase %.2f",txt,Phase);
//		OutputDebugString(txt);

		if (Reset)
		{
//			OutputDebugString("mi::DoLFO reset");
			if ((Shape == 5) || (Shape == 6))
			{
				RandomLast2 = RandomLast;
				RandomLast  = RandomNext;
				RandomNext  = RandomNext2;
				RandomNext2 = GetNewRandomValue(RandomNext);
				CurrentValue = RandomNext;
			}
		}

		CurrentValue = GetWaveValue (Shape, Phase);

		for (int t=0; t < numTracks; t++)
			Tracks[t].DoLFO(CurrentValue);

/*		sprintf(txt,"mi::DoLFO value %.2f",CurrentValue);
		OutputDebugString(txt);
*/	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::Tick(tvals &tv)
//Track-level tick stuff
{
	if (tv.UpdFreq != paraUpdFreq.NoValue)
		UpdInt = tv.UpdFreq;

	if (tv.ExtentMode != paraExtentMode.NoValue)
	{
		ExtentMode = tv.ExtentMode;
		//reset all params
		if (tv.Min == paraMin.NoValue) tv.Min = pMin;
		if (tv.Max == paraMax.NoValue) tv.Max = pMax;
		if (tv.Center == paraCenter.NoValue) tv.Center = pCenter;
		if (tv.Ampl == paraAmpl.NoValue) tv.Ampl = (pAmpl / 2) + 0x8000;
	}

	if (tv.Min != paraMin.NoValue)
	{
		pMin = tv.Min;
		if (ExtentMode == 0) //min/max
		{
			if (pmi->InertiaActive && !pmi->Initialising)
			{
				MinTarget = tv.Min;
				MinStep = float(MinTarget - Min) / float(pmi->SlideLength);
				MinSliding = true;
			}
			else
			{
				Min = tv.Min;
				MinSliding = false;
				UpdateMinMax();
			}
		}
	}

	if (tv.Max != paraMax.NoValue)
	{
		pMax = tv.Max;
		if (ExtentMode == 0) //min/max
		{
			if (pmi->InertiaActive && !pmi->Initialising)
			{
				MaxTarget = tv.Max;
				MaxStep = float(MaxTarget - Max) / float(pmi->SlideLength);
				MaxSliding = true;
			}
			else
			{
				Max = tv.Max;
				MaxSliding = false;
				UpdateMinMax();
			}
		}
	}

	bool CenterAmplUpdatesMinMax = false;

	if (tv.Center != paraCenter.NoValue)
	{
		pCenter = tv.Center;

		if (ExtentMode == 1) CenterAmplUpdatesMinMax = true;
	}

	if (tv.Ampl != paraAmpl.NoValue)
	{
		pAmpl = (tv.Ampl - 0x8000) * 2;

		if (ExtentMode == 1) CenterAmplUpdatesMinMax = true;
	}

	if (CenterAmplUpdatesMinMax)
	{
		int tmin = pCenter - pAmpl;
		int tmax = pCenter + pAmpl;
/*		if (tmin < paraMin.MinValue) tmin = paraMin.MinValue;
		if (tmin > paraMin.MaxValue) tmin = paraMin.MaxValue;
		if (tmax < paraMax.MinValue) tmax = paraMax.MinValue;
		if (tmax > paraMax.MaxValue) tmax = paraMax.MaxValue;
*/
		if (pmi->InertiaActive && !pmi->Initialising)
		{
			MinTarget = tmin;
			MinStep = float(MinTarget - Min) / float(pmi->SlideLength);
			MinSliding = true;
			MaxTarget = tmax;
			MaxStep = float(MaxTarget - Max) / float(pmi->SlideLength);
			MaxSliding = true;
		}
		else
		{
			Min = tmin;
			MinSliding = false;
			Max = tmax;
			MaxSliding = false;
			UpdateMinMax();
		}
	}

	if (tv.TrackPhase != paraTrackPhase.NoValue)
	{
		PhaseOffset = (tv.TrackPhase - 0x8000) / 65534.0f * TWICEPI;
	}

	if (tv.Track != paraTrack.NoValue)
	{
		if (tv.Track == 0xFE) //All tracks
			BusTrack = -1;
		else
			BusTrack = tv.Track;
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Tick() 
//Machine-level tick stuff
{
	Reset = false;

	if (gval.Inertia != paraInertia.NoValue)
	{
		if (gval.Inertia == 0) //Off
		{
			InertiaActive = false;
			
			if (PeriodSliding)
			{
				PeriodCurrent = PeriodTarget;
				PeriodSliding = false;
				UPDATE_PHASESTEP;
			}

			if (GlobalAmpSliding)
			{
				GlobalAmpCurrent = GlobalAmpTarget;
				GlobalAmpSliding = false;
				for (int t=0; t < numTracks; t++)
					Tracks[t].UpdateMinMax();
			}

			for (int t=0; t < numTracks; t++)
				Tracks[t].StopInertia();

		}
		else
		{
			SlideLength = pMasterInfo->SamplesPerTick * gval.Inertia / 10;
			InertiaActive = true;
		}
	}

	if (gval.OnOff != paraOnOff.NoValue)
	{
		Playing = (gval.OnOff == SWITCH_ON);

		for (int t=0; t < numTracks; t++)
			Tracks[t].UpdCount = 100; //force update
	}

	if (gval.GlobalAmp != paraGlobalAmp.NoValue)
	{		
		if (InertiaActive && !Initialising)
		{
			GlobalAmpTarget = float(gval.GlobalAmp) / 32768.0f;
			GlobalAmpStep = float(GlobalAmpTarget - GlobalAmpCurrent) / float(SlideLength);
			GlobalAmpSliding = true;
		}
		else
		{
			GlobalAmpCurrent = float(gval.GlobalAmp) / 32768.0f;
			GlobalAmpSliding = false;
			for (int t=0; t < numTracks; t++)
				Tracks[t].UpdateMinMax();

		}
	}

	if (gval.Phase != paraPhase.NoValue)
	{
		Phase = float(gval.Phase) / 32768.0f * TWICEPI;
		if (gval.Phase == 0)
			Reset = true;
	}

	if (gval.Shape != paraShape.NoValue)
		Shape = gval.Shape;

	if (gval.WaveNo != paraWaveNo.NoValue)
		WaveNo = gval.WaveNo;

	if (gval.MinDiff != paraMinDiff.NoValue)
		MinDiff = gval.MinDiff / 65536.0f;

	if (gval.PeriodUnit != paraPeriodUnit.NoValue)
	{
		PeriodUnit = gval.PeriodUnit;
	}

	if (gval.Period != paraPeriod.NoValue)
	{
		int ThePeriod;
		switch (PeriodUnit)
		{
		case 0: //tick/16
			//ThePeriod = pMasterInfo->SamplesPerTick * gval.Period / 16;
			ThePeriod = gval.Period;
			break;
		case 1: //ms
			ThePeriod = pMasterInfo->SamplesPerSec * gval.Period / 1000;
			break;
		case 2: //Hz/256
			ThePeriod = pMasterInfo->SamplesPerSec * 4096 / gval.Period;
			break;
		default:
			ThePeriod = 44100;
		}

		if (InertiaActive && !Initialising)
		{
			PeriodTarget = float(ThePeriod);
			PeriodStep = float(PeriodTarget - PeriodCurrent) / float(SlideLength);
			PeriodSliding = true;
		}
		else
		{
			PeriodCurrent = float(ThePeriod);
			PeriodSliding = false;
			UPDATE_PHASESTEP;
		}
	}

	if (InertiaActive) UpdateInertia();

	for (int t=0; t < numTracks; t++)
	{
		Tracks[t].Tick(tval[t]);
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Stop()
//Stop the machine (if we have to) when the Stop button is clicked
{
	if (aval.HeedStopBtn == 1) 
	{
		Playing = false;
		pCB->ControlChange(ThisMac, 1, 0, 0, SWITCH_OFF); //Switch param off
	}
}

////////////////////////////////////////////////////////////////////////

HINSTANCE dllInstance;
mi *g_mi;
CTrack *g_track;
int g_tracknum;

BOOL WINAPI DllMain ( HANDLE hModule, DWORD fwdreason, LPVOID lpReserved )
//GUI code from Cyanphase's "small" guide to making Buzz effects
{
   switch (fwdreason) {
   case DLL_PROCESS_ATTACH:
      dllInstance = (HINSTANCE) hModule;
      break;

   case DLL_THREAD_ATTACH: break;
   case DLL_THREAD_DETACH: break;
   case DLL_PROCESS_DETACH: break;
   }
   return TRUE;
}

////////////////////////////////////////////////////////////////////////

bool CTrack::DoEndAssignDlg (HWND hDlg, bool ShouldSave)
//Store the parameters chosen (if any) in the dialog, or not
//Return value: true if should close dialog, false if not
{
	if (ShouldSave)
	{	
		char MacName[128], ParaName[128];

		m_ctrl.UnassignParameter();

		if (GetDlgItemText(hDlg, IDC_MACHINECOMBO, MacName, 128) != 0)
		{
			int paramn = SendDlgItemMessage(hDlg, IDC_PARALIST, LB_GETCURSEL, 0, 0);
			if (paramn != LB_ERR)
			{
				if (LB_ERR != SendDlgItemMessage(hDlg, IDC_PARALIST,
					LB_GETTEXT, paramn, (LPARAM)ParaName))
				{
					m_ctrl.AssignParameter(MacName, ParaName);
				}
			}

			UpdateMinMax();
		}

		return true;
	}
	else //shouldn't save
	{
		return true;
	}

}

////////////////////////////////////////////////////////////////////////

BOOL APIENTRY AssignDialog(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) 
//The big procedure for the GUI dialog
//Loosely based on more code by Cyanphase
{
	switch(uMsg) {
	case WM_INITDIALOG:
		char txt[128];
		_snprintf(txt, 128, "%s, track %i (current assign: %s)",
			g_mi->pCB->GetMachineName(g_mi->ThisMac),
			g_tracknum,
			g_track->m_ctrl.GetAssignmentString());

		SetDlgItemText(hDlg, IDC_THISMACINFO, txt);

		//Populate the boxes
		g_track->m_ctrl.GetMachineNamesToCombo(hDlg, IDC_MACHINECOMBO, "");

		if (g_track->m_ctrl.GotParam())
		{
			g_track->m_ctrl.SelectMachineInCombo(hDlg, IDC_MACHINECOMBO);

			//update + populate param list
			AssignDialog (hDlg, WM_COMMAND, 
				IDC_MACHINECOMBO | (CBN_SELCHANGE << 16) , 0);
				
			//select param
			g_track->m_ctrl.SelectParameterInList(hDlg, IDC_PARALIST);
		}
	return 1;

	case WM_SHOWWINDOW: 
		return 1;

	case WM_CLOSE:
		if (g_track->DoEndAssignDlg(hDlg, false))
			EndDialog (hDlg, TRUE);
		return 0;

	case WM_COMMAND:
		//If a button or other control is clicked or something
		switch (LOWORD (wParam))
		{
		case IDOK:
			if (g_track->DoEndAssignDlg(hDlg, true))
				EndDialog(hDlg, TRUE);
			return 1;

		case IDCANCEL:
			if (g_track->DoEndAssignDlg(hDlg, false))
				EndDialog(hDlg, TRUE);
			return 1;

		case IDC_MACHINECOMBO:
			if (HIWORD(wParam) == CBN_SELCHANGE) //selection is changed
			{
				char MacName[128];
				
				if (GetDlgItemText(hDlg, IDC_MACHINECOMBO, MacName, 128) != 0)
				{ //ie if a machine is selected
					g_track->m_ctrl.GetParamNamesToList(
						g_mi->pCB->GetMachine(MacName), 
						hDlg, IDC_PARALIST, 
						ALLOW_ALL_GROUPS | ALLOW_ALL_TYPES);
				}
			}

			return 1;

		case IDC_CLEARBTN:
			g_track->m_ctrl.GetMachineNamesToCombo(hDlg, IDC_MACHINECOMBO, "");
			SendMessage(GetDlgItem(hDlg, IDC_PARALIST), LB_RESETCONTENT, 0, 0);
			return 1;
			
		default:
			return 0;
		}
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////

void mi::DoAssignPar(int TrackNum)
//Display the dialog
{
	g_mi = this;
	g_track = &Tracks[TrackNum];
	g_tracknum = TrackNum;

	DialogBox(dllInstance, 
		MAKEINTRESOURCE (IDD_ASSIGNPAR), 
		GetForegroundWindow(), 
		(DLGPROC) &AssignDialog);
}

////////////////////////////////////////////////////////////////////////

void mi::ImportFinished()
{
	CPeerCtrl::OnImportFinished();

	Initialising = false;
}

////////////////////////////////////////////////////////////////////////

void mi::GetSubMenu(const int i, CMachineDataOutput *pout)
//Populate submenus (with all current tracks/assignments)
{
	for (int t=0; t < numTracks; t++)
	{
		char txt[128];
		_snprintf(txt, 128, "%i: %s", t, Tracks[t].m_ctrl.GetAssignmentString());
		pout->Write(txt);
	}

	if (i != 0) //ie not assign
		pout->Write("All");
}

////////////////////////////////////////////////////////////////////////

void CTrack::CA2MM()
//Send center/amp values to min/max
{
	int tmin = pCenter - pAmpl;
	int tmax = pCenter + pAmpl;
	bool DoesClip = false;

	if (tmin < paraMin.MinValue) {tmin = paraMin.MinValue; DoesClip = true;}
	if (tmin > paraMin.MaxValue) {tmin = paraMin.MaxValue; DoesClip = true;}
	if (tmax < paraMax.MinValue) {tmax = paraMax.MinValue; DoesClip = true;}
	if (tmax > paraMax.MaxValue) {tmax = paraMax.MaxValue; DoesClip = true;}

	//I could add a warning for DoesClip here...

	pmi->pCB->ControlChange(pmi->ThisMac, 2, TrackNumber, 0, tmin);
	pmi->pCB->ControlChange(pmi->ThisMac, 2, TrackNumber, 1, tmax);
}

////////////////////////////////////////////////////////////////////////

void CTrack::MM2CA()
//Send min/max values to center/amp
{
	int tcent = (pMax + pMin) / 2;
	int tampl = (pMax - pMin) / 4 + 0x8000;

	pmi->pCB->ControlChange(pmi->ThisMac, 2, TrackNumber, 2, tcent);
	pmi->pCB->ControlChange(pmi->ThisMac, 2, TrackNumber, 3, tampl);
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i) 
//Handler for context menu items
{
	if (i == 3) // PAL
		CPeerCtrl::ShowPAL();

	if (i == 4) //About dlg
	{
		MessageBox(NULL,
			"Peer LFO v1.3b\n"
			"©2002-2015 Ed Powley / Phil Considine\n"
			"http://btdsys.sourceforge.net/\n"
			"Please read the help file for usage instructions",
			"Peer LFO",
			MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
	}

	if ((i >= 256) && (i < 512)) //Submenu of assign parameter
	{
		DoAssignPar(i - 256);
	}

	if ((i >= 512) && (i < 768)) //Submenu of CA2MM
	{
		if ((i - 512) == numTracks) //All
		{
			for (int t=0; t < numTracks; t++)
				Tracks[t].CA2MM();
		}
		else
			Tracks[i - 512].CA2MM();
	}

	if ((i >= 768) && (i < 1024)) //Submenu of MM2CA
	{
		if ((i - 768) == numTracks) //All
		{
			for (int t=0; t < numTracks; t++)
				Tracks[t].MM2CA();
		}
		else
			Tracks[i - 768].MM2CA();
	}
}

////////////////////////////////////////////////////////////////////////
char const *mi::DescribeValue(const int param, const int value)
//Describe value!
{
	static char txt[64];

	switch (param)
	{
	case 5: //MinDiff
	case 9: //Min
	case 10: //Max
	case 11: //Center
		_snprintf(txt, 64, "%.2f%%", value / 655.34f);
		return txt;
		break;

	case 12: //Ampl
		_snprintf(txt, 64, "%.2f%%", value / 327.67f - 100.0f);
		return txt;
		break;

	case 13: //ExtentMode
		switch (value)
		{
		case 0: return "Min/Max";		break;
		case 1: return "Center/Ampl";	break;
		default:return "Unknown";
		}
		break;

	case 14: //UpdFreq
		if(value > 0)
		{
			_snprintf(txt, 64, "%i tick%s", value, (value==1) ? "" : "s" );
			return txt;
		}
		else
		{
			return "Free"; // Free == use subticks if enabled or 1 tick
		}
		break;

	case 15: //TrackPhase
		_snprintf(txt, 64, "%.2f%%", value / 655.34f - 50.0f);
		return txt;
		break;

	case 16: //Track
		if (value == 0xFE) return "All";
		else
		{
			_snprintf(txt, 64, "%i", value);
			return txt;
		}
		break;
	
	case 0: //On/Off
		if (value == SWITCH_OFF) return "Off";
		else return "On";
		break;

	case 1: //GlobalAmp
	case 2: //Phase
		_snprintf(txt, 64, "%.1f%%", value / 327.68f);
		return txt;
		break;

	case 3: //Shape
		switch (value)
		{
		case 0: return "Sine";		break;
		case 1: return "Square";	break;
		case 2: return "Tri";		break;
		case 3:	return "Ramp Up";	break;
		case 4: return "Ramp Dn";	break;
		case 5: return "Random";	break;
		case 6: return "Wander";	break;
		case 7: return "Wavetable";	break;
		case 8: return "Min";		break;
		case 9: return "Max";		break;
		case 10:return "Center";	break;
		default:return "Unknown";	break;
		}
		break;

	case 4: //WaveNo
		_snprintf(txt, 64, "%i %s", value, pCB->GetWaveName(value));
		return txt;
		break;

	case 6: //Period
		switch (PeriodUnit)
		{
		case 0: //Tick/16
			_snprintf(txt, 64, "%i %i/16", int(value / 16), value % 16);
			break;
		case 1: //ms
			_snprintf(txt, 64, "%i ms", value);
			break;
		case 2: //Hz/256
			_snprintf(txt, 64, "%.2f Hz", value / 4096.0f);
			break;
		default:
			_snprintf(txt, 64, "Unknown");
		}
		return txt;
		break;

	case 7: //PeriodUnit
		switch (value)
		{
		case 0: return "Tick/16";	break;
		case 1: return "ms";		break;
		case 2: return "Hz/4096";	break;
		default:return "Unknown";	break;
		}
		break;

	case 8: //Inertia
		if (value == 0) return "Off";
		else
		{
			_snprintf(txt, 64, "%.1f ticks", float(value)/10.0f);
			return txt;
		}
		break;

	default:
		return NULL;
		break;
	}
}

////////////////////////////////////////////////////////////////////////

#pragma optimize ("", on)

