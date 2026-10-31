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

#ifndef __BTDSYS_PEERADSR_MI_CTRACK_DEFS
#define __BTDSYS_PEERADSR_MI_CTRACK_DEFS

#include "Resource.h"
#include "../mdk/MachineInterface.h"
#include "../PeerLib2/peerctrl.h"
#include "params.h"
#include "CustomEnvDlg.h"

class mi;

enum {epA, epD, epS, epR, epOff};

class CTrack
{
public:
	void Init();
	void Tick(tvals *tv);
	void UpdateEnv(int numsamples);
	inline int FloatToPVal(float fval);
	void Trigger(byte TrigVal);

	void DoEndAssignDlg (HWND hDlg, CTrack *g_track);

	inline void EnterA();
	inline void EnterD();
	inline void EnterS();
	inline void EnterR();

	mi *pmi;
	CPeerCtrl PeerCtrl;

	int index;

	float fmin, fmax;

	int EnvType, EnvNum;
	float envA, envD, envS, envSLev, envR;
	float envLength;

	int lenA, lenD, lenS, lenR, len;
	int EnvCountdown;
	int EnvPhase;
	bool Enveloping;
	bool JustStoppedEnveloping;

	float EnvPos;
	float EnvPosStep;

	float ValueCurrent, ValueStep;

	int Track;
};

////////////////////////////////////////////////////////////////////////

#include "CEnvData.h"

class mi : public CMachineInterface, CMachineInterfaceEx
{
public:

	mi();
	virtual ~mi();

public: // CMachineInterface overrides

	virtual void Command(const int i);
	virtual char const * DescribeValue(int const param, int const value);
	virtual void Init(CMachineDataInput * const pi);
	virtual void MidiNote(const int channel, const int value, const int velocity);
	virtual void Save(CMachineDataOutput * const po);
	virtual void SetNumTracks(int const n) { numTracks = n; }
	virtual void Tick();
	virtual bool Work(float *psamples, int numsamples, int const mode);

public: // CMachineInterfaceEx overrides

	virtual void ImportFinished();
	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);
	virtual void SetDeletedState(bool deleted);

public: // mi stuff

	virtual void DoAssignPar(int TrackNum);
	virtual float GetEnvPoint(CEnvData *env, float pos);
	virtual int GetEnvPointNumber(CEnvData *env, float pos);
	float LengthValToTicks (int val);

	bool OnRenameMachine(void *pmac);

public:
	gvals gval;
	tvals tval[MAX_TRACKS];
	avals aval;

	CMachine *ThisMac;

	CCustomEnvDlg *envdlg;

	CTrack Tracks[MAX_TRACKS];
	int numTracks;

	CEnvData CustomEnv[NUM_CUSTOM_ENVS];

	bool Initialising;
	bool JustTicked;

	int LastMidiNote;
};

#endif
