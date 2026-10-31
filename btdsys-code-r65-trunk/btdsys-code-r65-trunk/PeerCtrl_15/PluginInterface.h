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

#ifndef __BTDSYS_PCTRL_PLUGININTERFACE_H
#define __BTDSYS_PCTRL_PLUGININTERFACE_H

#include "PCtrlPlug.h"
#include "../mdk/mdk.h"

#include <vector>
using namespace std;

typedef CPCtrlPlug* (__stdcall *pMakeNewPlug)();
typedef int (__stdcall *pGetPCtrlVersion)();

class CPlugAxis
{
public:
	char name[256];
	bool show;

	vector<int> binding;
};

class mi;

class CPlug
{
public:
	CPlug();
	~CPlug();

	HINSTANCE hInst;

	char dllname[256];

	CPCtrlPlug *pPl;
	SPlugInfo *pInfo;
	mi *pmi;

	vector<CPlugAxis> axes;

	bool bHadFileData;
};




class CInternalPCtrlCallbacks : public CPCtrlCallbacks
{
public:
	virtual int		AddAxis(char const *name);
	virtual void	DelAxis(int nAxis);
	virtual void	RenameAxis(int nAxis, char const *name);
	virtual void	ShowAxis(int nAxis);
	virtual void	HideAxis(int nAxis);
	virtual bool	IsAxisShowing(int nAxis);
	virtual float	GetValue(int nAxis);
	virtual void	SetValue(int nAxis, float fValue);

	virtual int		GetHostID();
	virtual int		GetHostCaps();
	virtual float	GetSongPos();
	virtual int		GetSongBPM();
	virtual int		GetSongTPB();
	virtual void	GetBuzzPointers(SBuzzPtrs *p);
	virtual bool	IsSongPlaying();

public:
	CPlug *pPlug;
};


class CDataChunk
{
public:
	CDataChunk() : next(0), ActualLength(0) {};
	~CDataChunk()
	{
		if (next) delete next;
	}

	char data[8192];
	int ActualLength;
	CDataChunk *next;
};

class CInternalDataRead
{
public:
	CInternalDataRead() : DataHead(0) {};
	virtual void	Read(void *data, int length);

	virtual int		ReadInt();
	virtual short	ReadShort();
	virtual char	ReadChar();
	virtual void	ReadASCIIZ(char *c);

	void *d;
	int DataHead;
};

class CInternalDataWrite
{
public:
	CInternalDataWrite() : DataHead(0), DataLen(0), curr(&d) {};
	virtual void	Write(void *data, int length);

	virtual void	Write(int	i);
	virtual void	Write(short	i);
	virtual void	Write(char	i);
	virtual void	WriteASCIIZ(char *c);

	CDataChunk d;
	CDataChunk *curr;
	int DataHead;

	int DataLen;
};


#endif
