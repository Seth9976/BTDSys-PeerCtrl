/*
Copyright (c) 2003-2006, Edward Powley
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

#ifndef __BTDSYS_BUZZMI_H
#define __BTDSYS_BUZZMI_H

#include "../mdk/mdk.h"

class CMICallbacksInternal;

//special values
#define INPUT_MACHINE	((CMDKMachineInterface *)1)
#define OUTPUT_MACHINE	((CMDKMachineInterface *)2)
#define THRU_MACHINE	((CMDKMachineInterface *)3)

class mi;
class CMDKInterface;
class CParamDlg;

#define BMIH_FILELINE   char *file_ = "???", int line_ = 0

class CMachine
{
	friend class CMICallbacksInternal;
public:
	CMachine();
	~CMachine();

	bool LoadMachine(char const *dllName, mi *ph);
	void InitMachineNoFileData();
	void InitMachineFromFileData(CMachineDataInput * const pi);
	void SaveMachineFileData(CMachineDataOutput * const po);
	void InitParamsAttrs(bool bNoAttr = false);
	void ClearParams();
	void UnloadMachine();

	bool Work(int numsamples, const int mode);

	void ChangeParam(int par, int track, int value);
	void ChangeParamFromHost(int par, int track, int value);

	void LogException(char *where, char *file, int line);
	int nExceptions;

	HMODULE hm;

private:
	CMachineInterface *pMI;
	CMachineInterfaceEx *pEx;
	CMDKMachineInterface *pMDKMI;
	CMDKMachineInterfaceEx *pMDKEx;
	CMDKInterface *pMDKI;
	CMICallbacksInternal *pCB;

public:
	CMachineInfo const *pInfo;

	bool IsThruMachine() {return (pMI == THRU_MACHINE);}
	bool IsInputMachine() {return (pMI == INPUT_MACHINE);}
	bool IsOutputMachine() {return (pMI == OUTPUT_MACHINE);}

	void SetMI(CMachineInterface *pmi) {pMI = pmi;}

	bool TestPtr(char *name)
	{
		if (strcmp(name,"pMI") == 0)
			return (pMI != NULL);
		if (strcmp(name,"pEx") == 0)
			return (pEx != NULL);
		if (strcmp(name,"pMDKMI") == 0)
			return (pMDKMI != NULL);
		if (strcmp(name,"pMDKI") == 0)
			return (pMDKI != NULL);
		if (strcmp(name,"pCB") == 0)
			return (pCB != NULL);

		MessageBox(NULL,"ERROR in CMachine::TestPtr",name,MB_OK);
	}

	bool IsMDK() {return (pMDKMI != NULL);}

	void Init(CMachineDataInput *pidi, BMIH_FILELINE)
	{
		try	{pMI->Init(pidi);}
		catch (...)	{LogException("Init",file_,line_);}
	}

	void SetNumTracks(int n, BMIH_FILELINE)
	{
		try	{pMI->SetNumTracks(n);}
		catch (...)	{LogException("SetNumTracks",file_,line_);}
	}

	void AttributesChanged(BMIH_FILELINE)
	{
		try	{pMI->AttributesChanged();}
		catch (...)	{LogException("AttributesChanged",file_,line_);}
	}

	void Command(int c, BMIH_FILELINE)
	{
		try	{pMI->Command(c);}
		catch (...)	{LogException("Command",file_,line_);}
	}

	const char *DescribeValue(int p, int v, BMIH_FILELINE)
	{
		try	{return pMI->DescribeValue(p,v);}
		catch (...)	{LogException("DescribeValue",file_,line_); return NULL;}
	}

	void OutputModeChanged(bool stereo, BMIH_FILELINE)
	{
		try	{pMDKMI->OutputModeChanged(stereo);}
		catch (...)	{LogException("OutputModeChanged",file_,line_);}
	}

	void Save(CMachineDataOutput *pido, BMIH_FILELINE)
	{
		try
		{
			if (pMDKMI)
			{
				int n = 2; pido->Write(&n,1);
				pMDKMI->MDKSave(pido);
			}
			else
				pMI->Save(pido);
		}
		catch (...)
		{
			LogException("Save",file_,line_);
		}
	}

	void Tick(BMIH_FILELINE)
	{
		try	{pMI->Tick();}
		catch (...)	{LogException("Tick",file_,line_);}
	}

    void Stop(BMIH_FILELINE)
    {
        try {pMI->Stop();}
        catch (...) {LogException("Stop",file_,line_);}
    }

	int *GetAttrVals()
	{
		if (pMI) return pMI->AttrVals;
	}

	word *pGlobalState;
	word *pTrackState;

	char name[256];
	char dllname[256];

	bool bSpecial;
	bool bIsEffect;
	bool bStereoOut;
	bool bNoOut;

	int nTracks;

	mi *pHost;

//	bool bMDK;

	//GUI stuff
	float fX, fY;
	CRect rect;
	bool bSelected;

	CParamDlg *pParamDlg;

	void GetSubMenuItems(HMENU hMenu, int nMenu, int nBaseID);

	void MainWindowIsClosing();

	//work stuff
	float buf[512];
	bool bWorked;
	bool bWorkResult;

	//for determining how to save connections
	int nPosInVector;

	//for loading bmx files
	FILE *fBMX;
};


class CConnection
{
public:
	CConnection() : fAmp(1.f), fPan(1.f) {}
	CMachine *pMac1;
	CMachine *pMac2;
	float fAmp;
	float fPan;
	CRect rect;
};

class CMDKInterface
{
public:
//	virtual ~CMDKInterface(){};

// Dummy placeholder functions
	virtual void _Init(CMachineDataInput * const pi){};
	virtual bool _Work(float *psamples, int numsamples, int const mode){ return false; };
	virtual bool _WorkMonoToStereo(float *pin, float *pout, int numsamples, int const mode){ return false; };
	virtual void _Save(CMachineDataOutput * const po){};
	void _SetOutputMode(bool stereo){};
	
// Access functions
	virtual CMDKMachineInterfaceEx *GetEx()
	{
		return pMDKInterfaceEx;
	};
	
	virtual void OutputModeChanged(bool stereo)
	{};

// MDK chainer functions
	virtual bool MDKWork(float *psamples, int numsamples, int const mode)
	{
		return pMDKInterface->MDKWork( psamples, numsamples, mode );
	};

	virtual bool MDKWorkStereo(float *ipsamples, float *opsamples, int numsamples, int const mode)
	{
		return pMDKInterface->MDKWorkStereo( opsamples, numsamples, mode );
	};

	virtual void MDKInit( CMachineDataInput * const pi )
	{
		pMDKInterface->MDKInit( pi );
	};

	virtual void MDKSave( CMachineDataOutput * const po )
	{
		pMDKInterface->MDKSave( po );
	};

	virtual void SetOutputMode( bool stereo )
	{
		OutputDebugString(" SetOutputMode");
		// set the required Work() mode
		pHostMachine->bStereoOut = stereo;
	};

	CMDKMachineInterface * pMDKInterface;		// the MDK interface
	CMDKMachineInterfaceEx * pMDKInterfaceEx;	// the extended MDK interface
	CMachine *pHostMachine;
};

//class CMDKInterface

class CMICallbacksInternal : public CMICallbacks
{
public:
	virtual CWaveInfo const *GetWave(int const i)
		{OutputDebugString("GetWave"); return pHostCB->GetWave(i);}
	virtual CWaveLevel const *GetWaveLevel(int const i, int const level)
		{OutputDebugString("GetWaveLevel"); return pHostCB->GetWaveLevel(i,level);}
	virtual void MessageBox(char const *txt);
	virtual void Lock()
		{pHostCB->Lock();}
	virtual void Unlock()
		{pHostCB->Unlock();}
	virtual int GetWritePos()
		{return pHostCB->GetWritePos();}
	virtual int GetPlayPos()	
		{return pHostCB->GetPlayPos();}
	virtual float *GetAuxBuffer()
		{return pHostCB->GetAuxBuffer();}
	virtual void ClearAuxBuffer()
		{pHostCB->ClearAuxBuffer();}
	virtual int GetFreeWave()
		{return pHostCB->GetFreeWave();}
	virtual bool AllocateWave(int const i, int const size, char const *name)
		{return pHostCB->AllocateWave(i,size,name);}
	virtual void ScheduleEvent(int const time, dword const data)
		{pHostCB->ScheduleEvent(time,data);}
	virtual void MidiOut(int const dev, dword const data)
		{pHostCB->MidiOut(dev,data);}
	virtual short const *GetOscillatorTable(int const waveform)
		{return pHostCB->GetOscillatorTable(waveform);}

	// envelopes
	virtual int GetEnvSize(int const wave, int const env);
	virtual bool GetEnvPoint(int const wave, int const env, int const i, word &x, word &y, int &flags);

	virtual CWaveLevel const *GetNearestWaveLevel(int const i, int const note);
	
	// pattern editing
	virtual void SetNumberOfTracks(int const n) {};
	virtual CPattern *CreatePattern(char const *name, int const length) {return NULL;};
	virtual CPattern *GetPattern(int const index) {return NULL;};
	virtual char const *GetPatternName(CPattern *ppat) {return NULL;};
	virtual void RenamePattern(char const *oldname, char const *newname) {};
	virtual void DeletePattern(CPattern *ppat) {};
	virtual int GetPatternData(CPattern *ppat, int const row, int const group, int const track, int const field)
		{return 0;};
	virtual void SetPatternData(CPattern *ppat, int const row, int const group, int const track, int const field, int const value) {};
 		
	// sequence editing
	virtual CSequence *CreateSequence() {return NULL;};
	virtual void DeleteSequence(CSequence *pseq) {};
	

	// special ppat values for GetSequenceData and SetSequenceData 
	// empty = NULL
	// <break> = (CPattern *)1
	// <mute> = (CPattern *)2
	// <thru> = (CPattern *)3
	virtual CPattern *GetSequenceData(int const row) {return NULL;};
	virtual void SetSequenceData(int const row, CPattern *ppat) {};
		

	// buzz v1.2 (MI_VERSION 15) additions start here
	
	virtual void SetMachineInterfaceEx(CMachineInterfaceEx *pex);
	// group 1=global, 2=track
	virtual void ControlChange__obsolete__(int group, int track, int param, int value);						// set value of parameter
	
	// direct calls to audiodriver, used by WaveInput and WaveOutput
	// shouldn't be used for anything else
	virtual int ADGetnumChannels(bool input)
		{return pHostCB->ADGetnumChannels(input);}
	virtual void ADWrite(int channel, float *psamples, int numsamples)
		{pHostCB->ADWrite(channel, psamples, numsamples);}
	virtual void ADRead(int channel, float *psamples, int numsamples)
		{pHostCB->ADRead(channel, psamples, numsamples);}

	virtual CMachine *GetThisMachine();	// only call this in Init()!
	virtual void ControlChange(CMachine *pmac, int group, int track, int param, int value);		// set value of parameter (group & 16 == don't record)

	// returns pointer to the sequence if there is a pattern playing
	virtual CSequence *GetPlayingSequence(CMachine *pmac) {return NULL;};

	// gets ptr to raw pattern data for row of a track of a currently playing pattern (or something like that)
	virtual void *GetPlayingRow(CSequence *pseq, int group, int track) {return NULL;};

	virtual int GetStateFlags()
		{return pHostCB->GetStateFlags();};

	virtual void SetnumOutputChannels(CMachine *pmac, int n);	// if n=1 Work(), n=2 WorkMonoToStereo()

	virtual void SetEventHandler(CMachine *pmac, BEventType et, EVENT_HANDLER_PTR p, void *param);

	virtual char const *GetWaveName(int const i)
		{return pHostCB->GetWaveName(i);}

	virtual void SetInternalWaveName(CMachine *pmac, int const i, char const *name)	// i >= 1, NULL name to clear
		{pHostCB->SetInternalWaveName(pmac,i,name);}

	virtual void GetMachineNames(CMachineDataOutput *pout);		// *pout will get one name per Write()
	virtual CMachine *GetMachine(char const *name);
	virtual CMachineInfo const *GetMachineInfo(CMachine *pmac);
	virtual char const *GetMachineName(CMachine *pmac);

	virtual bool GetInput(int index, float *psamples, int numsamples, bool stereo, float *extrabuffer);

public:
	CMICallbacks *pHostCB;
	CMachine *pHostMachine;
	mi *pHost;
};





#endif
