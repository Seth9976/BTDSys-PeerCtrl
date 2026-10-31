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

#include <windows.h>
#include <mmreg.h>
#include "common.h"
#include "resource.h"
#include "24bit.h"
#include "drumkit.h"
#include <vector>
using namespace std;

struct CAttrib
{
	char name[1024];
	char value[1024];
};

struct CTag
{
	int linenum;
	char name[1024];
	bool closingtag;
	bool selfclosing;
	vector<CAttrib> attribs;

	CAttrib *FindAttribute(char *aname)
	{
		for (CAttrib *a = attribs.begin(); a < attribs.end(); a++)
		{
			if (stricmp(a->name, aname) == 0)
				return a;
		}
		return NULL;
	};
};

int LexXML(FILE *f, vector<CTag> *tags);

CDrumkitLayer *LoadSample(char *fname);
bool LocateSample(char *name, char *path);

bool CDrumkit::LoadBatKitFromFile(FILE *f, char *basepath)
{
	char stat[1024];
	CLoadStatusDlg dlg;

	dlg.SetWindowText("BTDSys SampleGrid - Loading Battery Drumkit");
	strcpy(stat, "Loading...");	PUT_STATUS;

	fseek(f,0,SEEK_SET);
	char txt[256];

	Reset();
	nKitType = ktBattery;

	strcpy(stat, "Lexing XML");	PUT_STATUS;
	vector<CTag> tags;
	int res = LexXML(f,&tags);
	if (res)
	{
		sprintf(txt,"Battery kit loader: Syntax error on line %i",res);
		MessageBox(NULL,txt,"SampleGrid",MB_OK);
		sprintf(stat, "Error point a"); PUT_STATUS;
		return false;
	}

	strcpy(stat, "Stepping through tags");	PUT_STATUS;
	for (CTag *p = tags.begin(); p < tags.end(); p++)
	{
		if (stricmp(p->name, "SampleSlot") == 0 && !p->closingtag)
		{
			strcpy(stat, "SampleSlot found");	PUT_STATUS;
			CAttrib *a = p->FindAttribute("index");
			if (!a)
			{
				sprintf(txt,"Battery kit loader: SampleSlot has no index on line %i",p->linenum);
				MessageBox(NULL,txt,"SampleGrid",MB_OK);
				sprintf(stat, "Error point b"); PUT_STATUS;
				return false;
			}

			int idx = atoi(a->value) + 1;
			if (idx > 200)
			{
				sprintf(txt,"Battery kit loader: SampleSlot exceeds maximum (199) on line %i",p->linenum);
				MessageBox(NULL,txt,"SampleGrid",MB_OK);
				sprintf(stat, "Error point b2"); PUT_STATUS;
				return false;
			}

			if (sample[idx].HasData())
			{
				sprintf(txt,"Battery kit loader: SampleSlot has non-unique index on line %i",p->linenum);
				MessageBox(NULL,txt,"SampleGrid",MB_OK);
				sprintf(stat, "Error point c"); PUT_STATUS;
				return false;
			}

			sample[idx].Reset();
			a = p->FindAttribute("label");
			if (!a)
			{
				sprintf(txt,"Battery kit loader: SampleSlot has no label on line %i",p->linenum);
				MessageBox(NULL,txt,"SampleGrid",MB_OK);
				sprintf(stat, "Error point d"); PUT_STATUS;
				return false;
			}
			strcpy(sample[idx].Name, a->value);

			a = p->FindAttribute("volume");
			float smpvol  = a ? atof(a->value) : 1.0f;
			a = p->FindAttribute("pitch");
			float smprate = a ? atof(a->value) : 1.0f;

			for (p++; p < tags.end() && !p->closingtag; p++)
			{
				if (stricmp(p->name, "Sample") == 0)
				{
					strcpy(stat, "Sample found");	PUT_STATUS;
					a = p->FindAttribute("file");
					if (!a)
					{
						sprintf(txt,"Battery kit loader: Sample has no file on line %i",p->linenum);
						MessageBox(NULL,txt,"SampleGrid",MB_OK);
						sprintf(stat, "Error point e"); PUT_STATUS;
						return false;
					}

					char wavname[256];
					strcpy(wavname,basepath);
					strcat(wavname,a->value);
					sprintf(stat,"Checking wave %s",a->value); PUT_STATUS;
					CDrumkitLayer *lyr = LoadSample(wavname);
					if (!lyr)
					{
						if (!LocateSample(a->value,basepath))
						{
							sprintf(txt,"Battery kit loader: Can't open wave file '%s' on line %i",a->value, p->linenum);
							MessageBox(NULL,txt,"SampleGrid",MB_OK);
							sprintf(stat, "Error point f1"); PUT_STATUS;
							return false;
						}

						strcpy(wavname,basepath);
						strcat(wavname,a->value);
						lyr = LoadSample(wavname);
						if (!lyr)
						{
							sprintf(txt,"Battery kit loader: Can't open wave file '%s' on line %i",a->value, p->linenum);
							MessageBox(NULL,txt,"SampleGrid",MB_OK);
							sprintf(stat, "Error point f2"); PUT_STATUS;
							return false;
						}
					}

					a = p->FindAttribute("lowVelo");
					lyr->nVelMin = a ? atoi(a->value) : 0;
					a = p->FindAttribute("highVelo");
					lyr->nVelMax = a ? atoi(a->value) : 127;
					a = p->FindAttribute("layerVolume");
					lyr->fVol = a ? atof(a->value) : 1.0f;
					lyr->fVol *= smpvol;
					lyr->fRate = smprate;

					sample[idx].layers.push_back(lyr);
				}
			}
			strcpy(stat, "End of SampleSlot");	PUT_STATUS;
		}
	}

	sprintf(stat, "Bye bye"); PUT_STATUS;

	return true;
}


bool LocateSample(char *name, char *path)
{
	char FName[MAX_PATH];
//	FName[0] = 0;
	strcpy(FName,name);

	OPENFILENAME ofn;
	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= NULL;
	ofn.hInstance			= dllInstance;
	ofn.lpstrFilter			= "Wav files\0*.wav\0"
								"All files\0*.*\0";
	ofn.lpstrCustomFilter	= NULL;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= FName;
	ofn.lpstrFileTitle		= NULL;
	ofn.lpstrInitialDir		= path;
	ofn.nMaxFile			= MAX_PATH;
	char title[256];
	sprintf(title, "Locate file '%s'", name);
	ofn.lpstrTitle			= title;
	ofn.Flags				= OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt			= NULL;

	if (GetOpenFileName(&ofn))
	{
		strcpy(path, FName);
		char *c = path;
		while (*c) c++;
		while (*c != '\\' && c > path) c--;
		c++;
		strcpy(name,c);
		*c = 0;
		dbgprintf("'%s' |||| '%s'",name,path);
		return true;
	}
	else
		return false;
}

inline bool cmp4(char *a, char *b)
{
	return a[0] == b[0] 
		&& a[1] == b[1] 
		&& a[2] == b[2] 
		&& a[3] == b[3];
}


bool FindWaveFormat(FILE *f, WAVEFORMATEX *wf)
{
	fseek(f,12,SEEK_SET);

	while (!feof(f))
	{
		char buf[4];
		fread(buf,1,4,f);
		if (cmp4(buf,"fmt "))
		{
			fread(buf,1,4,f);
			fread(wf,1,16,f);
			return true;
		}
		else
		{
			int len;
			fread(&len,4,1,f);
			fseek(f,len,SEEK_CUR);
		}
	}

	return false;
}

int FindWaveData(FILE *f)
{
	fseek(f,12,SEEK_SET);

	while (!feof(f))
	{
		char buf[4];
		fread(buf,1,4,f);
		int len;
		fread(&len,4,1,f);
		if (cmp4(buf,"data"))
			return len;
		else
			fseek(f,len,SEEK_CUR);
	}

	return 0;
}

CDrumkitLayer *LoadSample(char *fname)
{
	FILE *f = fopen(fname,"rb");
	if (!f) return NULL;

	char buf[4];
	fread(buf,1,4,f);
	if (!cmp4(buf,"RIFF")) {fclose(f); return NULL;}
	fread(buf,1,4,f);
	fread(buf,1,4,f);
	if (!cmp4(buf,"WAVE")) {fclose(f); return NULL;}

	WAVEFORMATEX wf;
	if (!FindWaveFormat(f,&wf)) {fclose(f); return NULL;}

	int datalen = FindWaveData(f);
	if (!datalen) {fclose(f); return NULL;}

	CDrumkitLayer *lyr;
	if (wf.wFormatTag == 0x0003) // IEEE float
	{
		switch (wf.wBitsPerSample)
		{
		case 32:
			lyr = new CDrumkitLayerT<float>;
			lyr->fVolCompensate = 32768.0f;
			break;

		default:
			fclose(f); return NULL;
		}
	}
	else if (wf.wFormatTag == 0x0001) // PCM
	{
		switch (wf.wBitsPerSample)
		{
		case 16:
			lyr = new CDrumkitLayerT<short>;
			lyr->fVolCompensate = 1.0f;
			break;

		case 32:
			lyr = new CDrumkitLayerT<int>;
			lyr->fVolCompensate = 1.0f / 32768.0f;
			break;

		case 24:
			lyr = new CDrumkitLayerT<int24>;
			lyr->fVolCompensate = 1.0f / 32768.0f;
			break;

		default:
			fclose(f); return NULL;
		}
	}
	else {fclose(f); return NULL;}

	lyr->bStereo = (wf.nChannels != 1);
	lyr->nSampleRate = wf.nSamplesPerSec;
//	lyr->LoadWaveData(f,datalen);
	lyr->bDataLoaded = false;
	strcpy(lyr->dataFileName, fname);
	lyr->datasize = datalen;
	lyr->dataoffset = ftell(f);

	fclose(f);

	return lyr;
}

void CDrumkit::FlushSampleMemory()
{
	for (int i=0; i<201; i++)
	{
		for (CDrumkitLayer **pp = sample[i].layers.begin(); pp < sample[i].layers.end(); pp++)
		{
			(*pp)->FlushWaveData();
		}
	}


}






///////////////////////////////////////////////////////////////////////////////////////////////
// Ultra-ghetto XML lexer
// I don't even want to call it a "parser", that's how ghetto it is... but it does the job.

bool is_whitespace(char c)
{
	switch (c)
	{
	case ' ':
	case '\t':
	case 10:
	case 13:
		return true;

	default:
		return false;
	}
}

bool is_alphanum(char c)
{
	if (c >= 'a' && c <= 'z') return true;
	if (c >= 'A' && c <= 'Z') return true;
	if (c >= '0' && c <= '9') return true;
	switch (c)
	{
	case '-':
	case '_':
	case '.':
	case '?':
	case '!':
	case '*':
		return true;
	
	default:
		return false;
	}
}

int LexXML(FILE *f, vector<CTag> *tags)
{
	int state = 1;
	int bufhead = 0;

	CTag tag;
	CAttrib att;

	int line = 1;

	while (state && !feof(f))
	{
		char c;
		fread(&c,1,1,f);

		if (!feof(f))
		{
//			dbgprintf("Character %i (%c), in state %i",c,(c>=32)?c:'?',state);

			if (c == 10 || c == 13) line++;

			switch (state)
			{
			case 1:
				if (c == '<')
				{
					state = 2;
					bufhead = 0;
					tag.closingtag = tag.selfclosing = false;
					tag.attribs.clear();
					tag.linenum = line;
				}
				else if (is_whitespace(c))
				{
				}
				else
					state = 0;
				break;

			case 2:
				if (is_alphanum(c))
				{
					tag.name[bufhead] = c;
					bufhead++;
				}
				else if (c == '/' && bufhead == 0)
				{
					state = 9;
					tag.closingtag = true;
				}
				else if (c == '>')
				{
					tag.name[bufhead] = 0;
					tags->push_back(tag);
					state = 1;
				}
				else if (is_whitespace(c))
				{
					tag.name[bufhead] = 0;
					bufhead = 0;
					state = 3;
				}
				else
					state = 0;
				break;

			case 3:
				if (c == '>')
				{
					tags->push_back(tag);
					state = 1;
				}
				else if (c == '/' || c == '?')
				{
					tag.selfclosing = true;
					state = 8;
				}
				else if (is_alphanum(c))
				{
					att.name[0] = c;
					bufhead = 1;
					state = 4;
				}
				else if (is_whitespace(c))
				{
				}
				else
					state = 0;
				break;

			case 4:
				if (is_alphanum(c))
				{
					att.name[bufhead] = c;
					bufhead++;
				}
				else if (c == '=')
				{
					att.name[bufhead] = 0;
					state = 5;
				}
				else
					state = 0;
				break;

			case 5:
				if (c == '"')
				{
					bufhead = 0;
					state = 6;
				}
				else
					state = 0;
				break;

			case 6:
				if (c == '"')
				{
					att.value[bufhead] = 0;
					tag.attribs.push_back(att);
					state = 3;
				}
				else
				{
					att.value[bufhead] = c;
					bufhead++;
				}
				break;

			case 8:
				if (c == '>')
				{
					tags->push_back(tag);
					state = 1;
				}
				else
					state = 0;
				break;

			case 9:
				if (is_alphanum(c))
				{
					tag.name[bufhead] = c;
					bufhead++;
				}
				else if (c == '>')
				{
					tag.name[bufhead] = 0;
					tags->push_back(tag);
					state = 1;
				}
				else
					state = 0;
				break;
			} // end switch
		} // end if
	} //end while

	if (state == 1)
		return 0;
	else
		return line;
}
