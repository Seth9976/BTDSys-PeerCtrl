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

/********************************************************
**                B T D   S Y S T E M S                **
**                =====================                **
** PeerEnv										       **
** ©2002 Edward Powley                                 **
*********************************************************
** Created on 14-Jul-02                                **
********************************************************/

#include <windows.h>
#include "../mdk/MachineInterface.h"
#include <float.h>
#include <stdlib.h>
#include "resource.h"
#include <math.h>
#include "../PeerLib2/peerctrl.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

#include "params.h"

////////////////////////////////////////////////////////////////////////

#define MAX_TRACKS	255

CMachineInfo const MacInfo =
{
	MT_EFFECT,						//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT|MIF_STEREO_EFFECT|MIF_CONTROL_MACHINE|MIF_DOES_INPUT_MIXING,					//Flags
	1,								//Min tracks
	MAX_TRACKS,						//Max tracks
	NUMG,							//Number of global parameters
	NUMT,							//Number of track parameters
	pParameters,					//Pointer to parameters
	NUMA,							//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys PeerEnv",				//Full name
	"PeerEnv",						//Short name
	"Edward Powley (BTDSys)",		//Author
	"/Assign Parameter\n"
	"/Copy Center/Ampl To Min/Max\n"
	"/Copy Min/Max To Center/Ampl\n"	
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

	inline void DoBus(float CurrentValue, bool Immediate);
	bool DoEndAssignDlg (HWND hDlg, bool ShouldSave);
	void CA2MM();
	void MM2CA();

	mi *pmi;
	int TrackNumber;

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

	MyPeerCtrl m_ctrl;
};

void MyPeerCtrl::OnChangeAssignment(PeerEvent trigger)
{
	m_track->UpdateMinMax();
}

#pragma pack()

////////////////////////////////////////////////////////////////////////

class mi : public CMachineInterface, public CMachineInterfaceEx
{
public:
	mi();
	virtual ~mi();

	virtual void DoWorkNoSend(float *psamples, int numsamples, const int mode);
	virtual void DoWorkSend(float *psamples, int numsamples, const int mode);
	virtual void DoWorkSubtickTiming(float *psamples, int numsamples, const int mode);
	virtual void DoAssignPar (int TrackNum);
	virtual void UpdateInertia();
	inline virtual void DoBus(bool Immediate);

public: // CMachineInterface
	virtual void AttributesChanged();
	virtual void Command(int const i);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void Init(CMachineDataInput * const pi);
	virtual void Tick();
	virtual void Save(CMachineDataOutput * const po);
	virtual void SetNumTracks(int const n) { numTracks = n; }
	virtual void Stop();
	virtual bool Work(float *psamples, int numsamples, int const mode);;

public: // CMachineInterfaceEx
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

	bool InertiaActive;
	int SlideLength;

	bool Initialising;

	float CurrentValue;

	byte Mode;
	byte GetFrom;

	float AvgSum;
	int AvgCount;
	float EnvAttack, EnvRelease;

	int SendFreq;
	int SendCounter;
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
{ 
	GlobalVals = &gval;
	TrackVals = &tval;
	AttrVals = (int*) &aval;

	Initialising = true;

	SendCounter = 0;
}

mi::~mi() 
{
}

////////////////////////////////////////////////////////////////////////

void CTrack::Reset()
//Initialise track-related values
{
	m_ctrl.m_track = this;
	m_ctrl.Init(pmi->ThisMac, pmi);
	m_ctrl.SetLabelAsTrackNumber(TrackNumber);

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
}

////////////////////////////////////////////////////////////////////////

void mi::Init (CMachineDataInput * const pi)
//Initialise machine and load data from file
{
	ThisMac = pCB->GetThisMachine();
	pCB->SetMachineInterfaceEx(this);

	for (int c=0; c < MAX_TRACKS; c++)
	{
		Tracks[c].pmi = this;
		Tracks[c].TrackNumber = c;
		Tracks[c].m_ctrl.Init(ThisMac, this);
		Tracks[c].Reset();
	}

	CPeerCtrl::SetEventHandlers(pCB, ALL_EVENT_HANDLERS);

	CurrentValue = -1;

	if (pi != NULL) //Read data from file
	{
		byte dummy; pi->Read(dummy); // Fake MDK version byte

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
					char macname[128];
					pi->ReadString(macname, 128);
					int param;
					pi->Read(param);
					int group;
					pi->Read(group);

					Tracks[t].m_ctrl.PushFileData(macname, param, group);
				}
			}
		}
		else if(Version == 3)
		{
			byte NumTracks;
			pi->Read(NumTracks);

			for (int t=0; t < NumTracks; t++)
			{
				Tracks[t].m_ctrl.ReadFileData(pi);
			}
		}
		else
		{
			MessageBox (NULL, "Incompatible data", "BTDSys PeerEnv", MB_OK|MB_SYSTEMMODAL);
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Save (CMachineDataOutput * const po)
{
	po->Write((byte)0); // Fake MDK version byte

	po->Write(byte(3));	//version
	po->Write(byte(numTracks));
	for (int t=0; t < numTracks; t++)
	{
		Tracks[t].m_ctrl.WriteFileData(po, true);
		/*
		if (Tracks[t].HavePar)
		{
			po->Write(byte(0xFF)); // got par
			po->Write (pCB->GetMachineName (Tracks[t].BusMachine));
			po->Write(Tracks[t].BusParNo);
			po->Write(Tracks[t].BusGroup);
		}
		else
			po->Write(byte(0));
		*/
	}
}

////////////////////////////////////////////////////////////////////////

void mi::DoWorkNoSend(float *psamples, int numsamples, const int mode)
{
	if (mode & WM_READ) //have input
	{
		float t;

		switch (Mode)
		{
		case 0: //Immediate
			switch (GetFrom)
			{
			case 0: //Left
				t = psamples[numsamples*2-2];
				break;
			case 2: //Right
				t = psamples[numsamples*2-1];
				break;
			case 1: //Center
				t = (psamples[numsamples*2-2] + psamples[numsamples*2-1])
					/ 2.0f;
				break;
			default:
				t = 0;
			}
			CurrentValue = t / 65536.0f + 0.5f;
			break;

		case 1: //Absolute
			switch (GetFrom)
			{
			case 0: //Left
				t = fabsf(psamples[numsamples*2-2]);
				break;
			case 2: //Right
				t = fabsf(psamples[numsamples*2-1]);
				break;
			case 1: //Center
				t = fabsf(psamples[numsamples*2-2] + psamples[numsamples*2-1])
					/ 2.0f;
				break;
			default:
				t = 0;
			}
			CurrentValue = t / 32768.0f;
			break;

		case 2: //Envelope
			{
/////////////////////////////////////////////////////////////////////
#define THELOOP(INTERM)												\
				for (i=0; i < numsamples*2; i+=2)					\
				{													\
					float EnvIn = (INTERM);							\
					if(CurrentValue < EnvIn)						\
					{												\
						CurrentValue *= EnvAttack;					\
						CurrentValue += (1.0f - EnvAttack) * EnvIn;	\
					}												\
					else											\
					{												\
						CurrentValue *= EnvRelease;					\
						CurrentValue += (1.0f - EnvRelease) * EnvIn;\
					}												\
				}													\
// end define ///////////////////////////////////////////////////////

				if (CurrentValue == -1) CurrentValue = 0.0f;

				int i;

				switch(GetFrom)
				{
				case 0: //Left
					THELOOP(fabsf(psamples[i]) / 32768.0f);
					break;

				case 2: //Right
					THELOOP(fabsf(psamples[i+1]) / 32768.0f);
					break;

				case 1: //Center
					THELOOP((fabsf(psamples[i]) + fabsf(psamples[i+1])) / 65536.0f);
					break;
				}
#undef THELOOP
			}
			break;

		default:
			CurrentValue = -1;
		}
	}
	else
		CurrentValue = -1;
}


void mi::DoWorkSend(float *psamples, int numsamples, const int mode)
{
	if (mode & WM_READ) //have input
	{
		switch (Mode)
		{
		case 0: //Immediate
			{
/////////////////////////////////////////////////////////////////////
#define THELOOP0(INTERM)											\
				for (i=0; i < numsamples*2; i+=2)					\
				{													\
					SendCounter++;									\
					if (SendCounter >= SendFreq)					\
					{												\
						SendCounter = 0;							\
						CurrentValue = (INTERM) / 65536.0f + 0.5f;	\
						DoBus(true);								\
					}												\
				}
//end define ////////////////////////////////////////////////////////

				int i;

				switch(GetFrom)
				{
				case 0: //Left
					THELOOP0(psamples[i]);
					break;
				case 2: //Right
					THELOOP0(psamples[i+1]);
					break;
				case 1: //Center
					THELOOP0((psamples[i] + psamples[i+1]) / 2.0f);
					break;
				}
#undef THELOOP0
			}
			break;

		case 1: //Absolute
			{
/////////////////////////////////////////////////////////////////////
#define THELOOP1(INTERM)											\
				for (i=0; i < numsamples*2; i+=2)					\
				{													\
					SendCounter++;									\
					if (SendCounter >= SendFreq)					\
					{												\
						SendCounter = 0;							\
						CurrentValue = (INTERM) / 32768.0f;			\
						DoBus(true);								\
					}												\
				}
//end define ////////////////////////////////////////////////////////

				int i;

				switch(GetFrom)
				{
				case 0: //Left
					THELOOP1(psamples[i]);
					break;
				case 2: //Right
					THELOOP1(psamples[i+1]);
					break;
				case 1: //Center
					THELOOP1((psamples[i] + psamples[i+1]) / 2.0f);
					break;
				}
#undef THELOOP1
			}
			break;

		case 2: //Envelope
			{
/////////////////////////////////////////////////////////////////////
#define THELOOP2(INTERM)											\
				for (i=0; i < numsamples*2; i+=2)					\
				{													\
					float EnvIn = (INTERM);							\
					if(CurrentValue < EnvIn)						\
					{												\
						CurrentValue *= EnvAttack;					\
						CurrentValue += (1.0f - EnvAttack) * EnvIn;	\
					}												\
					else											\
					{												\
						CurrentValue *= EnvRelease;					\
						CurrentValue += (1.0f - EnvRelease) * EnvIn;\
					}												\
																	\
					SendCounter++;									\
					if (SendCounter >= SendFreq)					\
					{												\
						SendCounter = 0;							\
						DoBus(true);								\
					}												\
				}
//end define ////////////////////////////////////////////////////////

				if (CurrentValue == -1) CurrentValue = 0.0f;

				int i;

				switch(GetFrom)
				{
				case 0: //Left
					THELOOP2(fabsf(psamples[i]) / 32768.0f);
					break;
				case 2: //Right
					THELOOP2(fabsf(psamples[i+1]) / 32768.0f);
					break;
				case 1: //Center
					THELOOP2((fabsf(psamples[i]) + fabsf(psamples[i+1])) / 65536.0f);
					break;
				}
#undef THELOOP2
			}
			break;
		default:
			CurrentValue = -1;
		}
	}
	else
		CurrentValue = -1;
}

void mi::DoWorkSubtickTiming(float *psamples, int numsamples, const int mode)
// Copied from DoWorkNoSend()
// Think we may need to average the input to smooth out the envelope
// Maybe new attribute for window size?
{
	if (mode & WM_READ) //have input
	{
		float t;

		switch (Mode)
		{
		case 0: //Immediate
			switch (GetFrom)
			{
			case 0: //Left
				t = psamples[numsamples*2-2];
				break;
			case 2: //Right
				t = psamples[numsamples*2-1];
				break;
			case 1: //Center
				t = (psamples[numsamples*2-2] + psamples[numsamples*2-1])
					/ 2.0f;
				break;
			default:
				t = 0;
			}
			CurrentValue = t / 65536.0f + 0.5f;
			break;

		case 1: //Absolute
			switch (GetFrom)
			{
			case 0: //Left
				t = fabsf(psamples[numsamples*2-2]);
				break;
			case 2: //Right
				t = fabsf(psamples[numsamples*2-1]);
				break;
			case 1: //Center
				t = fabsf(psamples[numsamples*2-2] + psamples[numsamples*2-1])
					/ 2.0f;
				break;
			default:
				t = 0;
			}
			CurrentValue = t / 32768.0f;
			break;

		case 2: //Envelope
			{
/////////////////////////////////////////////////////////////////////
#define THELOOP(INTERM)												\
				for (i=0; i < numsamples*2; i+=2)					\
				{													\
					float EnvIn = (INTERM);							\
					if(CurrentValue < EnvIn)						\
					{												\
						CurrentValue *= EnvAttack;					\
						CurrentValue += (1.0f - EnvAttack) * EnvIn;	\
					}												\
					else											\
					{												\
						CurrentValue *= EnvRelease;					\
						CurrentValue += (1.0f - EnvRelease) * EnvIn;\
					}												\
				}													\
// end define ///////////////////////////////////////////////////////

				if (CurrentValue == -1) CurrentValue = 0.0f;

				int i;

				switch(GetFrom)
				{
				case 0: //Left
					THELOOP(fabsf(psamples[i]) / 32768.0f);
					break;

				case 2: //Right
					THELOOP(fabsf(psamples[i+1]) / 32768.0f);
					break;

				case 1: //Center
					//THELOOP((fabsf(psamples[i]) + fabsf(psamples[i+1])) / 65536.0f);
					{
						for (i=0; i < numsamples*2; i+=2)
						{
							float EnvIn = (fabsf(psamples[i]) + fabsf(psamples[i+1])) / 65536.0f;
							if(CurrentValue < EnvIn)
							{
								CurrentValue *= EnvAttack;
								CurrentValue += (1.0f - EnvAttack) * EnvIn;
							}
							else
							{
								CurrentValue *= EnvRelease;
								CurrentValue += (1.0f - EnvRelease) * EnvIn;
							}
						}	
					}
					break;
				}
#undef THELOOP
			}
			break;

		default:
			CurrentValue = -1;
		}
	}
	else
		CurrentValue = 0.0f;//-1;

	// FIXME: static for testing
	static float buffer[44] = {0,0,0,0,0};
	int bufsize = sizeof(buffer);
	int movesize = bufsize - sizeof(float);
	memmove(&buffer[1], &buffer[0], movesize);
	buffer[0] = CurrentValue;
	float average = 0;
	for(int b = 0; b < 44; b++)
	{
		average += buffer[b];
	}
	average = average / 44;
	if(pCB->GetSubTickInfo()->PosInSubTick == 0)
	{
		for (int t=0; t < numTracks; t++)
		{
			if (Tracks[t].m_ctrl.GotParam())
			{
				Tracks[t].DoBus(average, true);
			}
		}
	}
}

bool mi::Work(float *psamples, int numsamples, const int mode)
//This is the most sensible place to set Initialising to false, since it
//(probably) won't be called until all machines are present and correct.
{
	Initialising = false;

	CSubTickInfo const *pInfo = pCB->GetSubTickInfo();

	if(pInfo) // Subtick timing on
	{
		DoWorkSubtickTiming(psamples, numsamples, mode);
	}
	else // Subtick timing off
	{
		if (SendFreq == 0)
			DoWorkNoSend(psamples, numsamples, mode); // Once per tick
		else
			DoWorkSend(psamples, numsamples, mode); // User defined subticks
	}

	return false;
}


////////////////////////////////////////////////////////////////////////

void CTrack::UpdateMinMax()
//If we need to update min and max values for parameter
{
	if (m_ctrl.GotParam())
	{
		CMachineParameter const * pInfo = m_ctrl.GetParamInfo();

		if (pInfo->Type != pt_switch)
		{
			BusMin = int(Min / 65534.0f
						* float(pInfo->MaxValue - pInfo->MinValue + 1))
						+ pInfo->MinValue;
			BusMax = int(Max / 65534.0f
						* float(pInfo->MaxValue - pInfo->MinValue + 1))
						+ pInfo->MinValue;
		}
		else
		{
			BusMin = int(Min / 32768.0f);
			BusMax = int(Max / 32768.0f);
		}
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

void CTrack::DoBus(float CurrentValue, bool Immediate)
//Track-level Bus stuff: sending values
{
	if (m_ctrl.GotParam() && (CurrentValue != -1))
	{
		int pval = int(
			CurrentValue * float(BusMax - BusMin + 1))
			+ BusMin;

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

		CMachineParameter const *pInfo = m_ctrl.GetParamInfo();

		if (pval < pInfo->MinValue) pval = pInfo->MinValue;
		if (pval > pInfo->MaxValue) pval = pInfo->MaxValue;

		if(Immediate)
		{
			if (m_ctrl.GetGroup() == 1) //Global
			{
				m_ctrl.ControlChange_Immediate(pval);
			}
			else //Track
			{
				if (BusTrack != -1) //Not all tracks
				{
					m_ctrl.ControlChange_Immediate(BusTrack, pval);
				}
				else //All tracks
				{
					for (int t=0; t < m_ctrl.GetMInfo()->maxTracks; t++)
					{
						m_ctrl.ControlChange_Immediate(t, pval);
					}
				}
			}
		}
		else
		{
			if (m_ctrl.GetGroup() == 1) //Global
			{
				m_ctrl.ControlChange_NextTick(pval);
			}
			else //Track
			{
				if (BusTrack != -1) //Not all tracks
				{
					m_ctrl.ControlChange_NextTick(BusTrack, pval);
				}
				else //All tracks
				{
					for (int t=0; t < m_ctrl.GetMInfo()->maxTracks; t++)
					{
						m_ctrl.ControlChange_NextTick(t, pval);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::DoBus(bool Immediate)
//Machine-level Bus stuff: choosing a value
{
	if (!Initialising) //Do the actual stuff
	{
		for (int t=0; t < numTracks; t++)
		{
			if (Tracks[t].m_ctrl.GotParam())
			{
				Tracks[t].DoBus(CurrentValue, Immediate);
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////

void CTrack::Tick(tvals &tv)
//Track-level tick stuff
{
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

	if (tv.Track != paraTrack.NoValue)
	{
		BusTrack = (tv.Track == 0xFE) ? -1 : tv.Track;
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Tick() 
//Machine-level tick stuff
{
	if (gval.Inertia != paraInertia.NoValue)
	{
		if (gval.Inertia == 0) //Off
		{
			InertiaActive = false;
			
			for (int t=0; t < numTracks; t++)
				Tracks[t].StopInertia();

		}
		else
		{
			SlideLength = pMasterInfo->SamplesPerTick * gval.Inertia / 10;
			InertiaActive = true;
		}
	}

	if (gval.Mode != paraMode.NoValue)
	{
		Mode = gval.Mode;
	}

	if (gval.GetFrom != paraGetFrom.NoValue)
	{
		GetFrom = gval.GetFrom;
	}

	if (InertiaActive) UpdateInertia();

	for (int t=0; t < numTracks; t++)
		Tracks[t].Tick(tval[t]);

	if(!pCB->GetSubTickInfo())
	{
		DoBus(false);
	}
	SendCounter = 0;
}

////////////////////////////////////////////////////////////////////////

void mi::AttributesChanged()
{
	SendFreq = pMasterInfo->SamplesPerTick / aval.TickSubdiv;
	EnvAttack = expf(-1 / float(aval.EnvAtt));
	EnvRelease = expf(-1 / float(aval.EnvRel));
}



void mi::Stop()
//Stop the machine (if we have to) when the Stop button is clicked
{
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
	case DLL_PROCESS_DETACH:
		break;
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
	switch(uMsg)
	{
	case WM_INITDIALOG:
		char txt[128];
		sprintf(txt, "%s, track %i (current assign:",
			g_mi->pCB->GetMachineName(g_mi->ThisMac),
			g_tracknum);

		if (g_track->m_ctrl.GotParam())
			sprintf(txt,"%s %s)",
			txt,
			g_track->m_ctrl.GetAssignmentString());
		else
			sprintf(txt,"%s None)", txt);

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
		{
			EndDialog (hDlg, TRUE);
		}
		return 0;

	case WM_COMMAND:
		//If a button or other control is clicked or something
		switch (LOWORD (wParam))
		{
		case IDOK:
			if (g_track->DoEndAssignDlg(hDlg, true))
			{
				EndDialog(hDlg, TRUE);
			}
			return 1;

		case IDCANCEL:
			if (g_track->DoEndAssignDlg(hDlg, false))
			{
				EndDialog(hDlg, TRUE);
			}
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
			break;
			
		default:
			return 0;
		}
		break;
	}

	return FALSE;
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
}

void mi::GetSubMenu(const int i, CMachineDataOutput *pout)
//Populate submenus (with all current tracks/assignments)
{
	for (int t=0; t < numTracks; t++)
	{
		char txt[128];
		sprintf(txt,"%i: %s", t, Tracks[t].m_ctrl.GetAssignmentString());
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
	if (i == 3) //About dlg
	{
		MessageBox(NULL,
			"BTDSys PeerEnv v1.1\n"
			"©2002-2011 Ed Powley (BTDSys)\n\n"
			"http://btdsys.sourceforge.net/\n"
			"Please read the help file for usage instructions",
			"BTDSys PeerEnv",
			MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION
			);		
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
	static char txt[16];

	switch (param)
	{
	case PARAMODE: //Mode
		switch (value)
		{
		case 0: return "Immediate";
		case 1: return "Absolute";
		case 2: return "Envelope";
		default:return "Unknown";
		}
		break;

	case PARAGETFROM: //GetFrom
		switch (value)
		{
		case 0: return "Left";
		case 1: return "Center";
		case 2: return "Right";
		default:return "Unknown";
		}
		break;

	case PARAINERTIA: //Inertia
		if (value == 0) return "Off";
		else
		{
			sprintf(txt, "%.1f ticks", float(value)/10.0f);
			return txt;
		}
		break;


	case PARAMIN: //Min
	case PARAMAX: //Max
	case PARACENTER: //Center
		sprintf(txt,"%.2f%%", value / 655.34f);
		return txt;
		break;

	case PARAAMPL: //Ampl
		sprintf(txt,"%.2f%%", value / 327.67f - 100.0f);
		return txt;
		break;

	case PARAEXTENTMODE: //ExtentMode
		switch (value)
		{
		case 0: return "Min/Max";		break;
		case 1: return "Center/Ampl";	break;
		default:return "Unknown";
		}
		break;

	case PARATRACK: //Track
		if (value == 0xFE) return "All";
		else
		{
			sprintf(txt,"%i",value);
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

