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

#include "drumkit.h"
#include <windows.h>
#include "resource.h"
#include <commctrl.h>

#include "common.h"

#include "kitinfodlg.h"

#include "sgrid.h"
#include "hermite.h"
#include "24bit.h"

CDrumkit::CDrumkit()
{
	Reset();
}

CDrumkit::~CDrumkit()
{
}

void CDrumkitSample::Reset()
{
	Name[0] = 0;
	for (CDrumkitLayer **p = layers.begin(); p < layers.end(); p++) if (*p) delete *p;

	layers.clear();
}

void CDrumkit::Reset()
{
	Name[0] = 0;

	for (int i=0; i<200; i++)
		sample[i].Reset();
}

CDrumkitSample::CDrumkitSample()
{
	Reset();
}

CDrumkitSample::~CDrumkitSample()
{
	for (CDrumkitLayer **p = layers.begin(); p < layers.end(); p++) if (*p) delete *p;
}

bool CDrumkit::LoadFromFile(char *_FileName)
{
	FILE *f = fopen(_FileName, "rb");
	if (f)
	{
		char txt[5];
		fread(txt, 1, 4, f);
		txt[4] = 0;
		if (strcmp(txt, "PSID") == 0)
			LoadPSIFromFile(f,_FileName);
		else
		{
			char basepath[256];
			strcpy(basepath,_FileName);
			strcat(basepath,".basepath");
			FILE *fbp = fopen(basepath,"rb");
			if (fbp)
			{
				int i = fread(basepath,1,255,fbp);
				basepath[i] = 0;
				fclose(fbp);
			}
			else
			{	
				strcpy(basepath, _FileName);
				char *c = basepath;
				while (*c) c++;
				while (*c != '.' && c > basepath) c--;
				strcpy(c," samples\\");
			}

			if (!LoadBatKitFromFile(f,basepath))
			{
				fclose(f);
				return false;
			}

			char bpf[256];
			strcpy(bpf,_FileName);
			strcat(bpf,".basepath");
			fbp = fopen(bpf,"wb");
			if (fbp)
			{
				fwrite(basepath,1,strlen(basepath),fbp);
				fclose(fbp);
			}
		}

		char *c = _FileName;
		while (strstr(c,"\\"))
		{
			c = strstr(c,"\\") + 1;
//OutputDebugString(c);
		}

		strcpy(Name, c);
		strcpy(FileName, _FileName);

		fclose(f);
		return true;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////

void CDrumkit::Load(CMachineDataInput * const pi)
{
	byte version;
	pi->Read(version);
	if (version == 3)
	{
		ReadString(Name, pi);
		ReadString(FileName, pi);
		ReadString(Author, pi);
		ReadString(Comment, pi);
		pi->Read((int &)nKitType);

		for (int c=0; c<=200; c++)
		{
			dbgprintf("sample %i",c);
			CDrumkitSample *ps = &sample[c];
			ReadString(ps->Name, pi);
			int nlayers; pi->Read(nlayers);

			for (int l=0; l<nlayers; l++)
			{
				byte type; pi->Read(type);

				CDrumkitLayer *plyr;
				switch (type)
				{
				case 0: plyr = new CDrumkitLayerT<short>; break;
				case 1: plyr = new CDrumkitLayerT<int24>; break;
				case 2: plyr = new CDrumkitLayerT<int>;   break;
				case 3: plyr = new CDrumkitLayerT<float>; break;
				default:plyr = new CDrumkitLayer;		  break;
				}

				plyr->SongLoad(pi);

				ps->layers.push_back(plyr);




/*				int numSamples;
				pi->Read(numSamples);
				for (int j=0; j<numSamples; j++)
				{
					float f;
					pi->Read(f);
					samp.data.push_back(f);
				}

				SampleClass[c].samples.push_back(samp);
*/			}
		}
	}

}

void CDrumkit::Save(CMachineDataOutput * const po, int mode)
{
	po->Write((byte)3); // version
	
	WriteString(Name, po);
	WriteString(FileName, po);
	WriteString(Author, po);
	WriteString(Comment, po);
	po->Write((int)nKitType);

	for (int c=0; c<=200; c++)
	{
		CDrumkitSample *ps = &sample[c];
		WriteString(ps->Name, po);
		po->Write((int)ps->layers.size());

		for (CDrumkitLayer **pp = ps->layers.begin(); pp < ps->layers.end(); pp++)
		{
			po->Write((*pp)->GetDataType());

			if (mode == 2 && !(*pp)->bDataLoaded)
				(*pp)->LoadWaveData();

			(*pp)->SongSave(po);
		}



/*		WriteString(SampleClass[c].name, po);
		po->Write((int)SampleClass[c].samples.size());
		for (CDrumkitSample *p = SampleClass[c].samples.begin();
			p < SampleClass[c].samples.end(); p++)
		{
			WriteString(p->Name, po);
			po->Write(p->Tone);
			po->Write(p->Volume);
			po->Write(p->nSampleRate);
			po->Write(p->bStereo);
			po->Write((int)p->data.size());
			for (float *q = p->data.begin(); q < p->data.end(); q++)
				po->Write(*q);
		}
*/	}

}

CDrumkitLayer *CDrumkitSample::GetLayer(float vel)
{
	if (layers.empty()) return NULL;

	int nvel = (int)(vel * 64.0f);

	CDrumkitLayer *pRet = NULL;

	for (CDrumkitLayer **p = layers.begin(); p < layers.end() && !pRet; p++)
	{
		if ((*p)->nVelMin <= nvel && nvel <= (*p)->nVelMax)
			pRet = *p;
	}

	if (!pRet)
	{
		CDrumkitLayer **pClosest = layers.begin();
		int nDist = min(abs(nvel - (*pClosest)->nVelMin), abs(nvel - (*pClosest)->nVelMax));

		for (p = layers.begin(); p < layers.end(); p++)
		{
			int d = min(abs(nvel - (*p)->nVelMin), abs(nvel - (*p)->nVelMax));
			if (d < nDist)
			{
				d = nDist;
				pClosest = p;
			}
		}

		pRet = *pClosest;
	}

	if (!pRet->bDataLoaded)
		if (!pRet->LoadWaveData()) return NULL;

	return pRet;
}


void CDrumkit::ShowInfoDlg(mi *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CKitInfoDlg dlg;

	dlg.pmi = pmi;
	dlg.pKit = this;
	dlg.DoModal();
}

int CDrumkitSample::GetSize(int mode)
{
	int s = 0;
	for (CDrumkitLayer **pp = layers.begin(); pp < layers.end(); pp++)
	{
		s += (*pp)->GetSize(mode);
		char i = (*pp)->GetDataType();
		char txt[16];
		sprintf(txt,"%i",i);
		OutputDebugString(txt);
	}
	return s;
}

int CDrumkit::GetSize(int mode)
{
	if (mode == 0)
		return 256;
	else
	{
		int s = 0;
		for (int i=1; i<=200; i++)
			s += sample[i].GetSize(mode);
		return s;
	}
}

char *FormatSize(int size);

char *DataFormatName(char d)
{
	switch (d)
	{
	case 0: return "16-bit int";
	case 1: return "24-bit int";
	case 2: return "32-bit int";
	case 3: return "32-bit float";
	default: return "Unknown";
	}
}


void CDrumkitLayer::GetInfo(char *txt)
{
	sprintf(txt,
		"Sample data file: %s\r\n"
		"Data size: %s\r\n"
		"Sample rate: %iHz\tStereo: %s\r\n"
		"Data format: %s",
		dataFileName, FormatSize(datasize), nSampleRate,
		bStereo ? "Yes" : "No", DataFormatName(GetDataType()));
}

void CDrumkitSample::GetInfo(char *txt)
{
	sprintf(txt,
		"Name: %s\r\n"
		"Layers: %i\r\n"
		"Data in memory: %s\tData on disk:",
		Name, layers.size(), FormatSize(GetSize(1)));
	sprintf(txt, "%s %s", txt, FormatSize(GetSize(2)));
}
