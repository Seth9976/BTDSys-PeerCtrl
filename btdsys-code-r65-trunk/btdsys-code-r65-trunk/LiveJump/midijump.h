
#ifndef __BTDSYS_MIDIJUMP_H
#define __BTDSYS_MIDIJUMP_H

#include "../mdk/mdk.h"
#include "KeyCapDlg.h"
#include <vector>
using namespace std;

extern CMachineParameter const paraTickSnap;
extern CMachineParameter const paraJumpNote;
extern CMachineParameter const paraJumpTick;
extern CMachineParameter const paraJumpTickMode;
extern CMachineParameter const paraOnOff;

////////////////////////////////////////////////////////////////////////

extern CMachineParameter const *pParameters[];

extern CMachineAttribute const *pAttributes[];

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class gvals
{
public:
	word TickSnap;
	byte JumpNote;
	word JumpTick;
	byte JumpTickMode;
	byte OnOff;
};

class avals
{
public:
	int MidiChannel;
	int WantNoteOff;
};

#pragma pack()

enum
{
	numTickSnap,
	numJumpNote,
	numJumpTick,
	numJumpTickMode,
	numOnOff,
	NUMG,

	NUMA = 2,
};

////////////////////////////////////////////////////////////////////////

extern CMachineInfo const MacInfo;


enum JumpTriggerType {jttNote, jttKey, jttVKey};

struct JumpTickRec
{
	JumpTriggerType eTrigType;
	int nTrig;
	int nTick;
	char nJumpMode;
};

////////////////////////////////////////////////////////////////////////

class mi;

class miex : public CMDKMachineInterfaceEx
{
public:
	mi *pmi;
};

class mi : public CMDKMachineInterface
{
	friend class CSettingsDlg;
	friend class CKeyCapDlg;
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop();
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode);
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode) {return false;};
	virtual void Command(int const i);
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void OutputModeChanged(bool stereo) {}

	virtual void MidiNote(const int channel, const int value, const int velocity);

	miex ex;

	gvals gval;
	avals aval;

private:
	bool bHackEnabled;
	bool bOn;

	vector<JumpTickRec> vJumpTicks;

	JumpTickRec *FindTickRec(JumpTriggerType ttype, int trig);

//	int nDestTicks[128];
//	char nJumpModes[128];

	int nTickSnap;
	int nNextDest;
	int nNextDestMode;
	int nTickTriggerMode;

	int nMidiNote;
	bool bHadNoteOff;
	bool bHeldMoreThanOneLoop;
	
	CSettingsDlg *pdlg;

	CKeyCapDlg *pKeyCapDlg;

	bool bUseNewCallbacks;
	int getSongPosition();
	void setSongPosition(int pos);
};


#endif