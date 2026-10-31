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

#ifndef __BTDSYS_KITGRID_DRUMKIT_H
#define __BTDSYS_KITGRID_DRUMKIT_H

#include "../../mdk/mdk.h"
#include <windows.h>
#include <typeinfo.h>
#include "resource.h"
#include "hermite.h"
#include "LoadStatusDlg.h"
#include <vector>
using namespace std;

#define PUT_STATUS	if (dlg) dlg.SetDlgItemText(IDC_STATUS, stat); OutputDebugString(stat);

void ReadString (char *temp, CMachineDataInput * const pi);
void WriteString (const char *temp, CMachineDataOutput * const po);

extern HINSTANCE dllInstance;

class CDrumkitLayer
{
public:
	CDrumkitLayer() {};
	virtual ~CDrumkitLayer() {};

	int nSampleRate;
	bool bStereo;

	int numSamples;
	int datasize, dataoffset;
	bool bDataLoaded;
	char dataFileName[256];

	int nVelMin, nVelMax;
	float fVol;
	float fVolCompensate;
	float fRate;

	void GetInfo(char *txt);

	virtual float GetSampleL(const float Pos) {MessageBox(NULL,"PimPom","GetSampleL",MB_OK); return 0.0f;};
	virtual float GetSampleR(const float Pos) {MessageBox(NULL,"PimPom","GetSampleR",MB_OK); return 0.0f;};
	virtual float GetRawSampleL(const int Pos) {MessageBox(NULL,"PimPom","GetRawSampleL",MB_OK); return 0.0f;};
	virtual float GetRawSampleR(const int Pos) {MessageBox(NULL,"PimPom","GetRawSampleR",MB_OK); return 0.0f;};

	virtual int GetSize(int mode) {MessageBox(NULL,"PimPom","GetSize",MB_OK); return 0;};

	virtual bool LoadWaveData() {MessageBox(NULL,"PimPom","LoadWaveData",MB_OK); return false;};
	virtual void FlushWaveData() {MessageBox(NULL,"PimPom","FlushWaveData",MB_OK);};

	virtual void SongLoad(CMachineDataInput  *pi) {MessageBox(NULL,"PimPom","SongLoad",MB_OK);};
	virtual void SongSave(CMachineDataOutput *po) {MessageBox(NULL,"PimPom","SongSave",MB_OK);};
	virtual char GetDataType() {MessageBox(NULL,"PimPom","GetDataType",MB_OK); return 127;};
};

template <typename T>
class CDrumkitLayerT : public CDrumkitLayer
{
public:
	T *data;

	CDrumkitLayerT() {data = NULL;};
	virtual ~CDrumkitLayerT() {if (data) delete[] data;};

	virtual inline float GetSampleL(const float Pos)
	{
		if (bStereo)
			return ::GetSampleL(Pos,data,numSamples);
		else
			return ::GetSampleM(Pos,data,numSamples);
	};

	virtual inline float GetSampleR(const float Pos)
	{
		if (bStereo)
			return ::GetSampleR(Pos,data,numSamples);
		else
			return ::GetSampleM(Pos,data,numSamples);
	};

	virtual inline float GetRawSampleL(const int pos)
	{
		if (bStereo)
			return (float)data[pos*2];
		else
			return (float)data[pos];
	};

	virtual inline float GetRawSampleR(const int pos)
	{
		if (bStereo)
			return (float)data[pos*2+1];
		else
			return (float)data[pos];
	};

	virtual bool LoadWaveData()
	{
		FILE *f = fopen(dataFileName,"rb");
		if (!f) return false;
		fseek(f,dataoffset,SEEK_SET);

		char stat[512];
		CLoadStatusDlg dlg;

		strcpy(stat,"Loading wave "); strcat(stat,dataFileName); PUT_STATUS;

		numSamples = datasize / sizeof(T);
		if (data) delete[] data;
		data = new T[numSamples];
		if (bStereo) numSamples /= 2;

		fread(data,1,datasize,f);

		fclose(f);
		bDataLoaded = true;

		return true;
	};

	virtual int GetSize(int mode)
	{
		if (bDataLoaded || mode == 2)
			return datasize;
		else
			return 0;
	};

	virtual void FlushWaveData()
	{
		if (data) delete[] data;
		data = NULL;
		bDataLoaded = false;
	};

	virtual char GetDataType()
	{
		if (typeid(T) == typeid(short))
			return 0;
		else if (typeid(T) == typeid(int24))
			return 1;
		else if (typeid(T) == typeid(int))
			return 2;
		else if (typeid(T) == typeid(float))
			return 3;
		else
			return 127;
	};

	virtual void SongLoad(CMachineDataInput *pi)
	{
		pi->Read(bDataLoaded);
		ReadString(dataFileName,pi);

		pi->Read(nSampleRate);
		pi->Read(bStereo);

		pi->Read(nVelMin); pi->Read(nVelMax);
		pi->Read(fVol);
		pi->Read(fVolCompensate);
		pi->Read(fRate);

		pi->Read(datasize);
		pi->Read(dataoffset);
		numSamples = datasize / sizeof(T);

		if (bDataLoaded)
		{
			data = new T[numSamples];
			pi->Read(data, datasize);
		}

		if (bStereo) numSamples /= 2;
	};

	virtual void SongSave(CMachineDataOutput *po)
	{
		po->Write(bDataLoaded);
		WriteString(dataFileName,po);

		po->Write(nSampleRate);
		po->Write(bStereo);

		po->Write(nVelMin); po->Write(nVelMax);
		po->Write(fVol);
		po->Write(fVolCompensate);
		po->Write(fRate);

		po->Write(datasize);
		po->Write(dataoffset);

		if (bDataLoaded)
		{
			po->Write(data, datasize);
		}
	};
};


class CDrumkitSample
{
public:
	void Reset();

	char Name[32];
//	float Tone;
//	float Volume;
//	inline dword GetNumSamples() {return bStereo ? data.size() >> 1 : data.size();};

	inline bool HasData() {return !layers.empty();};

	vector<CDrumkitLayer *> layers;

	CDrumkitLayer *GetLayer(float vel);

/*	int nSampleRate;
	bool bStereo;

	vector<float> data;

	float GetSampleL(const float Pos);
	float GetSampleR(const float Pos);
*/

	int GetSize(int mode);

	void GetInfo(char *txt);

	CDrumkitSample();
	~CDrumkitSample();
};

class mi;

enum EDrumkitType
{
	ktPSI,
	ktBattery
};

class CDrumkit
{
public:
	char Name[128];
	char FileName[256];
	char Author[64];
	char Comment[128];

	CDrumkitSample sample[201];

	CDrumkit();
	~CDrumkit();

	void Reset();

	bool LoadFromFile(char *FileName);
	void Load(CMachineDataInput * const pi);
	void Save(CMachineDataOutput * const po, int mode);

	void FlushSampleMemory();

	int GetSize(int mode);

	void ShowInfoDlg(mi *pmi);

	HINSTANCE dllInstance;

	EDrumkitType nKitType;

private:
	void LoadPSIFromFile(FILE *f, char *filename);
	bool LoadBatKitFromFile(FILE *f, char *basepath);
};

#endif
