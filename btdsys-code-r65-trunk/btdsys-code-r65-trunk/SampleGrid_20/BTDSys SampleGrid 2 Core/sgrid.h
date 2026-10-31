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

#ifndef __BTDSYS_SGRID_H
#define __BTDSYS_SGRID_H

//#define NUMTRIGGERS 16
//#define NUMTRIGGERS_STRING_HEX "10"
//#define NUMTRIGGERS_STRING_DEC "16"

//#define TRIGGER_TYPE_SWITCH
//#define TRIGGER_TYPE_BYTE

/*
#define _4_		"4"
#define _8_		"8"
#define _16_	"16"
#define _24_	"24"
#define _32_	"32"
#define _04_	"04"
#define _08_	"08"
#define _10_	"10"
#define _18_	"18"
#define _20_	"20"
*/

//overloader wavetable extensions

#define WF_NOT16		4

#define WFE_32BITF		1
#define WFE_32BITI		2
#define WFE_24BIT		3


#define MAXTRIGGERS	64

#include "../../mdk/mdk.h"
#include "../../dsplib/dsplib.h"
#include "peerctrl.h"
#include "params.h"
//#include "attrs.h"
#include "drumkit.h"
#include "channel.h"
#include "BandLimit.h"
#include "InertiaParam.h"
#include "func_typedefs.h"
#include "fuzzpilz_envelope.h"

#include <vector>
using namespace std;

void ReadString (char *temp, CMachineDataInput * const pi);
void WriteString (const char *temp, CMachineDataOutput * const po);

enum
{
	trigVel,
	trigPan,
	trigTune,
	trigProb,
	trigArg1,
	trigArg2,
	trigOffset,
	NUM_TRIGGER_MODES,
	trigInherit
};

extern const char *pszTriggerModes[];

enum EKeyType
{
	ktNone,
	ktChar,
	ktVkey,
	ktMidi
};

enum
{
	peerTrig,
	NUM_PEERS,
	peerVelVol,
	peerPan,
	peerTune,
	peerOffset,
	peerAux1,
	peerAux2,
//	NUM_PEERS
};

extern const char *pszPeerNames[]; //see paperwork.cpp
extern const int nPeerAllowedTypes[];

enum
{
	gmmToggle,
	gmmMomOff,
	gmmMomOn,
	NUM_GROUP_MUTE_MODES
};

extern const char *pszGroupMuteModes[];

enum
{
	lmNone,
	lmStraight,
	lmPingPong,
	lmRandom,
	NUM_LOOP_MODES
};

extern const char *pszLoopModes[];

enum
{
	cmdGroupSetup,
	cmdGroupMatrix,
	cmdDrumkitMenu,
//	cmdLoopSlicer,
	cmdEnvelope,
//	cmdShuffle,
	cmdCommandList,
	cmdAbout,
	NUM_COMMANDS
};

extern const char *pszCommands[];

class mi;
struct STrigInfo;

class CTrack
{
public:
	CTrack();
	~CTrack();
	void Tick();
	void Reset();
	bool Generate(float *psamples, int numsamples, int const mode);

	void EverythingOffNow();

	inline void UpdateVolRamping();

	int GetSampleNum();
	CDrumkitSample *GetPSISample();
	const char *GetSampleName();

	void ProcessGlobalCommand(byte CommandVal, byte ArgumentVal);
	bool ProcessCommand(byte CommandVal, byte ArgumentVal,
						STrigInfo *pti, int &RetrigInt, int &RetrigCount);

	bool ProcessTrigger(int type, int val,
						byte &ArgumentVal1, byte &ArgumentVal2,
						STrigInfo *pti, int &RetrigInt, int &RetrigCount);

	void TriggerIt( byte TrigVal, byte CommandVal1, byte ArgumentVal1, byte CommandVal2,
					byte ArgumentVal2, int Delay = 0);
	void TriggerOn(STrigInfo ti);
	void TriggerOff();
	void TriggerPreview(CDrumkitLayer *pLayer);
	void TriggerPreview(CDrumkitSample *pSample);

	int GetSampleLen(float vel);
	float GetSampleRateCorrection(float vel);

	mi *pmi;
	tvals *tval;
	int tracknum;

	bool Playing;
	bool Waiting;
	int WaitCountDown;
	float CurPos;

	bool Muted;
	bool MuteRamping;
	float MuteRampVol;
	float MuteRampVolStep;
	int MuteRampCountDown;

	bool WaitOff;
	int WaitOffCountDown;

	int WaveNum;

	float fOffset;
	float fLoopStart, fLoopEnd;
	int loopMode;
	int nCut;

	InertiaParam ipVolume;
	InertiaParam ipPan;
	InertiaParam ipTune;

	float fHumanPan, fHumanVel, fHumanTune;
	float fVelocity;

	float VolumeCurrent;

	float Velocity;
	float VelL;
	float VelR;

	float LGainCurrent;

	float TuneCurrent;
	float TuneThisHit;

	int nLoopFit;
	int nLoopFitMode;

	bool Reverse;

	bool RampingOn;
	float RampOnVol;
	bool RampingOff;
	float RampOffPos;
	float RampOffVol;
	float RampOffL;
	float RampOffR;

	float RampVolTarget, RampVolStep;
	int RampVolCountDown;

	int CmdLenUnit;
    int CmdLenToSamples(int n);
    int SamplesToCmdLen(int s);

	byte Trigger;

	int Group;

	int nTrigKey;
	EKeyType nTrigKeyType;

	void ResetVirtualChannels(int n = -1);

	CPeerCtrl *pCtrl[NUM_PEERS];

	int nVolEnv, nPanEnv, nTuneEnv, nPosEnv;
	float fVolEnvLen, fPanEnvLen, fTuneEnvLen;

	float fVu;

	float retrigVel, retrigPan, retrigRate, retrigOffset; 
	int retrigCut;

	vector<STrigInfo> retrigs;

	vector<CChannel> channels;
	CChannel rampoffchannel;

    bool customNumberOfChannels;
};

struct STrigInfo
{
	float vel, pan, rate, offset; 
	int cut, delay;

	operator < (STrigInfo s2) {return delay < s2.delay;} //for sorting a vector of 'em
};


class CGroup
{
public:
	CGroup();
	~CGroup();
	char name[256];
	int nOutput;
	bool bXOR;

	int nMuteKey;
	EKeyType nMuteKeyType;
	int nMuteMode;

	bool bMuted;
};

class miex : public CMDKMachineInterfaceEx
{
public:
	mi *pmi;
	virtual char const *DescribeParam(int const param);
	virtual void GetSubMenu(int const i, CMachineDataOutput *pout);
};

class CAuxGroup;
class CAuxGroupInfo;
class CGroupDlg;
class CGroupPerfDlg;
class CLoopDlg;
class CEnvelopeEditor;
class CCmdListDlg;

class mi : public CMDKMachineInterface
{
public:
	mi(int n, char t);
	virtual ~mi();
	virtual void Tick();
	virtual void Stop();
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode) {return false;}
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);	
	virtual void Command(int const i);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void AttributesChanged();
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void OutputModeChanged(bool stereo) {}
	virtual const CEnvelopeInfo **GetEnvelopeInfos();
	virtual void InertiaOff();

	virtual void DoMidiDialog();
	virtual void MidiNote(const int channel, const int value, const int velocity);

	void EverythingOffNow();

	const char *GetDefaultKitPath();

	miex ex;

	float m_frand(int seed = -1);
	float m_fgrand(float m, float s);

	char const *DescribeTrigger(int const type, int const value);

public:
	int nTriggers;
	char nTrigType;

public:
	gvals gval;
	avals aval;
	tvals tval[MAXTRIGGERS];
	CTrack Tracks[MAXTRIGGERS];
	float pfOutBuf[NUM_OUTPUTS][513];
	CGroup Groups[NUM_GROUPS];

	CMachine *ThisMac;
	char pszMacName[256];

	HWND hMidiDlg;
	int LastMidiNote;

	HMODULE hm;

//	HMODULE hCoreDLL;

	CGroupPerfDlg *pGroupPerfDlg;
	CLoopDlg *pLoopDlg;
	CGroupDlg *pGroupDlg;
	CEnvelopeEditor *pEnveDlg;
	CCmdListDlg *pCmdListDlg;

	CCustomEnvelopeShape pCustomEnvelopeShapes[NumCustomEnvelopes];

public:
	int nInertiaLen;

	int nTriggerModeA, nTriggerModeB;

	int FirstWaveNo;
	int ShuffleStep;
	bool Shuffle;
	float ShuffleSize;
	float ShuffleRnd;
	int ShuffleCount;
	bool ShuffleThisTick;

	float RampVolStep;

	int SoloTrack;

	InertiaParam ipVolume;
	InertiaParam ipPan;
	InertiaParam ipTune;

	float fHumanPan, fHumanVel, fHumanTune;
	float fVelocity;

	float VolumeCurrent;
	float LGainCurrent;
	float TuneCurrent;

	bool Initialising;

	bool bKitMode;
	CDrumkit Kit;
	int nKitSaveMode;
	bool SaveKitInFile;

	HWND hPatEdStatusBar;
	HWND hPatEdMachineCombo;

	bool bStopThread;
	void ThreadFunc();

	bool bWorkedSinceLastTick;

	int CmdLenUnit;
};

#define MAX_BLOCKSIZE	MAX_BUFFER_LEN



extern vector<mi *> g_vpmi;

#endif
