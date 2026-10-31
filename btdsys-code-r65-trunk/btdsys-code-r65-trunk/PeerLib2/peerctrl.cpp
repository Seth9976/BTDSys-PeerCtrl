/*
Copyright (c) 2003-2008, Edward Powley
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

// For mfc projects, this prevents linker errors lnk1169 and lnk2005
// For non-mfc projects this creates errors so needs to be disabled

#ifdef MACHINE_USING_MFC
#include <afx.h>
#endif

#include "peerctrl.h"

//////////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::s_initialised = false;
int CPeerCtrl::s_hostversion = 0;
std::vector<CPeerCtrl*> CPeerCtrl::s_instances;

bool CPeerCtrl::s_immediatebatchmode = false;
std::set<CMachine*> CPeerCtrl::s_immediatebatch, CPeerCtrl::s_attributebatch;

HMODULE CPeerCtrl::s_palModule;

void CPeerCtrl::InitStatic(CMachine *pmac, CMachineInterface *pmi)
{
	if (s_initialised) return;

	if (pmi->pCB->GetNearestWaveLevel(-2, -2))
		s_hostversion = pmi->pCB->GetHostVersion();
	else
		s_hostversion = 1;

	s_palModule = LoadLibrary("Gear\\BTDSys PAL2.dll");

	s_initialised = true;
}

//////////////////////////////////////////////////////////////////////////////

CPeerCtrl::CPeerCtrl()
{
	m_initialised = false;
	m_loadedname[0] = 0;
	s_instances.push_back(this);
}

CPeerCtrl::CPeerCtrl(CMachine *pmac, CMachineInterface *pmi)
{
	m_initialised = false;
	m_loadedname[0] = 0;
	Init(pmac, pmi);
	s_instances.push_back(this);
}

CPeerCtrl::~CPeerCtrl()
{
	m_targetmac = NULL;
	TellPAL();
	OnChangeAssignment(InstanceDestroyed);

	if (s_palModule)
	{
		pf_addCMICallbacks f = (pf_addCMICallbacks)GetProcAddress(s_palModule, "removeCMICallbacks");
		if (f) f(pCB);
	}

	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if (*i == this)
		{
			s_instances.erase(i);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

void CPeerCtrl::Init(CMachine *pmac, CMachineInterface *pmi)
{
	if (!s_initialised) InitStatic(pmac, pmi);

	pCB = pmi->pCB;
	m_parentmac = pmac;
	m_parentmi = pmi;

	m_targetmac = NULL;

	if (s_palModule)
	{
		pf_addCMICallbacks f = (pf_addCMICallbacks)GetProcAddress(s_palModule, "addCMICallbacks");
		if (f) f(pCB);
	}

	m_initialised = true;
}

void CPeerCtrl::SetLabel(const std::string &label)
{
	m_label = label;
}

void CPeerCtrl::SetLabel(const char* label)
{
	m_label = label;
}

void CPeerCtrl::SetLabelAsTrackNumber(int t)
{
	char txt[128];
	_snprintf(txt, 128, "Track %i", t);
	SetLabel(txt);
}

//////////////////////////////////////////////////////////////////////////////

void CPeerCtrl::SetEventHandlers(CMICallbacks *pCB, int which)
{
	class DummyMI : public CMachineInterface, CMachineInterfaceEx
	{
	public:
		bool OnAddMachine(void* pmac)
		{ return CPeerCtrl::OnAddMachine((CMachine*)pmac); }
		bool OnDeleteMachine(void* pmac)
		{ return CPeerCtrl::OnDeleteMachine((CMachine*)pmac); }
		bool OnRenameMachine(void* pmac)
		{ return CPeerCtrl::OnRenameMachine((CMachine*)pmac); }
		bool OnUndeleteMachine(void* pmac)
		{ return CPeerCtrl::OnUndeleteMachine((CMachine*)pmac); }

	};

	if (which & EVENT_HANDLER_ADD)
	{
		pCB->SetEventHandler(
			pCB->GetThisMachine(), gAddMachine, (EVENT_HANDLER_PTR)&DummyMI::OnAddMachine, NULL);
	}

	if (which & EVENT_HANDLER_DELETE)
	{
		pCB->SetEventHandler(
			pCB->GetThisMachine(), gDeleteMachine, (EVENT_HANDLER_PTR)&DummyMI::OnDeleteMachine, NULL);
	}

	if (which & EVENT_HANDLER_RENAME)
	{
		pCB->SetEventHandler(
			pCB->GetThisMachine(), gRenameMachine, (EVENT_HANDLER_PTR)&DummyMI::OnRenameMachine, NULL);
	}

	if (which & EVENT_HANDLER_UNDELETE)
	{
		pCB->SetEventHandler(
			pCB->GetThisMachine(), gUndeleteMachine, (EVENT_HANDLER_PTR)&DummyMI::OnUndeleteMachine, NULL);
	}
}

bool CPeerCtrl::OnAddMachine(CMachine *pmac)
{
	if (s_instances.size() == 0) return true;

	const char *macname = s_instances[0]->pCB->GetMachineName(pmac);

	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if ((*i)->m_loadedname[0] && strcmp((*i)->m_loadedname, macname) == 0)
		{
			(*i)->m_targetmac = pmac;
			(*i)->m_loadedname[0] = '\0';
			(*i)->TellPAL();
			(*i)->OnChangeAssignment(TargetLoaded);
		}
	}
	return true;
}

bool CPeerCtrl::OnDeleteMachine(CMachine *pmac)
{
	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if ((*i)->GetMachine() == pmac)
		{
			(*i)->m_undoList.push_back(SPeerUndoData(pmac, (*i)->GetGroup(), (*i)->GetParamNumber_Grouped(), (*i)->GetTrack()));
			(*i)->m_targetmac = NULL;
			(*i)->TellPAL();
			(*i)->OnChangeAssignment(TargetRemoved);
		}
	}
	return true;
}

bool CPeerCtrl::OnRenameMachine(CMachine *pmac)
{
	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if ((*i)->GetMachine() == pmac)
		{
			(*i)->TellPAL();
			(*i)->OnChangeAssignment(TargetRenamed);
		}
	}
	return true;
}

bool CPeerCtrl::OnUndeleteMachine(CMachine *pmac)
{
	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if(!(*i)->m_undoList.empty())
		{
			SPeerUndoData info = (*i)->m_undoList.back();
			if(pmac == info.m_targetmac)
			{
				if((*i)->ReassignQuery(info))
				{
					(*i)->m_targetmac = info.m_targetmac;
					(*i)->m_group = info.m_group;
					(*i)->m_paramn = info.m_paramn;
					(*i)->m_track = info.m_track;
					(*i)->TellPAL();
					(*i)->OnChangeAssignment(TargetRestored);
				}
				(*i)->m_undoList.pop_back();
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////

void CPeerCtrl::ReadFileData(CMachineDataInput * const pi)
{
	if (pi)
	{
		byte version;
		pi->Read(version);
		if (version >= 1 && version <= 2)
		{
			byte haspar;
			pi->Read(haspar);
			if (haspar)
			{
				m_targetmac = NULL;
				char *p = m_loadedname;
				do
				{
					pi->Read(p,1);
					p++;
				}
				while (p[-1]);
				pi->Read(m_paramn);
				pi->Read(m_group);
				if (version == 2)
					pi->Read(m_track);

				FindJustLoadedMachine();
			}
			else
			{
				m_targetmac = NULL;
				m_loadedname[0] = '\0';
			}
		}
		else
            throw PeerCtrlException("Incompatible data");
	}
}

void CPeerCtrl::PushFileData(const char* macname, int paramnum, int group)
{
	if (macname)
	{
		m_targetmac = NULL;
		if (macname != m_loadedname) strcpy(m_loadedname, macname);
		m_paramn = paramnum;
		m_group = group;

		FindJustLoadedMachine();
	}
	else
	{
		m_targetmac = NULL;
		m_loadedname[0] = '\0';
	}
}

bool CPeerCtrl::FindJustLoadedMachine()
{
	if (m_loadedname[0] == '\0') return false;

	if(pCB->GetBuildNumber() > 1292)
		pCB->RemapLoadedMachineName(m_loadedname, sizeof(m_loadedname));

	CMachine *m = pCB->GetMachine(m_loadedname);
	if (m)
	{
		m_targetmac = m;
		m_loadedname[0] = '\0';
		TellPAL();
		OnChangeAssignment(TargetLoaded);
		return true;
	}
	else
		return false;
}

void CPeerCtrl::WriteFileData(CMachineDataOutput * const po, bool backwardsCompatible)
{
	// version
	if (backwardsCompatible)
		po->Write((byte)1);
	else
		po->Write((byte)2);

	if (GotParam())
	{
		po->Write((byte)1);
		const char *p = GetMachineName();
		for (; *p; p++)
		{
			po->Write((byte)*p);
		}
		po->Write((byte)0);
		po->Write(m_paramn);
		po->Write(m_group);

		if (!backwardsCompatible)
			po->Write(m_track);
	}
	else
	{
		po->Write((byte)0);
	}
}

void CPeerCtrl::OnImportFinished()
{
	for (std::vector<CPeerCtrl*>::iterator i = s_instances.begin(); i != s_instances.end(); ++i)
	{
		if(*(*i)->m_loadedname && !(*i)->FindJustLoadedMachine())
		{
			char str[512];
			_snprintf(str, 512, "Could not find machine \"%s\"!", (*i)->m_loadedname);
			MessageBox(GetForegroundWindow(), str, "Missing Machine", MB_ICONERROR | MB_OK);
			(*i)->UnassignParameter();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

void CPeerCtrl::AssignParameter(CMachine *pmac, int group, int param, int track)
{
	m_targetmac = pmac;
	m_group = group;
	m_paramn = param;
	m_track = track;
	TellPAL();
	OnChangeAssignment(TargetAssigned);
}

void CPeerCtrl::UnassignParameter()
{
	m_targetmac = NULL;
	TellPAL();
	OnChangeAssignment(TargetUnassigned);
}

void CPeerCtrl::TellPAL()
{
	if (s_palModule)
	{
		pf_setPeerAssignment f 
			= (pf_setPeerAssignment)GetProcAddress(s_palModule, "setPeerAssignment");
		if (f)
		{
			f((void*)this,
				m_parentmac,
				m_label.c_str(),
				m_targetmac,
				m_group, GetParamNumber_Ungrouped(), m_track
			);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

bool CPeerCtrl::GotParam()
{
	return m_targetmac != NULL;
}

//////////////////////////////////////////////////////////////////////////////

char const *CPeerCtrl::GetLabel()
{
	return m_label.c_str();
}

CMachine *CPeerCtrl::GetMachine()
{
	if (GotParam())
		return m_targetmac;
	else
		return NULL;
}

char const *CPeerCtrl::GetMachineName()
{
	if (GotParam())
		return pCB->GetMachineName(m_targetmac);
	else
		return NULL;
}

CMachineInfo const *CPeerCtrl::GetMInfo()
{
	if (GotParam())
		return pCB->GetMachineInfo(m_targetmac);
	else
		return NULL;
}

CMachineParameter const *CPeerCtrl::GetParamInfo()
{
	if (GotParam())
	{
		if (m_group == GROUP_ATTRIBUTE)
		{
			memset(&m_attrAsParam, 0, sizeof(CMachineParameter));

			const CMachineAttribute* attrinfo = GetMInfo()->Attributes[m_paramn];
			m_attrAsParam.Type = pt_word;
			m_attrAsParam.Name = attrinfo->Name;
			m_attrAsParam.MinValue = attrinfo->MinValue;
			m_attrAsParam.MaxValue = attrinfo->MaxValue;
			m_attrAsParam.DefValue = attrinfo->DefValue;

			return &m_attrAsParam;
		}
		else
			return GetMInfo()->Parameters[GetParamNumber_Ungrouped()];
	}
	else
		return NULL;
}

int CPeerCtrl::GetParamNumber_Grouped()
{
	if (GotParam())
		return m_paramn;
	else
		return -1;
}

int CPeerCtrl::GetParamNumber_Ungrouped()
{
	if (GotParam())
	{
		if (m_group == GROUP_TRACK)
			return m_paramn + GetMInfo()->numGlobalParameters;
		else
			return m_paramn;
	}
	else
		return -1;
}

int CPeerCtrl::GetGroup()
{
	return m_group;
}

char const *CPeerCtrl::GetParamString()
{
	static char txt[256];
	if (GotParam())
	{
		_snprintf(txt, 256, "[%s] %i: %s",
			(m_group == 1) ? "G" : ((m_group == 'A') ? "A" : "T"),
			m_paramn,
			GetParamInfo()->Name);
		return txt;
	}
	else
		return NULL;
}

char const *CPeerCtrl::GetAssignmentString()
{
	static char txt[256];
	if (GotParam())
		_snprintf(txt, 256, "%s->%s", GetMachineName(),GetParamInfo()->Name);
	else
		_snprintf(txt, 256, "No assign");
	return txt;
}

int CPeerCtrl::GetTrack()
{
	return m_track;
}

//////////////////////////////////////////////////////////////////////////////

#ifdef PEERLIB_NEXTTICK
	void CPeerCtrl::ControlChange_NextTick(int track, int value)
	{
		if (track == TRACK_DEFAULT) 
			track = m_track;
		if (track == TRACK_DEFAULT && m_track == TRACK_DEFAULT) 
			track = 0;

		if (GotParam())
		{
			pCB->ControlChange(m_targetmac,
				m_group,
				track,
				GetParamNumber_Grouped(),
				value);
		}
	}
#endif

//////////////////////////////////////////////////////////////////////////////

void CPeerCtrl::ControlChange_Immediate(int track, int value)
{
	if (track == TRACK_DEFAULT) 
		track = m_track;
	if (track == TRACK_DEFAULT && m_track == TRACK_DEFAULT) 
		track = 0;

	if (GotParam())
	{
		if (m_group == GROUP_ATTRIBUTE)
		{
			char txt[256];
			_snprintf(txt, 256, "pCB->SetAttribute(%p,%i,%i)", m_targetmac, m_paramn, value);
			OutputDebugString(txt);

			pCB->SetAttribute(m_targetmac, m_paramn, value);
			if (s_immediatebatchmode)
				s_attributebatch.insert(m_targetmac);
			else
				pCB->AttributesChanged(m_targetmac);
		}
		else
		{
			pCB->ControlChangeImmediate(m_targetmac,
				m_group,
				track,
				GetParamNumber_Grouped(),
				value);

			if (s_immediatebatchmode)
				s_immediatebatch.insert(m_targetmac);
			else
				pCB->SendControlChanges(m_targetmac);
		}
	}
}

void CPeerCtrl::BeginImmediateBatch()
{
	s_immediatebatch.clear();
	s_attributebatch.clear();
	s_immediatebatchmode = true;
}

void CPeerCtrl::EndImmediateBatch()
{
	CMICallbacks* pCB = s_instances[0]->pCB;

	for (std::set<CMachine*>::iterator i = s_immediatebatch.begin();
		i != s_immediatebatch.end(); ++i)
	{
		pCB->SendControlChanges(*i);
	}

	for (std::set<CMachine*>::iterator i = s_attributebatch.begin();
		i != s_attributebatch.end(); ++i)
	{
		pCB->AttributesChanged(*i);
	}

	s_immediatebatch.clear();
	s_attributebatch.clear();
	s_immediatebatchmode = false;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

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

void CMyDataOut::Write(void *pbuf, int const numbytes)
{
	if (strcmp((char*)pbuf, ExclName) != 0)
	{
		SendMessage(DlgItem, 
			CtrlAddMessage, 0, (LPARAM)(LPCTSTR)(pbuf));
		Count++;
	}
}

void CMachineDataOutput::Write(void*, const int)
{
	MessageBox(NULL,"peerlib pimpom",NULL,MB_OK);
}

//////////////////////////////////////////////////////////////////////////////

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

int CPeerCtrl::GetParamNamesToDlgItem(CMachine *pmac, HWND DlgItem, int AddMessage, int TypesAllowed)
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
				_snprintf(txt, 256, "[G] %i: %s", p, par->Name);
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
				_snprintf(txt, 256, "[T] %i: %s", p, par->Name);
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
			_snprintf(txt, 256, "[A] %i: %s", a, minfo->Attributes[a]->Name);
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
	return GetParamNamesToDlgItem(pmac, GetDlgItem(hWnd, ComboID), CB_ADDSTRING, TypesAllowed);
}

int CPeerCtrl::GetParamNamesToList(CMachine *pmac, HWND hWnd, int ListID, int TypesAllowed)
{
	SendMessage(GetDlgItem(hWnd, ListID), LB_RESETCONTENT, 0, 0);
	return GetParamNamesToDlgItem(pmac, GetDlgItem(hWnd, ListID), LB_ADDSTRING, TypesAllowed);
}

////////////////////////////////////////////////////////////////////////

int CPeerCtrl::FindMachine(HWND DlgItem, int SelectStringMessage)
{
	return (int) SendMessage(DlgItem,
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

void CPeerCtrl::AssignParameter(char *MacName, char *ParaName, int track)
// [G] 1: Cutoff
// 0123456
{
	CMachine *BusMachine = pCB->GetMachine(MacName);
	bool HavePar = false;
	int BusGroup, BusParNo;

	if (BusMachine != NULL)
	{
		HavePar = true;
		switch (ParaName[1])
		{
			case 'G': BusGroup = GROUP_GLOBAL;		break;
			case 'T': BusGroup = GROUP_TRACK;		break;
			case 'A': BusGroup = GROUP_ATTRIBUTE;	break;
			default:  throw PeerCtrlException("Invalid group");
		}

		BusParNo = atoi(ParaName+4);
		AssignParameter(BusMachine, BusGroup, BusParNo, track);
	}
}

///////////////////////////////////////////////////////////////////////

void CPeerCtrl::ShowPAL()
{
	HMODULE hm = LoadLibrary("Gear\\BTDSys PAL2.dll");
	if (!hm)
	{
		MessageBox(NULL, "Failed to load Buzz\\Gear\\BTDSys PAL2.dll.", "ShowPAL", MB_OK);
		return;
	}

	typedef void (*showfunc)();
	showfunc f = (showfunc)GetProcAddress(hm, "showPAL");
	if (!f)
	{
		MessageBox(NULL, "GetProcAddress failed", "ShowPAL", MB_OK);
		return;
	}

	f();

	FreeLibrary(hm);
}
