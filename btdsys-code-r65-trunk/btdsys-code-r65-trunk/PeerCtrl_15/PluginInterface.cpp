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

#include "stdafx.h"
#include "PluginInterface.h"
#include "PCtrlPlug.h"
#include "ctrl.h"

void mi::LoadPlugins()
{
	//load all plugins

	strcpy(MacInfo_MenuItems, BASE_MENUITEMS);

	WIN32_FIND_DATA fdata;

	char plugin_path[256];
	plugin_path[0] = 0;
	
	HKEY hk;	
	if (RegOpenKey(HKEY_CURRENT_USER,"Software\\Jeskola\\Buzz\\Settings",&hk)==ERROR_SUCCESS)
	{		
		DWORD dwType;
		DWORD dwSize=MAX_PATH;					
		if (RegQueryValueEx(hk,"BuzzPath",NULL,&dwType,(BYTE *)plugin_path,&dwSize)==ERROR_SUCCESS)
		{							
			int len=strlen(plugin_path);
			if (plugin_path[len-1]!='\\') 
			{
				plugin_path[len]='\\';
				plugin_path[len+1]=0;
			}		
			strcat(plugin_path,"Gear\\Generators\\PeerCtrl\\");
		}
		RegCloseKey(hk);
	}	

	if (plugin_path[0])
	{
		char srchpatt[256];
		strcpy(srchpatt, plugin_path);
		strcat(srchpatt, "*.dll");
		HANDLE hfind = FindFirstFile(
			srchpatt,
			&fdata);

		if (hfind != INVALID_HANDLE_VALUE)
		{
			do
			{
				static char dllname[256];
				strcpy(dllname,plugin_path);
				strcat(dllname,fdata.cFileName);

				HINSTANCE hI = LoadLibrary(dllname);
				if (hI)
				{
					pGetPCtrlVersion pfVers;
					pfVers = (pGetPCtrlVersion)GetProcAddress(hI, "GetPCtrlVersion");
					if (pfVers)
					{
						if (pfVers() == 3)
						{
							pMakeNewPlug pfMake 
								= (pMakeNewPlug)GetProcAddress(hI, "MakeNewPlug");

							if (pfMake)
							{
								CPlug *t = new CPlug;
								t->hInst = hI;
								t->pmi = this;
								t->pPl = pfMake();
								strcpy(t->dllname, fdata.cFileName);

								if (t->pPl)
								{
									t->pInfo = t->pPl->GetInfo();

									CInternalPCtrlCallbacks *pcb = new CInternalPCtrlCallbacks;
									pcb->pPlug = t;
									t->pPl->pCB = pcb;

									if (t->pPl->Init())
									{

										strcat(MacInfo_MenuItems, "\n/");
										strcat(MacInfo_MenuItems, t->pInfo->Name);

										plugs.push_back(t);
									}
									else
										delete t;
								}
								else
									delete t;
							}
						}
					}
				}
			}
			while (FindNextFile(hfind, &fdata) != 0);
		}

		FindClose(hfind);
	}
}


CPlug::CPlug()
{
	hInst	= 0;
	pPl		= NULL;
	pInfo	= NULL;
	bHadFileData = false;
}


CPlug::~CPlug()
{
	if (pPl)	
	{
		if (pPl->pCB)
			delete pPl->pCB;
		delete pPl;
	}
	if (hInst)	FreeLibrary(hInst);
}

int CInternalPCtrlCallbacks::AddAxis(char const *name)
{
	CPlugAxis ax;
	strcpy(ax.name, name);

	pPlug->axes.push_back(ax);
	return pPlug->axes.size()-1;
}

void CInternalPCtrlCallbacks::DelAxis(int nAxis)
{
	if (nAxis >= 0 && nAxis < pPlug->axes.size())
	{
		pPlug->axes.erase(pPlug->axes.begin() + nAxis);

		CTrack *pt;
		for (int t=0; t<pPlug->pmi->numTracks; t++)
		{
			pt = &pPlug->pmi->Tracks[t];
			for (int a=0; a<pt->axes.size(); a++)
			{
				if (pt->axes[a].pPl == pPlug)
				{
					if (pt->axes[a].nAxis == nAxis)
						pt->axes.erase(pt->axes.begin() + a);
					else if (pt->axes[a].nAxis > nAxis)
						pt->axes[a].nAxis --;
				}
			}
		}
	}
}

void CInternalPCtrlCallbacks::RenameAxis(int nAxis, char const *name)
{
	strcpy(pPlug->axes[nAxis].name, name);
}

void CInternalPCtrlCallbacks::ShowAxis(int nAxis)
{
	pPlug->axes[nAxis].show = true;
}

void CInternalPCtrlCallbacks::HideAxis(int nAxis)
{
	pPlug->axes[nAxis].show = false;
}

bool CInternalPCtrlCallbacks::IsAxisShowing(int nAxis)
{
	return pPlug->axes[nAxis].show;
}


float CInternalPCtrlCallbacks::GetValue(int nAxis)
{
	if (pPlug->axes[nAxis].binding.size() > 0)
		return pPlug->pmi->Tracks[pPlug->axes[nAxis].binding[0]].ValueTarget;
	else
		return -1.0f;
}

void CInternalPCtrlCallbacks::SetValue(int nAxis, float fValue)
{
	for (int i=0; i<pPlug->axes[nAxis].binding.size(); i++)
		pPlug->pmi->Tracks[pPlug->axes[nAxis].binding[i]].ValueChange(fValue, pPlug, nAxis);
}

int CInternalPCtrlCallbacks::GetHostID()
{
	return HOSTID_BUZZ_PEERCTRL_15;
}

int CInternalPCtrlCallbacks::GetHostCaps()
{
	return CAP_BUZZ_POINTERS | CAP_SONG_POS | CAP_SONG_TEMPO;
}

int CInternalPCtrlCallbacks::GetSongBPM()
{
	return pPlug->pmi->pCB->GetTempo();
}

int CInternalPCtrlCallbacks::GetSongTPB()
{
	return pPlug->pmi->pCB->GetTPB();
}

float CInternalPCtrlCallbacks::GetSongPos()
{
	return (float)pPlug->pmi->pCB->GetSongPosition()
		+ ((float)pPlug->pmi->pMasterInfo->PosInTick 
			/ (float)pPlug->pmi->pMasterInfo->SamplesPerTick);
}

void CInternalPCtrlCallbacks::GetBuzzPointers(SBuzzPtrs *p)
{
	p->pmac = pPlug->pmi->ThisMac;
	p->pmi	= pPlug->pmi;
	p->pinfo= pPlug->pmi->pCB->GetMachineInfo(pPlug->pmi->ThisMac);
}

bool CInternalPCtrlCallbacks::IsSongPlaying()
{
/*	char txt[256];
	sprintf(txt,"%i",pPlug->pmi->pCB->GetStateFlags());
	MessageBox(NULL,txt,"",MB_OK);
*/	return (pPlug->pmi->pCB->GetStateFlags() & SF_PLAYING) != 0;
}



void CInternalDataRead::Read(void *data, int length)
{
	memcpy(data, (void *)((char *)d + DataHead), length);

	DataHead += length;
}

int CInternalDataRead::ReadInt()
{
	int t;
	Read(&t, sizeof(int));
	return t;
}

short CInternalDataRead::ReadShort()
{
	short t;
	Read(&t, sizeof(short));
	return t;
}

char CInternalDataRead::ReadChar()
{
	char t;
	Read(&t, sizeof(char));
	return t;
}

void CInternalDataRead::ReadASCIIZ(char *c)
{
	char *t = c;
	char k;
	do
	{
		*t = k = ReadChar();
		t++;
	}
	while (k != 0);
}


void CInternalDataWrite::Write(void *data, int length)
{
	if (DataHead + length < 8192)
	{
		memcpy(curr->data + DataHead, data, length);
		DataHead += length;
		curr->ActualLength += length;
	}
	else
	{
		int newlen = length - (8192 - DataHead);
		void *newdata = (void*)((char*)data + (8192 - DataHead));
		memcpy(curr->data + DataHead, data, 8192 - DataHead);
		curr->ActualLength = 8192;

		CDataChunk *ch = new CDataChunk;
		ch->ActualLength = DataHead = newlen;
		memcpy(ch->data, newdata, newlen);
		curr->next = ch;
		curr = ch;
	}

	DataLen += length;
}

void CInternalDataWrite::Write(int i)
{
	Write(&i, sizeof(int));
}

void CInternalDataWrite::Write(short i)
{
	Write(&i, sizeof(short));
}

void CInternalDataWrite::Write(char i)
{
	Write(&i, sizeof(char));
}

void CInternalDataWrite::WriteASCIIZ(char *c)
{
	char *t = c;
	char k;
	do
	{
		k = *t;
		Write(k);
		t++;
	}
	while (k != 0);
}

