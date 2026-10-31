/*
Copyright (c) 2005-2011, Edward Powley
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


#ifndef __BTDSYS_PSCALE_MACHINE_H
#define __BTDSYS_PSCALE_MACHINE_H

#ifdef REBUILD_060710
#define MACHINE_NAME		"BTDSys PeerScale"
#define MACHINE_VERSION		"060710a"
#define MACHINE_AUTHOR		"Ed Powley (BTDSys)"
#else
#define MACHINE_NAME		"PeerScale"
#define MACHINE_VERSION		"BETA 4a"
#define MACHINE_AUTHOR		"BTDSys / IX"
#endif

#define MACHINE_FULL_NAME	MACHINE_NAME" "MACHINE_VERSION

#include <vector>

#include "..\Win32++\include\wincore.h"
#include "..\PeerLib2\peerctrl.h"
#include "..\mdk\MachineInterface.h"

using namespace std;

extern CMachineParameter const
	paraScale,
	paraRoot,
	paraLowOct,
	paraHighOct,
	paraPeerMe,
	paraTargetTrack;

#define MAX_SCALES 1000

////////////////////////////////////////////////////////////////////////

extern CMachineParameter const *pParameters[];

extern CMachineAttribute const *pAttributes[];

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class gvals
{
public:
	word Scale;
	byte Root;
	byte LowOct;
	byte HighOct;
	word PeerMe;
	byte TargetTrack;
};

class avals
{
public:
};

#pragma pack()

enum
{
	numScale,
	numRoot,
	numLowOct,
	numHighOct,
	numPeerMe,
	numTargetTrack,
	NUMG,

	NUMA = 0,
};

////////////////////////////////////////////////////////////////////////

extern CMachineInfo const MacInfo;

////////////////////////////////////////////////////////////////////////

class mi;

struct SScale
{
	SScale() {for (int i=0; i<12; i++) notes[i] = 0;  name[0]=0;}
	int notes[12];
	char name[512];
};

struct SChordRec
{
	int root;
	int chord;
};

class mi : public CMachineInterface, public CMachineInterfaceEx
{
public: // CMachineInterface
	virtual void Tick();
	virtual void Stop();
	virtual void Init(CMachineDataInput * const pi);
	virtual bool Work(float *psamples, int numsamples, int const mode);
	virtual void Command(int const i);
	virtual void Save(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual void MidiNote(int const channel, int const value, int const velocity);

public: // CMachineInterfaceEx

	virtual void ImportFinished();

public:
	mi();
	virtual ~mi();

	bool DblClickEvent(void *p);

	void LoadScaleFile();
	int AddScales(const char *txt, int first = 0);

	int PeerValToNote(int p);
	SChordRec *PeerValToChord(int p, int &octave);

	void RecalculateChords();

	bool IsChordInScale(
		int chordroot, int numchordnotes, signed char *chordnotes,
		int scaleroot, int numscalenotes, int *scalenotes);

	bool IsNoteInScale(
		int note,
		int scaleroot, int numscalenotes, int *scalenotes);

	int FindNearestNoteInScale(
		int note,
		int scaleroot, int *scalenotes, int roundingmode);

public:

	gvals gval;
	avals aval;

	CPeerCtrl pc;

	int scale;
	int root;
	int lowoct;
	int highoct;
	int targettrack;

	int m_note;
	int m_chord;

	CMachine *ThisMac;

	bool initialising;

	SScale scales[MAX_SCALES];
	int numBuiltinScales;

    tString userscales;

	bool specialpeerchordmode;

	vector<SChordRec> chords;

	int midiChannel;
	int midiMode;
};

#define MAXNOTES 6

struct SChord
{
	char const *Name;
	int NumOcts;
	int NumNotes;
	signed char Note[MAXNOTES];
};

#define NUMCHORDS 54

extern SChord ChordTable[NUMCHORDS];

#endif
