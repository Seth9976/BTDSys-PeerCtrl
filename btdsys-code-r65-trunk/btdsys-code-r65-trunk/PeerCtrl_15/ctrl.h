/*
ctrl.h: the main header file for this machine
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

#ifndef PEERCTRL_CTRL_H
#define PEERCTRL_CTRL_H

#include "../mdk/mdk.h"
#include "params.h"
#include "../peerlib2/peerctrl.h"
#include <vector>
#include <mmsystem.h>

#include "PluginInterface.h"

using namespace std;

#define MAX_TRACKS 255

//#define DBGCON

#ifdef DBGCON
#define LOG(t)	fprintf(stderr,t);
#else
#define LOG(t)
#endif


extern char MacInfo_MenuItems[1024];
#define BASE_MENUITEMS "Assignment Settings...\nShow PAL...\nAbout..."

/////////////////////////////////////////////////////////////////////////////

class CEnvData
{
public:
	CEnvData();
	~CEnvData();
	void Set(int NumPoints, long *Points);
	void ReadData(CMachineDataInput * const pi);
	void WriteData(CMachineDataOutput * const po);
	void CopyFrom(const CEnvData& from);

	int numpoints() {return points.size() / 2;};
	vector<long> points;
};

/////////////////////////////////////////////////////////////////////////////

class mi;
class CPlug;

struct SAxisInfo
{
	CPlug *pPl;
	int nAxis;
};

class CTrack
{
public:
	void Init();
	inline int FloatToPVal(int n, float fval);
	void UpdateInertia(int numsamples);
	void StopInertia();
	void Tick();
//	int GetParValue (float fValue);
//	void UpdateJoy(JOYINFOEX ji);

	void ValueChange(float val, CPlug *pPl, int nAxis);

	mi *p_mi;
	tvals *tval;
	int tracknum;

	vector<CPeerCtrl *> peerctrl;
	vector<CEnvData *> mapping;
	vector<int> Track; //for control

	float ValueCurrent, ValueStep, ValueTarget;
	bool ValueSliding;
	float ValueOnLastUpdate, ValueOnLastTick;

	int MIDIChannel;
	int MIDIController;
	bool MIDIFeedback;
	bool MIDIIncDec;
	bool MIDIIncDecWrap;
	int MIDIFBOutDev;
	int LastFBCCValue;

/*	char JoyID;
	char JoyItem;
	char JoyItemType;
	int JoyMin;
	int JoyMax;
	float JoyCurrentPos;
	float JoyLastPos;
	float JoyAntiJitterThreshold;
	bool JoyToggleState;
*/
	bool Slaved;
	bool ShouldSend;

	vector<SAxisInfo> axes;

//	char DisplayName[32];
};

/////////////////////////////////////////////////////////////////////////////

class miex : public CMDKMachineInterfaceEx
{
public:
	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);
	virtual void MidiControlChange(int const ctrl, int const channel, int const value);
	mi *p_mi;
};

/////////////////////////////////////////////////////////////////////////////

class CMidiDialog;

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop() {}
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode) {return false;}
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void OutputModeChanged(bool stereo) {}
	virtual void Command(const int i);
	virtual void SetNumTracks(const int n) {numTracks = n;};
	virtual void MidiNote(int const channel, int const value, int const velocity);

	bool DblClickEvent (void *p);
	void LoadPlugins();

	inline void SendMIDICC (const int device, const int channel, const int ctrl, const int value);

	miex ex;

	gvals gval;
	tvals tval[MAX_TRACKS];
	avals aval;

public:
	CMachine *ThisMac;
	CMidiDialog *pmididlg;

	CTrack Tracks[MAX_TRACKS];
	int numTracks;

	bool Initialising;
	bool JustTicked;
	bool MDKInitRun;

	int UpdateCounter;
	int SamplesSinceLastUpdate;

//	int UpdateJoyCounter;

	bool bMuted;
	bool bWorkedSinceLastTick;

public:
	bool Inertia;
	float InertiaLength;

	vector <CPlug *> plugs;

	bool bHackedParamWndLastTick;
};

//#define UpdateJoyCounterThreshold 10

#endif
