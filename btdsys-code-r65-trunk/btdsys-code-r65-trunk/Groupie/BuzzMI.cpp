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

#include "stdafx.h"
#include "BuzzMI.h"
#include "DSPStuff.h"
#include "dbgprintf.h"

#include "groupie.h"
#include "ParamDlg.h"

CMachine::CMachine()
{
	hm = NULL;
	pMI = NULL;
	pEx = NULL;
	pInfo = NULL;
	pMDKI = NULL;
	pMDKMI = NULL;
	pMDKEx = NULL;
	pParamDlg = NULL;
	pTrackState = NULL;
	pGlobalState = NULL;
	bSpecial = false;
	bSelected = false;
	bNoOut = false;
	fBMX = NULL;
	nExceptions = 0;
}

CMachine::~CMachine()
{
	if (!bSpecial)
	{
		if (pEx)
			delete pEx;
		if (pMI)
			delete pMI;
		if (pCB)
			delete pCB;
		if (pMDKEx)
			delete pMDKEx;
		if (pTrackState)
			delete[] pTrackState;
		if (pGlobalState)
			delete[] pGlobalState;

		if (hm)
			FreeLibrary(hm);
	}
}

void CMachine::UnloadMachine()
{
	if (!bSpecial)
	{
		bSpecial = true;

		try { if (pEx)			delete pEx;				} catch (...) {};	pEx = NULL;
		try { if (pMI)			delete pMI;				} catch (...) {};	pMI = NULL;
		try { if (pCB)			delete pCB;				} catch (...) {};	pCB = NULL;
		try { if (pMDKEx)		delete pMDKEx;			} catch (...) {};	pMDKEx = NULL;
		try { if (pTrackState)	delete[] pTrackState;	} catch (...) {};	pTrackState = NULL;	
		try { if (pGlobalState)	delete[] pGlobalState;	} catch (...) {};	pGlobalState = NULL;	

		try { if (hm)			FreeLibrary(hm);		} catch (...) {};	hm = NULL;

		static CMachineInfo const macThruInfo =
		{
			MT_EFFECT,						//Machine type
			-1,								//Interface version
			0,								//Flags
			0,								//Min tracks
			0,								//Max tracks
			0,								//Number of global parameters
			0,								//Number of track parameters
			{NULL},							//Pointer to parameters
			0,								//Number of attributes
			{NULL},							//Pointer to attributes
			"BTDSys Groupie - Thru Dummy",	//Full name
			"thru",							//Short name
			"Ed Powley (BTDSys)",			//Author
			"About..."						//Commands
		};

		pInfo = &macThruInfo;
		pMI = THRU_MACHINE;
		strcpy(dllname,"(internal thru)");
	}
}

void CMachine::LogException(char *where, char *file, int line)
{
	dbgprintf("GROUPIE: Exception detected in %s function of %s (%s) [%s:%i]",where,name,dllname,file,line);

	nExceptions++;
	if (nExceptions >= 2)
	{
		UnloadMachine();

		char txt[1024];
		sprintf(txt,"Exceptions detected in %s function of %s (%s) [%s:%i]. "
			"This machine has been unloaded. Please click OK. Then right-click the machine and use the Replace "
			"or Delete command.",
			where,name,dllname,file,line);
		MessageBox(NULL,txt,NULL,MB_OK | MB_ICONERROR);

		if (pHost->pMacView) pHost->pMacView->RedrawWindow();
	}
}

void CMachine::ChangeParam(int par, int track, int value)
{
	if (track == -1)
	{
		byte *b = (byte *)pMI->GlobalVals;
		for (int i=0; i<par; i++)
		{
			b++;
			if (pInfo->Parameters[i]->Type == pt_word)
				b++;
		}

		if (pInfo->Parameters[par]->Type == pt_word)
			*(word *)b = (word)value;
		else
			*b = (byte)value;
	}
	else
	{
		int tracksize = 0;
		for (int i = pInfo->numGlobalParameters;
			 i < pInfo->numGlobalParameters+pInfo->numTrackParameters; i++)
		{
			tracksize++;
			if (pInfo->Parameters[i]->Type == pt_word)
				tracksize++;
		}

		byte *b = (byte *)pMI->TrackVals;
		b += track * tracksize;

		for (i = 0; i < par; i++)
		{
			b++;
			if (pInfo->Parameters[pInfo->numGlobalParameters + i]->Type == pt_word)
				b++;
		}

		if (pInfo->Parameters[pInfo->numGlobalParameters + par]->Type == pt_word)
			*(word *)b = (word)value;
		else
			*b = (byte)value;
	}
}

void CMachine::ChangeParamFromHost(int par, int track, int value)
{
	const CMachineParameter *ppar;
	if (track == -1)
		ppar = pInfo->Parameters[par];
	else
		ppar = pInfo->Parameters[pInfo->numGlobalParameters + par 
									+ track*pInfo->numTrackParameters];

	int min = ppar->MinValue;
	int max = ppar->MaxValue;
	if (ppar->Type == pt_switch)
	{
		min = 0;
		max = 1;
	}

	if (value >= min && value <= max)
	{
		// state
		if (track == -1 && pGlobalState && (ppar->Flags & MPF_STATE))
			pGlobalState[par] = (word) value;
		else if (track != -1 && pTrackState && (ppar->Flags & MPF_STATE))
			pTrackState[par + track*pInfo->numTrackParameters] = (word) value;

		// GUI
		if (pParamDlg && ppar->Type != pt_note)
		{
			if (track == -1)
			{
				pParamDlg->vpSliders[par]->SetPos(value);
				pParamDlg->UpdateParamValueLabel(par);
			}
			else
			{
				pParamDlg->vpSliders[pInfo->numGlobalParameters + par]->SetPos(value);
				pParamDlg->UpdateParamValueLabel(pInfo->numGlobalParameters + par);
			}

		}

		// machine
		ChangeParam(par, track, value); //phew that was easy
	}
}

CMachine *mi::FindMachine(char const *name)
{
	for (vector<CMachine*>::iterator pp = vpMacs.begin();
		 pp < vpMacs.end(); pp++)
	{
		if (strcmp((*pp)->name, name) == 0)
			return *pp;
	}

	return NULL;
}

CConnection *mi::TryConnectMachines(CMachine *m1, CMachine *m2)
{
	bool bCanConn = true;

	if (m1 == m2)
		return NULL;

	if (m1->bNoOut)
		return NULL;

	if (!m2->bIsEffect)
		return NULL;

	for (vector<CConnection>::iterator p = vConns.begin(); 
		 p < vConns.end() && bCanConn; p++)
	{
		if ((p->pMac1 == m1 && p->pMac2 == m2) ||
			(p->pMac1 == m2 && p->pMac2 == m1))
			bCanConn = false;
	}

	if (bCanConn)
	{
		//look for feedback loops
		if (ExistPath(m2,m1))
		{
			return NULL;
		}

		CConnection c;
		c.pMac1 = m1;
		c.pMac2 = m2;

		vConns.push_back(c);

/*		if (c.pMac2->pEx)
		{
			c.pMac2->pEx->AddInput(c.pMac1->name, true);
		}
*/
		CConnection *p = vConns.end();
		return --p;
	}
	else
		return NULL;
}

bool mi::ExistPath(CMachine *m1, CMachine *m2)
{
	for (CConnection *p = vConns.begin(); p < vConns.end(); p++)
	{
		if (p->pMac1 == m1)
		{
			if (p->pMac2 == m2) return true;
			if (ExistPath(p->pMac2,m2)) return true;
		}
	}

	return false;
}

CMachine *mi::TryAddMachine(char const *dllname)
{
	CMachine *newm = new CMachine;

	if (newm->LoadMachine(dllname,this))
	{
		newm->InitMachineNoFileData();
		newm->pHost = this;
		vpMacs.push_back(newm);
		return newm;
	}
	else
	{
		delete newm;
		return NULL;
	}
}

bool mi::TryDeleteMachine(CMachine **ppm)
{
	OutputDebugString((*ppm)->name);

	if ((*ppm)->bSpecial && !(*ppm)->IsThruMachine())
		return false;

	for (int a=0; a<4; a++) for (int t=0; t<MAX_TRACKS; t++)
		if (assigns[a][t].pTargetMac == *ppm) assigns[a][t].pTargetMac = NULL;

	for (vector<CConnection>::iterator p = vConns.begin();
		 p < vConns.end(); p++)
	{
		if (p->pMac1 == *ppm || p->pMac2 == *ppm)
		{
			vConns.erase(p);
			p--;
		}
	}

	delete *ppm;

	vpMacs.erase(ppm);

	return true;
}


class CGSMI_DataOut : public CMachineDataOutput
{
public:
	HMENU hMenu;
	int nID;
	virtual void Write(void *pbuf, int const numbytes)
	{
		AppendMenu(hMenu, MF_STRING, nID, (char *)pbuf);
		nID++;
	}
};


void CMachine::GetSubMenuItems(HMENU hMenu, int nMenu, int nBaseID)
{
	CGSMI_DataOut dat;
	dat.hMenu = hMenu;
	dat.nID = nBaseID;

	if (pEx)
		pEx->GetSubMenu(nMenu,&dat);

}

void CMachine::MainWindowIsClosing()
{
	if (pParamDlg)
	{
		pParamDlg->DestroyWindow();
		pParamDlg = NULL;
	}
}

class CIntDataOut : public CMachineDataOutput
{
public:
	CIntDataOut(FILE *_f)
	{
		f = _f; 
		nBeginPos = ftell(f) + 4; 
		fwrite(&nBeginPos,4,1,f);
	}

	virtual ~CIntDataOut()
	{
		int i = ftell(f);
		fseek(f,nBeginPos-4,SEEK_SET); 
		nBeginPos = i - nBeginPos;
		fwrite(&nBeginPos,4,1,f);
		fseek(f,i,SEEK_SET);
	}

	virtual void Write(void *pbuf, int const numbytes)
	{
		fwrite(pbuf,numbytes,1,f);
	}

private:
	FILE *f;
	int nBeginPos;
};

class CIntDataIn : public CMachineDataInput
{
public:
	CIntDataIn(FILE *_f)
	{
		f = _f; 
		fread(&nNumBytes,4,1,f);
		nBeginPos = ftell(f);
	}

	virtual ~CIntDataIn()
	{
		fseek(f,nBeginPos + nNumBytes,SEEK_SET); 
	}

	virtual void Read(void *pbuf, int const numbytes)
	{
		fread(pbuf,numbytes,1,f);
	}

private:
	FILE *f;
	int nBeginPos;
	int nNumBytes;
};


bool mi::SaveToBMW(FILE *f)
{
#	ifdef _GROUPIE_GEN
		CMachine macIn;
#	endif

	char buf[1024];
	int sectionbegin;
	int sectionend;
	int n,i;
	int zero = 0;
	CMachine **pp;

	int numsec = 0;

	fwrite("Buzz",4,1,f);
	fwrite(buf,4 + 12*31,1,f); // come back to fill in the section table later

	sectionbegin = ftell(f);
	//PARA
	n = vpMacs.size() - 1;
	fwrite(&n,4,1,f);
	for (pp = vpMacs.begin(); pp < vpMacs.end(); pp++)
	{
		CMachine *p = *pp;

		if (p == &macIn)
		{
			fprintf(f,"GROUPIE_INPUT"); fwrite(&zero,1,1,f);
			fprintf(f,"cheapo do-nothing"); fwrite(&zero,1,1,f);
			fwrite(&zero,4,1,f); //no global pars
			fwrite(&zero,4,1,f); //no track pars
		}
		else if (p->bSpecial && p->IsThruMachine())
		{
			fprintf(f,p->name); fwrite(&zero,1,1,f);
			fprintf(f,"cheapo do-nothing"); fwrite(&zero,1,1,f);
			fwrite(&zero,4,1,f); //no global pars
			fwrite(&zero,4,1,f); //no track pars
		}
		else if (!p->bSpecial)
		{
			fprintf(f,p->name); fwrite(&zero,1,1,f);
			fprintf(f,p->pInfo->Name); fwrite(&zero,1,1,f);
			fwrite(&p->pInfo->numGlobalParameters,4,1,f);
			fwrite(&p->pInfo->numTrackParameters,4,1,f);
			for (i=0; i<p->pInfo->numGlobalParameters + p->pInfo->numTrackParameters; i++)
			{
				const CMachineParameter *par = p->pInfo->Parameters[i];
				fwrite(&par->Type,1,1,f);
				fprintf(f,par->Name); fwrite(&zero,1,1,f);
				fwrite(&par->MinValue,4,1,f);
				fwrite(&par->MaxValue,4,1,f);
				fwrite(&par->NoValue,4,1,f);
				fwrite(&par->Flags,4,1,f);
				fwrite(&par->DefValue,4,1,f);
			}
		}
	}

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("PARA",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//MACH
	n = vpMacs.size();
	fwrite(&n,2,1,f);
	fprintf(f,"Master"); fwrite(&zero,1,1,f);
	fwrite(&zero,1,1,f); //type
	fwrite(&macOut.fX,4,1,f);
	fwrite(&macOut.fY,4,1,f);
	fwrite(&zero,4,1,f); //size of custom data
	fwrite(&zero,2,1,f); //num attributes
	n = 0; fwrite(&n,2,1,f); //parameter: volume
	n = pMasterInfo->BeatsPerMin; fwrite(&n,2,1,f); //parameter: bpm
	n = pMasterInfo->TicksPerBeat; fwrite(&n,1,1,f); //parameter: tpb
	fwrite(&zero,2,1,f); //num tracks
	macOut.nPosInVector = 0;

	int posinvec = 1;

	for (pp = vpMacs.begin(); pp < vpMacs.end(); pp++)
	{
		CMachine *p = *pp;
		if (p == &macIn)
		{
			fprintf(f,"GROUPIE_INPUT"); fwrite(&zero,1,1,f);
			n = 2; fwrite(&n,1,1,f); //type
			fprintf(f,"cheapo do-nothing"); fwrite(&zero,1,1,f);
			fwrite(&macIn.fX,4,1,f);
			fwrite(&macIn.fY,4,1,f);
			fwrite(&zero,4,1,f); //size of custom data
			fwrite(&zero,2,1,f); //num attributes
			fwrite(&zero,2,1,f); //num tracks

			macIn.nPosInVector = posinvec++;
		}
		else if (p->bSpecial && p->IsThruMachine())
		{
			fprintf(f,p->name); fwrite(&zero,1,1,f);
			n = 2; fwrite(&n,1,1,f); //type
			fprintf(f,"cheapo do-nothing"); fwrite(&zero,1,1,f);
			fwrite(&p->fX,4,1,f);
			fwrite(&p->fY,4,1,f);
			fwrite(&zero,4,1,f); //size of custom data
			fwrite(&zero,2,1,f); //num attributes
			fwrite(&zero,2,1,f); //num tracks

			p->nPosInVector = posinvec++;
		}
		else if (!p->bSpecial)
		{
			fprintf(f,p->name); fwrite(&zero,1,1,f);
			int n = (p->bIsEffect ? 2 : 1); fwrite(&n,1,1,f);
			strcpy(buf,p->dllname);
			buf[strlen(buf)-4] = 0;
			fprintf(f,buf); fwrite(&zero,1,1,f);
			fwrite(&p->fX,4,1,f);
			fwrite(&p->fY,4,1,f);

			CIntDataOut *pido = new CIntDataOut(f);
			p->Save(pido,__FILE__,__LINE__);
			delete pido;

			fwrite(&p->pInfo->numAttributes,2,1,f);
			int *AttrVals = p->GetAttrVals();
			for (i=0; i<p->pInfo->numAttributes; i++)
			{
				fprintf(f,p->pInfo->Attributes[i]->Name); fwrite(&zero,1,1,f);
				fwrite(&AttrVals[i],4,1,f);
			}

			for (i=0; i<p->pInfo->numGlobalParameters; i++)
			{
//				if (p->pInfo->Parameters[i]->Flags & MPF_STATE)
				{
					int n = p->pGlobalState[i];
					if (p->pInfo->Parameters[i]->Type != pt_word)
						fwrite(&n,1,1,f);
					else
						fwrite(&n,2,1,f);
				}
			}

			fwrite(&p->nTracks,2,1,f);
			for (int t=0; t<p->nTracks; t++)
			{
				int j = 0;
				for (i=0; i<p->pInfo->numTrackParameters; i++)
				{
//					if (p->pInfo->Parameters[i+p->pInfo->numGlobalParameters]->Flags & MPF_STATE)
					{
						int n = p->pTrackState[j];
						if (p->pInfo->Parameters[i+p->pInfo->numGlobalParameters]->Type != pt_word)
							fwrite(&n,1,1,f);
						else
							fwrite(&n,2,1,f);
					}
					j++;
				}
			}

			p->nPosInVector = posinvec++;
		}
	}

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("MACH",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//CONN
	//nb: amp = 0..16384  pan = 0..32768
	n = vConns.size();
	fwrite(&n,2,1,f);
	for (CConnection *p = vConns.begin(); p<vConns.end(); p++)
	{
		fwrite(&p->pMac1->nPosInVector,2,1,f);
		fwrite(&p->pMac2->nPosInVector,2,1,f);
		n = (int)(p->fAmp * 16384.0f);
		fwrite(&n,2,1,f);
		n = (int)(p->fPan * 16384.0f);
		fwrite(&n,2,1,f);
	}

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("CONN",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//PATT
	for (i=0; i<vpMacs.size(); i++)
	{
		fwrite(&zero,2,1,f); //no patterns
		fwrite(&vpMacs[i]->nTracks,2,1,f);
	}

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("PATT",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//SEQU
	n = 16;
	fwrite(&n,4,1,f); //end of song
	fwrite(&zero,4,1,f); //begin of loop
	fwrite(&n,4,1,f); //end of loop
	fwrite(&zero,2,1,f); //number of seqs

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("SEQU",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//WAVT
	fwrite(&zero,2,1,f); //num waves

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("WAVT",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;

	//BLAH
	char *blah = "";
	n = strlen(blah);
	fwrite(&n,4,1,f);
	fwrite(blah,n,1,f);

	//index entry
	sectionend = ftell(f);
	fseek(f,8 + 12*numsec,SEEK_SET);
	fwrite("BLAH",4,1,f);
	fwrite(&sectionbegin,4,1,f);
	n = sectionend - sectionbegin;
	fwrite(&n,4,1,f);
	fseek(f,sectionend,SEEK_SET);
	sectionbegin = sectionend;
	numsec++;


	fseek(f,4,SEEK_SET);
	fwrite(&numsec,4,1,f);

	return true;
}



bool FindSection(FILE *f, char *name)
{
	char buf[5];
	buf[4] = 0;

	fseek(f,4,SEEK_SET);
	int numsec; fread(&numsec,4,1,f);
	for (int i=0; i<numsec; i++)
	{
		fread(buf,4,1,f);
		int offset, len;
		fread(&offset,4,1,f);
		fread(&len,4,1,f);
		if (strcmp(buf,name) == 0)
		{
			fseek(f,offset,SEEK_SET);
			return true;
		}
	}

	return false;
}

void ReadASCIIZ(FILE *f, char *buf)
{
	do
	{
		fread(buf,1,1,f);
	}
	while (*buf++);
}

bool mi::LoadFromBMW(FILE *f)
{
	if (!FindSection(f,"MACH")) {MessageBox(NULL,"Couldn't find MACH","",MB_OK); return false;}

	int i,n;
	char buf[1024];

	int nummacs = 0; fread(&nummacs,2,1,f);

	for (int mac=0; mac<nummacs; mac++)
	{
		ReadASCIIZ(f,buf);
//MessageBox(NULL,buf,"machine",MB_OK);
		n=0; fread(&n,1,1,f); //type
		if (n==0) //master
		{
			fread(&macOut.fX,4,1,f);
			fread(&macOut.fY,4,1,f);
			fseek(f,13,SEEK_CUR); //skip junk
			macOut.nPosInVector = mac;
		}
#		ifndef _GROUPIE_GEN
			else if (strcmp(buf,"GROUPIE_INPUT") == 0)
			{
				ReadASCIIZ(f,buf);
				if (strcmp(buf,"cheapo do-nothing") != 0) {MessageBox(NULL,"Not cheapo donowt",buf,MB_OK); return false;}
				fread(&macIn.fX,4,1,f);
				fread(&macIn.fY,4,1,f);
				fseek(f,8,SEEK_CUR); //skip junk
				macIn.nPosInVector = mac;
			}
#		endif
		else
		{
			char macname[256];
			strcpy(macname,buf);

			char dllname[256];

			ReadASCIIZ(f,dllname);
			strcat(dllname,".dll");
			CMachine *m = new CMachine;
			
			int macstart = ftell(f);

			try
			{
				if (!m->LoadMachine(dllname,this)) {MessageBox(NULL,"Can't load machine",dllname,MB_OK); return false;}
				strcpy(m->name,macname);
				fread(&m->fX,4,1,f);
				fread(&m->fY,4,1,f);

				m->fBMX = f;
				CIntDataIn *pidi = new CIntDataIn(f);

				m->Init(pidi,__FILE__,__LINE__);

				delete pidi;
				m->fBMX = NULL;

				int *AttrVals = m->GetAttrVals();
				for (i=0; i<m->pInfo->numAttributes; i++)
				{
					AttrVals[i] = m->pInfo->Attributes[i]->DefValue;
				}

				int numattr = 0; fread(&numattr,2,1,f);
				for (i=0; i<numattr; i++)
				{
					ReadASCIIZ(f,buf);
					fread(&n,4,1,f);
/*					for (int j=0; j<m->pInfo->numAttributes; j++)
					{
						if (strcmp(m->pInfo->Attributes[j]->Name,buf) == 0)
						{
							m->pMI->AttrVals[j] = n;
						}
					}
*/				}

				if (m->pInfo->numGlobalParameters > 0)
				{
					m->pGlobalState = new word[m->pInfo->numGlobalParameters];
					for (i=0; i<m->pInfo->numGlobalParameters; i++)
					{
						m->pGlobalState[i] = 0;
						if (m->pInfo->Parameters[i]->Type == pt_word)
							fread(&m->pGlobalState[i],2,1,f);
						else
							fread(&m->pGlobalState[i],1,1,f);

						if (!(m->pInfo->Parameters[i]->Flags & MPF_STATE))
							m->pTrackState[i] = m->pInfo->Parameters[i]->NoValue;
					}
				}

				m->nTracks = 0; fread(&m->nTracks,2,1,f);

				if (m->pInfo->numTrackParameters > 0)
				{
					m->pTrackState  = new word[m->pInfo->numTrackParameters * m->pInfo->maxTracks];

					int j=0;
					for (int t=0; t<m->nTracks; t++)
					{
						for (i=0; i<m->pInfo->numTrackParameters; i++)
						{
							m->pTrackState[j] = 0;
							if (m->pInfo->Parameters[m->pInfo->numGlobalParameters + i]->Type == pt_word)
								fread(&m->pTrackState[j],2,1,f);
							else
								fread(&m->pTrackState[j],1,1,f);

							if (!(m->pInfo->Parameters[m->pInfo->numGlobalParameters + i]->Flags & MPF_STATE))
								m->pTrackState[j] = m->pInfo->Parameters[m->pInfo->numGlobalParameters + i]->NoValue;

							j++;
						}
					}
				}

				macstart = 0;

				m->InitParamsAttrs(true);
				m->SetNumTracks(m->nTracks,__FILE__,__LINE__);
				m->AttributesChanged(__FILE__,__LINE__);
				if (m->IsMDK())
				{
					m->bStereoOut = true;
					m->OutputModeChanged(true,__FILE__,__LINE__);
				}

				m->Tick(__FILE__,__LINE__);

				m->ClearParams();

				m->nPosInVector = mac;

				m->pHost = this;

				vpMacs.push_back(m);
			}
			catch (...)
			{
				float x = m->fX;
				float y = m->fY;

				if (m) delete m;
				m = new CMachine;
				if (macstart)
				{
					if (!FindSection(f,"PARA")) {MessageBox(NULL,"Couldn't find PARA","",MB_OK); return false;}

					int nummacs; fread(&nummacs,4,1,f);
					for (i=0; i<nummacs && macstart; i++)
					{
						char macname2[256];
						ReadASCIIZ(f,macname2);

						ReadASCIIZ(f,buf);
						int numg, numt, j, sg, st;
						fread(&numg,4,1,f); fread(&numt,4,1,f);
						sg = numg; st = numt;
						for (j=0; j<numg; j++)
						{
							n=0; fread(&n,1,1,f);
							if (n == pt_word) sg++;
							ReadASCIIZ(f,buf);
							fseek(f,20,SEEK_CUR);
						}
						for (j=0; j<numt; j++)
						{
							n=0; fread(&n,1,1,f);
							if (n == pt_word) st++;
							ReadASCIIZ(f,buf);
							fseek(f,20,SEEK_CUR);
						}

						if (strcmp(macname,macname2) == 0)
						{
							fseek(f,macstart+8,SEEK_SET);
							fread(&n,4,1,f);
							fseek(f,n,SEEK_CUR);
							n = 0; fread(&n,2,1,f);
							for (j=0; j<n; j++)
							{
								ReadASCIIZ(f,buf);
								fseek(f,4,SEEK_CUR);
							}

							fseek(f,sg,SEEK_CUR);
							n = 0; fread(&n,2,1,f);
							fseek(f,st*n,SEEK_CUR);

							macstart = 0;
						}
					}

					if (macstart)
					{
						sprintf(buf,"Couldn't find machine '%s' (%s) in PARA",macname,dllname);
						MessageBox(NULL,buf,"",MB_OK);
						return false;
					}
				}

				try
				{
					if (m->LoadMachine("cheapo do-nothing.dll",this))
					{
						m->fX = x;
						m->fY = y;
						m->InitMachineNoFileData();
						strcpy(m->name,macname);
						m->nPosInVector = mac;
						m->pHost = this;
						vpMacs.push_back(m);
						
						sprintf(buf,"Machine '%s' (%s) crashed while loading. It has been replaced by a cheapo "
							"do-nothing.",macname,dllname);
					}
					else
					{
						sprintf(buf,"Machine '%s' (%s) crashed while loading. Tried to replace it with a cheapo "
							"do-nothing, but even that failed. This machine has been skipped totally. "
							"You will get some 'bad connection' errors, ignore them.",macname,dllname);
					}
				}
				catch (...)
				{
					sprintf(buf,"Machine '%s' (%s) crashed while loading. Tried to replace it with a cheapo "
						"do-nothing, but even that crashed. This machine has been skipped totally. "
						"You will get some 'bad connection' errors, ignore them.",macname,dllname);
				}

				MessageBox(NULL,buf,"",MB_OK);
			}
		}
	}

	if (!FindSection(f,"CONN")) {MessageBox(NULL,"Couldn't find CONN","",MB_OK); return false;}

	int numconns = 0; fread(&numconns,2,1,f);

	for (i=0; i<numconns; i++)
	{
		int m1=0, m2=0, amp=0, pan=0;
		fread(&m1,2,1,f); fread(&m2,2,1,f); fread(&amp,2,1,f); fread(&pan,2,1,f);

		CConnection c;
		c.fAmp = (float)amp / 16384.0f;
		c.fPan = (float)pan / 16384.0f;
		c.pMac1 = c.pMac2 = NULL;

		for (CMachine **pp = vpMacs.begin(); pp<vpMacs.end() && !(c.pMac1 && c.pMac2); pp++)
		{
			CMachine *p = *pp;
//			char txt[256];
//			sprintf(txt,"machine %i",p->nPosInVector);
//			MessageBox(NULL,txt,"",MB_OK);
			if (p->nPosInVector == m1) c.pMac1 = p;
			if (p->nPosInVector == m2) c.pMac2 = p;
		}

		if (c.pMac1 && c.pMac2) vConns.push_back(c);
		else
		{
			char txt[256];
			sprintf(txt,"m1=%i m2=%i amp=%i pan=%i",m1,m2,amp,pan);
			MessageBox(NULL,txt,"Bad connection",MB_OK);
//			return false;
		}
	}

	return true;
}

