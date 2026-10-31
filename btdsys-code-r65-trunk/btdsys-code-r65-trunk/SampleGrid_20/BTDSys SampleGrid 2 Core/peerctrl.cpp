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


#include "windows.h"
#include "../../mdk/mdk.h"
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include "peerctrl.h"

#include "hack.h"

////////////////////////////////////////////////////////////////////////

class CMyDataOut : public CMachineDataOutput
//A data out class for receiving machine names
{
public:
	CMyDataOut();
	~CMyDataOut();
	virtual void Write(void *pbuf, int const numbytes);

	HWND DlgItem;
	int CtrlID;
	int CtrlAddMessage;

	char ExclName [128];
	int Count;
};

CMyDataOut::CMyDataOut()
	: Count(0)
{
}

CMyDataOut::~CMyDataOut()
{
}

void CMyDataOut::Write(void *pbuf, const int numbytes)
{
	if (strcmp((char*)pbuf, ExclName) != 0)
	{
		SendMessage(DlgItem, 
			CtrlAddMessage, 0, (LPARAM)(LPCTSTR)(pbuf));
		Count++;
	}
}

////////////////////////////////////////////////////////////////////////

CPeerCtrl::CPeerCtrl(CMachine *pmac, CMachineInterface *pmi)
{
	pCB = pmi->pCB;
	ParentMac = pmac;
	FindMIOffset(pmac, pmi);
	HavePar = false;
	JustLoaded = false;
	bRecord = true;
}

////////////////////////////////////////////////////////////////////////

CPeerCtrl::~CPeerCtrl()
{
}

////////////////////////////////////////////////////////////////////////

inline void ReadString (char *temp, CMachineDataInput * const pi)
//Read a null-terminated string from a CMachineDataInput
{
	byte inch;
	int i=0;

	do
	{
		pi->Read(inch);
		*temp = char(inch);
		temp++;
		i++;
	}
	while (inch != 0);
}

////////////////////////////////////////////////////////////////////////

inline void WriteString (const char *temp, CMachineDataOutput * const po)
//Write a null-terminated string to a CMachineDataOutput
{
	byte outch;
	int i=0;
	do
	{
		outch = *temp;
		temp++;
		po->Write(outch);
		i++;
	}
	while (outch != 0);
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::ReadFileData(CMachineDataInput * const pi)
{
	if (pi != NULL) //Read data from file
	{
		byte Version;
		pi->Read(Version);
		if (Version == 2)
		{
			byte HasPar;
			pi->Read(HasPar);
			if (HasPar != 0)
			{
				ReadString(JustLoadedMacName, pi);
				pi->Read(BusParNo);
				pi->Read(BusGroup);
				pi->Read(min);
				pi->Read(max);
				HavePar = true;
				JustLoaded = true;
			}
			else
				HavePar = false;
		}
		else
			MessageBox (NULL, "Incompatible data", "Error",
						MB_OK|MB_SYSTEMMODAL);
	}
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::WriteFileData(CMachineDataOutput * const po)
{
	po->Write(byte(2));	//version
	if (HavePar)
	{
		po->Write(byte(0xFF));
		WriteString (pCB->GetMachineName (BusMachine), po);
		po->Write(BusParNo);
		po->Write(BusGroup);
		po->Write(min);
		po->Write(max);
	}
	else
		po->Write(byte(0));
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::GetMachineNamesToCombo(HWND hWnd, int ComboID, const char *ExcludeName)
{
	CMyDataOut dout;

	dout.DlgItem = GetDlgItem(hWnd, ComboID);
	dout.CtrlAddMessage = CB_ADDSTRING;
	strcpy (dout.ExclName, ExcludeName);
	
	SendMessage(dout.DlgItem, CB_RESETCONTENT, 0, 0);
	pCB->GetMachineNames(&dout);
	
	return dout.Count;
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::GetMachineNamesToList(HWND hWnd, int ListID, const char *ExcludeName)
{
	CMyDataOut dout;

	dout.DlgItem = GetDlgItem(hWnd, ListID);
	dout.CtrlAddMessage = LB_ADDSTRING;
	strcpy (dout.ExclName, ExcludeName);
	
	SendMessage(dout.DlgItem, LB_RESETCONTENT, 0, 0);
	pCB->GetMachineNames(&dout);
	
	return dout.Count;
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::GetParamNames(CMachine *pmac, HWND DlgItem, int AddMessage, int TypesAllowed)
{
	const CMachineInfo *minfo = pCB->GetMachineInfo(pmac);
	const CMachineParameter *par;
	char txt[256];
	int count = 0;

	if (TypesAllowed & ALLOW_GLOBAL)
	{
		for (int p=0; p < minfo->numGlobalParameters; p++)
		{
			par = minfo->Parameters[p];
			if (	((par->Type == pt_switch)	&& (TypesAllowed & ALLOW_SWITCH))
				||	((par->Type == pt_byte)		&& (TypesAllowed & ALLOW_BYTE))
				||	((par->Type == pt_note)		&& (TypesAllowed & ALLOW_NOTE))
				||	((par->Type == pt_word)		&& (TypesAllowed & ALLOW_WORD)))
			{
				sprintf(txt, "[G] %i: %s", p, par->Name);
				SendMessage(DlgItem, 
					AddMessage, 0, (LPARAM)(LPCTSTR)(txt));
				count++;
			}
		}
	}

	if (TypesAllowed & ALLOW_TRACK)
	{
		for (int p=0; p < minfo->numTrackParameters; p++)
		{
			par = minfo->Parameters[p + minfo->numGlobalParameters];
			if (	((par->Type == pt_switch)	&& (TypesAllowed & ALLOW_SWITCH))
				||	((par->Type == pt_byte)		&& (TypesAllowed & ALLOW_BYTE))
				||	((par->Type == pt_note)		&& (TypesAllowed & ALLOW_NOTE))
				||	((par->Type == pt_word)		&& (TypesAllowed & ALLOW_WORD)))
			{
				sprintf(txt, "[T] %i: %s", p, par->Name);
				SendMessage(DlgItem, 
					AddMessage, 0, (LPARAM)(LPCTSTR)(txt));
				count++;
			}
		}
	}

	if (TypesAllowed & ALLOW_ATTRIBUTES)
	{
		for (int a=0; a < minfo->numAttributes; a++)
		{
			sprintf(txt, "[A] %i: %s", a, minfo->Attributes[a]->Name);
			SendMessage(DlgItem,
				AddMessage, 0, (LPARAM)(LPCTSTR)(txt));
			count++;
		}
	}

	return count;
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::GetParamNamesToCombo(CMachine *pmac, HWND hWnd, int ComboID, int TypesAllowed)
{
	SendMessage(GetDlgItem(hWnd, ComboID), CB_RESETCONTENT, 0, 0);
	return GetParamNames(pmac, GetDlgItem(hWnd, ComboID), CB_ADDSTRING, TypesAllowed);
}

int CPeerCtrl::GetParamNamesToList(CMachine *pmac, HWND hWnd, int ListID, int TypesAllowed)
{
	SendMessage(GetDlgItem(hWnd, ListID), LB_RESETCONTENT, 0, 0);
	return GetParamNames(pmac, GetDlgItem(hWnd, ListID), LB_ADDSTRING, TypesAllowed);
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::FindMachine(HWND DlgItem, int SelectStringMessage)
{
	return SendMessage(DlgItem, 
		SelectStringMessage, 
		0, (LPARAM)(GetMachineName()));
}

bool CPeerCtrl::FindParam(HWND DlgItem, int SelectStringMessage)
{
	return (SendMessage(DlgItem, 
		SelectStringMessage, 
		0, (LPARAM)(GetParamString())) != CB_ERR);
}

////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::SelectMachineInCombo(HWND hWnd, int ComboID)
{
	int t = FindMachine(GetDlgItem(hWnd, ComboID), CB_FINDSTRINGEXACT);
	if (t != CB_ERR)
	{
		SendMessage(GetDlgItem(hWnd, ComboID),
			CB_SETCURSEL,
			t, 0);
		return true;
	}
	else
		return false;
}

bool CPeerCtrl::SelectMachineInList(HWND hWnd, int ListID)
{
	int t = FindMachine(GetDlgItem(hWnd, ListID), LB_FINDSTRINGEXACT);
	if (t != LB_ERR)
	{
		SendMessage(GetDlgItem(hWnd, ListID),
			LB_SETCURSEL,
			t, 0);
		return true;
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::SelectParameterInCombo(HWND hWnd, int ComboID)
{
	return FindParam(GetDlgItem(hWnd, ComboID), CB_SELECTSTRING);
}

bool CPeerCtrl::SelectParameterInList(HWND hWnd, int ListID)
{
	return FindParam(GetDlgItem(hWnd, ListID), LB_SELECTSTRING);
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::AssignParameter(CMachine *pMachine, int Group, int Param)
{
	BusMachine = pMachine;

	if (BusMachine != NULL)
	{
		HavePar = true;
		BusGroup = Group;
		BusParNo = Param;
		const CMachineInfo *minfo = pCB->GetMachineInfo(BusMachine);
		if (BusGroup == 1)
			BusPar = minfo->Parameters[BusParNo];
		else if (BusGroup == 'A')
		{
			dummypar.Type			= pt_word;
			dummypar.Name			= minfo->Attributes[Param]->Name;
			dummypar.Description	= minfo->Attributes[Param]->Name;
			dummypar.MinValue		= minfo->Attributes[Param]->MinValue;
			dummypar.MaxValue		= minfo->Attributes[Param]->MaxValue;
			dummypar.NoValue		= 0;
			dummypar.Flags			= 0;
			dummypar.DefValue		= minfo->Attributes[Param]->DefValue;

			BusPar = &dummypar;
		}
		else
			BusPar = minfo->Parameters[BusParNo + minfo->numGlobalParameters];
	}
}

void CPeerCtrl::AssignParameter(char *MacName, char *ParaName)
// [G] 1: Cutoff
// 0123456
{
	BusMachine = pCB->GetMachine(MacName);

	if (BusMachine != NULL)
	{
		HavePar = true;
		if (ParaName[1] == 'A')
			BusGroup = 'A';
		else
			BusGroup = (ParaName[1] == 'G') ? 1 : 2;

		BusParNo = atoi(ParaName+4);
		AssignParameter(BusMachine, BusGroup, BusParNo);
/*		const CMachineInfo *minfo = pCB->GetMachineInfo(BusMachine);
		if (BusGroup == 1)
			BusPar = minfo->Parameters[BusParNo];
		else
			BusPar = minfo->Parameters[BusParNo + minfo->numGlobalParameters];
*/	}
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::UnassignParameter()
{
	HavePar = false;
}

////////////////////////////////////////////////////////////////////////

CMachineParameter const *CPeerCtrl::GetParamInfo()
{
	if (GotParam())
		return BusPar;
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////

char const *CPeerCtrl::GetParamString()
{
	static char txt[256];
	sprintf (txt, "[%s] %i: %s",
		(BusGroup == 1) ? "G" : ((BusGroup == 'A') ? "A" : "T"),
		BusParNo,
		BusPar->Name);
	OutputDebugString(txt);
	return txt;
}

////////////////////////////////////////////////////////////////////////

CMachine *CPeerCtrl::GetMachine()
{
	if (GotParam())
		return BusMachine;
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////

char const *CPeerCtrl::GetMachineName()
{
	if (GotParam())
		return pCB->GetMachineName(BusMachine);
	else
	{
		if (JustLoaded)
			return JustLoadedMacName;
		else
			return NULL;
	}
}

////////////////////////////////////////////////////////////////////////

CMachineInfo const *CPeerCtrl::GetMInfo()
{
	if (GotParam())
		return pCB->GetMachineInfo(BusMachine);
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////

CMachineInterface *CPeerCtrl::GetMInterface()
{
	if (GotParam())
		return GetMI(BusMachine);
	else
		return NULL;
}

////////////////////////////////////////////////////////////////////////

char const *CPeerCtrl::GetAssignmentString()
{
	static char txt[256];
	if (GotParam())
		sprintf(txt,"%s->%s",pCB->GetMachineName(BusMachine),BusPar->Name);
	else
		sprintf(txt,"No assign");
	return txt;
}

////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::CheckMachine()
{
	if (HavePar)
	{
		if (JustLoaded) //The machine was just loaded in a song file
		{
			BusMachine = pCB->GetMachine (JustLoadedMacName);

			if (BusMachine == NULL)
			{
				char txt[1024];
				sprintf(txt,"Machine named '%s' could not be found.\n"
					"Hence machine %s needs reassigning.\n"
					"Reasons:\n"
					"* the file could be corrupted\n"
					"* it is a bug - it happens sometimes :( If you get "
					"this message frequently, please let me know",
					JustLoadedMacName,
					pCB->GetMachineName(ParentMac));
				MessageBox(NULL,txt,"Error",MB_OK);

				HavePar = false;
				JustLoaded = false;
				return false;
			}
			else
			{
				const CMachineInfo *minfo = pCB->GetMachineInfo(BusMachine);

				if (BusGroup == 1) //Global
					BusPar = minfo->Parameters[BusParNo];
				else if (BusGroup == 'A')
				{
					dummypar.Type			= pt_word;
					dummypar.Name			= minfo->Attributes[BusParNo]->Name;
					dummypar.Description	= minfo->Attributes[BusParNo]->Name;
					dummypar.MinValue		= minfo->Attributes[BusParNo]->MinValue;
					dummypar.MaxValue		= minfo->Attributes[BusParNo]->MaxValue;
					dummypar.NoValue		= 0;
					dummypar.Flags			= 0;
					dummypar.DefValue		= minfo->Attributes[BusParNo]->DefValue;

					BusPar = &dummypar;
				}
				else
					BusPar = minfo->Parameters[BusParNo + minfo->numGlobalParameters];

				JustLoaded = false;
				HavePar = CheckMachine();

				return HavePar;
			}
		}
		else // if (!JustLoaded)
		{
			CMachine * NewMac = pCB->GetMachine (pCB->GetMachineName (BusMachine));
			
			
			if (NewMac != BusMachine) //Something is amiss! (Should be the same)
			{
				HavePar = false;
				return false;
			}
			else
				return true;
		}
	}
	else
		return false;
}

////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::GotParam()
{
	return (HavePar && !JustLoaded);
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::ControlChange_Immediate(int track, int value)
{
	if (GotParam() && (track < GetMInfo()->maxTracks || BusGroup != 2))
	{
		if (BusGroup == 'A')
		{
			GetMI(BusMachine)->AttrVals[BusParNo] = value;
			GetMI(BusMachine)->AttributesChanged();
		}
		else
		{
			Hack_ControlsClear(pCB->GetMachineInfo(BusMachine), BusMachine);
			Hack_ControlChange(pCB->GetMachineInfo(BusMachine),
				BusMachine,
				BusGroup,
				track,
				BusParNo,
				value);
			GetMI(BusMachine)->Tick();
		}
	}
}

////////////////////////////////////////////////////////////////////////

void CPeerCtrl::ControlChange_NextTick(int track, int value)
{
	if (GotParam())
	{
		if (BusGroup == 'A')
		{
			GetMI(BusMachine)->AttrVals[BusParNo] = value;
			GetMI(BusMachine)->AttributesChanged();
		}
		else
		{
			pCB->ControlChange(BusMachine,
				BusGroup + (bRecord ? 0 : 16),
				track,
				BusParNo,
				value);
		}
	}
}

//////////////////////////////////////////////////////////////////////

const char *CPeerCtrl::DescribeValue(int value)
{
	if (GotParam())
		return GetMI(BusMachine)->DescribeValue(
			BusParNo + ((BusGroup == GROUP_TRACK) ? pCB->GetMachineInfo(BusMachine)->numGlobalParameters : 0),
			value);
	else
		return NULL;
}
