/*
peerstate.h: header file for PeerState
Copyright (c) 2005, Edward Powley
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

#include "../../mdk/mdk.h"
#include <vector>
using namespace std;

namespace PeerState
{
	const int NUM_SNAPSHOTS = 128;

#pragma pack(1)

	class gvals
	{
	public:
		byte Snapshot;
		word Inertia;
	};

	class avals
	{
	public:
		int MIDIChan;
		int MIDISnapKey;
		int InertiaFreq;
		int DblClickAction;
	};

#pragma pack()

	class mi;

	struct snap
	{
		byte *pg;
		byte *pt;
	};

	class CMachineRec
	{
	public:
		mi *pmi;

		CMachine *pMac;
		bool bActive;
		vector<bool> vbParamActive;

		byte *pGState;
		byte *pTState;
		int nGStateSize;
		int nTStateSize;

		snap Snapshots[NUM_SNAPSHOTS];

		void TakeSnapshot(int n);
		void RestoreSnapshot(int n);

		bool CheckMachine();

		bool bLoading;
		char szLoadingMac[256];

		bool bSliding;
		int nSlideProgress, nSlideLen;
		snap FromSnap;
		int nToSnap;

		void UpdateInertia(int numSamples, bool Hack);

		int nCheckCountDown;
	};

	class miex : public CMDKMachineInterfaceEx
	{
	};

	class CMainDialog;
	class COtherDialog;

	class mi : public CMDKMachineInterface
	{
	public:
		// standard
		mi();
		virtual ~mi();
		virtual void Tick();
		virtual void Stop() {}
		virtual void MDKInit(CMachineDataInput * const pi);
		virtual bool MDKWork(float *psamples, int numsamples, int const mode) { return false; };
		virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
		virtual void MDKSave(CMachineDataOutput * const po);
		virtual char const *DescribeValue(int const param, int const value);
		virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
		virtual void Command(const int i);
		virtual void OutputModeChanged(bool stereo) {}
		virtual void MidiNote(const int channel, const int value, const int velocity);
		bool DblClickEvent(void *p);
		miex ex;
		gvals gval;
		avals aval;
		CMachine *thismac;
		int numTracks;
		bool bFirstTick, bDoneWork;

	//	bool CanUpdateParamNameAndState;
	//	void UpdateParamNameAndState(int n);
		bool GotSnap(int n);

		int nCurrentSnapshot;

		vector<CMachineRec> peers;
		CMachineRec *FindMachineInPeers(const char *name);
		CMachineRec *FindMachineInPeers(CMachine *p);

		char snapname[NUM_SNAPSHOTS][32];

		void AddMachineToPeers(const char *name);
		void AddMachineToPeers(CMachine *p);

		CMainDialog *pdlg;
		COtherDialog *pdlg2;

		int nInertiaLen;
		int nSamplesSinceLastInertia;

	};


};






