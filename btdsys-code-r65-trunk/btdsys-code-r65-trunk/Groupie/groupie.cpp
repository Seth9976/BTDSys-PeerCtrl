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
#include "../mdk/mdk.h"
#include "groupie.h"
#include "BuzzMI.h"
#include "DSPStuff.h"

#include "resource.h"

#include "MacViewFrame.h"

////////////////////////////////////////////////////////////////////////
// Machine info

CMachineParameter const *pParameters[] = {
	&paraNote,
	&paraSwitch,
	&paraByte,
	&paraWord,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

CMachineInfo const MacInfo =
{
#ifdef _GROUPIE_GEN
	MT_GENERATOR,
#else
	MT_EFFECT,						//Machine type
#endif
	MI_VERSION,						//Interface version
	MIF_MONO_TO_STEREO | MIF_DOES_INPUT_MIXING,			//Flags
	1,								//Min tracks
	MAX_TRACKS,						//Max tracks
	NUMG,							//Number of global parameters
	NUMT,							//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
#ifdef _GROUPIE_GEN
	"BTDSys Groupie (Gen)",			//Full name
#else
	"BTDSys Groupie (Eff)",
#endif
	"Group",						//Short name
	"Ed Powley (BTDSys)",			//Author
	"About...\nEdit..."				//Commands
};

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	TrackVals = &tval;

	pMacView = NULL;

	for (int i=0; i<4; i++)
	{
		for (int j=0; j<MAX_TRACKS; j++)
		{
			assigns[i][j].m_nType  = i;
			assigns[i][j].m_nTrack = j;
			assigns[i][j].pmi = this;
		}
	}

    MacViewPlacement.length = 0;
}

mi::~mi()
{
	for (vector<CMachine*>::iterator pp=vpMacs.begin(); pp<vpMacs.end(); pp++)
	{
		if (!(*pp)->bSpecial)
			delete (*pp);
	}
}

////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
	Command(1); return true;
//	return false;
}

void mi::SetupInOutMachines()
{
#ifndef _GROUPIE_GEN
	static CMachineInfo const macInInfo =
	{
		MT_MASTER,						//Machine type
		-1,								//Interface version
		0,								//Flags
		0,								//Min tracks
		0,								//Max tracks
		0,								//Number of global parameters
		0,								//Number of track parameters
		{NULL},							//Pointer to parameters
		0,								//Number of attributes
		{NULL},							//Pointer to attributes
		"BTDSys Groupie - Input Dummy",	//Full name
		"Input",						//Short name
		"Ed Powley (BTDSys)",			//Author
		"About..."						//Commands
	};

	macIn.pHost = this;
	macIn.bSpecial = true;
	macIn.bIsEffect = false;
	macIn.bStereoOut = true;
	macIn.SetMI(INPUT_MACHINE);
	macIn.fX = -0.8f;
	macIn.fY = 0.0f;
	macIn.pInfo = &macInInfo;
	strcpy(macIn.name, "Input");
	strcpy(macIn.dllname, "(internal)");
	vpMacs.push_back(&macIn);
#endif

	static CMachineInfo const macOutInfo =
	{
		MT_MASTER,						//Machine type
		-1,								//Interface version
		0,								//Flags
		0,								//Min tracks
		0,								//Max tracks
		0,								//Number of global parameters
		0,								//Number of track parameters
		{NULL},							//Pointer to parameters
		0,								//Number of attributes
		{NULL},							//Pointer to attributes
		"BTDSys Groupie - Output Dummy",//Full name
		"Output",						//Short name
		"Ed Powley (BTDSys)",			//Author
		"About..."						//Commands
	};

	macOut.pHost = this;
	macOut.bSpecial = true;
	macOut.bIsEffect = true;
	macOut.bNoOut = true;
	macOut.SetMI(OUTPUT_MACHINE);
	macOut.fX = 0.8f;
	macOut.fY = 0.0f;
	macOut.pInfo = &macOutInfo;
	strcpy(macOut.name, "Output");
	strcpy(macOut.dllname, "(internal)");
	vpMacs.push_back(&macOut);
}

void GetRegVal(HKEY hk, char *name, unsigned long *pi)
{
	unsigned long i = 4;
	unsigned long type = REG_DWORD;

	if (RegQueryValueEx(hk,name,0,&type,(unsigned char *)pi,&i) != ERROR_SUCCESS)
		RegSetValueEx(hk,name,0,REG_DWORD,(unsigned char *)pi,4);
}


void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	thismac = pCB->GetThisMachine();
	pCB->SetnumOutputChannels(thismac,2);

	pCB->SetEventHandler(thismac, 
						DoubleClickMachine, 
						(EVENT_HANDLER_PTR)DblClickEvent, NULL);

	SetupInOutMachines();

	if (pi)
	{
		byte version;
		pi->Read(version);
		if (version == 1)
		{
			//machines
#ifndef _GROUPIE_GEN
			macIn.InitMachineFromFileData(pi);
#endif
			macOut.InitMachineFromFileData(pi);

			int numMacs;
			pi->Read(numMacs);

			for (int m=0; m<numMacs; m++)
			{
				char dllname[256];
				pi->Read(dllname,256);
				CMachine *mac = new CMachine;
				if (mac->LoadMachine(dllname, this))
				{
					mac->pHost = this;
					vpMacs.push_back(mac);
					mac->InitMachineFromFileData(pi);
				}
			}

			//connections
			int numConns;
			pi->Read(numConns);

			for (int c=0; c<numConns; c++)
			{
				int t,u;
				pi->Read(t);
				pi->Read(u);

				char txt[256];
				sprintf(txt,"Conn %i : %s ->- %s",c,vpMacs[t]->name,vpMacs[u]->name);
				OutputDebugString(txt);

				CConnection *pC = TryConnectMachines(vpMacs[t], vpMacs[u]);
				if (pC)
				{
					pi->Read(pC->fAmp);
					pi->Read(pC->fPan);
				}
				else
				{
					float f;
					pi->Read(f);
					pi->Read(f);
				}
			}

			//assigns
			int numAssigns;
			pi->Read(numAssigns);

			for (int t=0; t<numAssigns; t++)
			{
				for (int u=0; u<4; u++)
				{
					CParamAssignInfo *a = &assigns[u][t];
					int m;
					pi->Read(m);
					if (m == -1)
						a->pTargetMac = NULL;
					else
					{
						a->pTargetMac = vpMacs[m];
						pi->Read(a->nTrack);
						pi->Read(a->nParam);
					}
				}
			}

            pi->Read(&MacViewPlacement, sizeof(WINDOWPLACEMENT));
		}
		else
		{
			char txt[256];
			sprintf(txt,"Incompatible data: version %i",version);
			MessageBox(NULL,txt,"",MB_OK);
		}
	}

	HKEY hk;

	colBack		= RGB(201,214,218);
	colGen		= RGB(169,174,199);
	colEff		= RGB(199,173,169);
	colMaster	= RGB(198,190,170);
	nMacWidth	= 100;
	nMacHeight	= 50;

	if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\Groupie",&hk) == ERROR_SUCCESS)
	{
		GetRegVal(hk,"colBack"		,&colBack	);
		GetRegVal(hk,"colGen"		,&colGen	);
		GetRegVal(hk,"colEff"		,&colEff	);
		GetRegVal(hk,"colMaster"	,&colMaster	);
		GetRegVal(hk,"nMacWidth"	,&nMacWidth	);
		GetRegVal(hk,"nMacHeight"	,&nMacHeight);

		RegCloseKey(hk);
	}
	else
		MessageBox(NULL,"Warning: couldn't open reg key HKEY_CURRENT_USER\\Software\\BTDSys\\Groupie","Groupie",MB_OK);

	bFirstTick = true;
}

void mi::MDKSave(CMachineDataOutput * const po)
{
	po->Write((byte)1); //version

	//machines
#ifndef _GROUPIE_GEN
	macIn.SaveMachineFileData(po);
	macIn.nPosInVector = 0;
	macOut.nPosInVector = 1;
#else
	macOut.nPosInVector = 0;
#endif
	macOut.SaveMachineFileData(po);

#ifndef _GROUPIE_GEN
	const int nFirstNonSpecialMac = 2;
#else
	const int nFirstNonSpecialMac = 1;
#endif

	po->Write((int)(vpMacs.size() - nFirstNonSpecialMac));

	for (int m=nFirstNonSpecialMac; m<vpMacs.size(); m++)
	{
		po->Write(vpMacs[m]->dllname,256);
		vpMacs[m]->SaveMachineFileData(po);
		vpMacs[m]->nPosInVector = m;
	}

	//connections
	po->Write((int)vConns.size());
	for (vector<CConnection>::iterator p = vConns.begin(); p < vConns.end(); p++)
	{
		po->Write(p->pMac1->nPosInVector);
		po->Write(p->pMac2->nPosInVector);
		po->Write(p->fAmp);
		po->Write(p->fPan);
	}

	//assigns
	po->Write(numTracks);
	for (int t=0; t<numTracks; t++)
	{
		for (int u=0; u<4; u++)
		{
			CParamAssignInfo *a = &assigns[u][t];
			if (a->pTargetMac)
			{
				po->Write(a->pTargetMac->nPosInVector);
				po->Write(a->nTrack);
				po->Write(a->nParam);
			}
			else
				po->Write((int)-1);
		}
	}

    if (pMacView)
    {
        MacViewPlacement.length = sizeof(WINDOWPLACEMENT);
        pMacView->GetWindowPlacement(&MacViewPlacement);
    }

    po->Write(&MacViewPlacement, sizeof(WINDOWPLACEMENT));

}

void mi::Command(const int i)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case 0:
		MessageBox(NULL,
			"BTDSys Groupie v?.?\n"
			"©2003-2005 Ed Powley",
			"About",MB_OK);
		break;

	case 1:
		if (pMacView)
			pMacView->SetActiveWindow();
		else
		{
			pMacView = new CMacViewFrame;
			pMacView->pmi = this;
			char txt[256];
			sprintf(txt,"BTDSys Groupie [%s] - Machine View",pCB->GetMachineName(thismac));
			pMacView->Create("",txt);
			pMacView->ShowWindow(SW_SHOW);
            if (MacViewPlacement.length)
                pMacView->SetWindowPlacement(&MacViewPlacement);
			pMacView->UpdateWindow();
		}

		break;
	}
}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
	for (vector<CMachine *>::iterator pp=vpMacs.begin(); pp<vpMacs.end(); pp++)
	{
		CMachine *p = *pp;
		if (p->bNoOut && !p->bSpecial)
		{
			WorkThisMachine(p,psamples,numsamples,mode);
			p->bWorked = true;
		}
		else
			p->bWorked = false;
	}

	if (WorkThisMachine(&macOut,psamples,numsamples,mode))
	{
		DSP_Copy(psamples,macOut.buf,numsamples*2);
		return true;
	}
	else
		return false;
}

void CParamAssignInfo::ControlChange(int value)
{
	if (pTargetMac)
		pTargetMac->ChangeParamFromHost(nParam, nTrack, value);
}

void mi::Tick()
{
	if (!bFirstTick)
	{
		for (int t=0; t<numTracks; t++)
		{
			if (tval[t].Note != paraNote.NoValue)
				assigns[pt_note][t].ControlChange(tval[t].Note);

			if (tval[t].Switch != paraSwitch.NoValue)
				assigns[pt_switch][t].ControlChange(tval[t].Switch);

			if (tval[t].Byte != paraByte.NoValue)
				assigns[pt_byte][t].ControlChange(tval[t].Byte);

			if (tval[t].Word != paraWord.NoValue)
				assigns[pt_word][t].ControlChange(tval[t].Word);
		}
	}

	for (vector<CMachine *>::iterator pp=vpMacs.begin(); pp<vpMacs.end(); pp++)
	{
		if (!(*pp)->bSpecial)
		{
			(*pp)->Tick(__FILE__,__LINE__);
			(*pp)->ClearParams();
		}
	}

	bFirstTick = false;
}

void mi::Stop()
{
	for (vector<CMachine *>::iterator pp=vpMacs.begin(); pp<vpMacs.end(); pp++)
	{
		if (!(*pp)->bSpecial)
		{
			(*pp)->Stop(__FILE__,__LINE__);
		}
	}
}

char const *mi::DescribeValue(int const param, int const value)
{
	return NULL;
}

char const *CParamAssignInfo::GetDesc()
{
	static char txt[256];
	switch (m_nType)
	{
	case pt_note:	strcpy(txt,"Note");		break;
	case pt_switch:	strcpy(txt,"Switch");	break;
	case pt_byte:	strcpy(txt,"Byte");		break;
	case pt_word:	strcpy(txt,"Word");		break;
	default:		strcpy(txt,"???");
	}

	sprintf(txt,"%s %i (Current:", txt, m_nTrack);

	if (pTargetMac)
	{
		if (nTrack == -1)
			sprintf(txt,"%s %s->%s)", txt, pTargetMac->name, 
				pTargetMac->pInfo->Parameters[nParam]->Name);
		else
			sprintf(txt,"%s %s->Track %i->%s)", txt, pTargetMac->name, 
				nTrack,
				pTargetMac->pInfo->Parameters[
					nParam + pTargetMac->pInfo->numGlobalParameters]->Name);
	}
	else
		strcat(txt," No assign)");
	
	return txt;
}
