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

// EditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerState.h"
#include "EditDialog.h"
#include "peerstate.h"
#include "hack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDialog dialog


CEditDialog::CEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDialog)
	DDX_Control(pDX, IDC_MIDINOTEB, m_txtMidiNoteB);
	DDX_Control(pDX, IDC_MIDINOTEA, m_txtMidiNoteA);
	DDX_Control(pDX, IDC_TREEB, m_treeB);
	DDX_Control(pDX, IDC_TREEA, m_treeA);
	DDX_Control(pDX, IDC_COMBOB, m_comboB);
	DDX_Control(pDX, IDC_COMBOA, m_comboA);
	DDX_Control(pDX, IDC_AMOUNTTEXT, m_txtAmount);
	DDX_Control(pDX, IDC_AMOUNTSLIDER, m_slidAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditDialog, CDialog)
	//{{AFX_MSG_MAP(CEditDialog)
	ON_CBN_SELCHANGE(IDC_COMBOA, OnComboASelchange)
	ON_CBN_SELCHANGE(IDC_COMBOB, OnComboBSelchange)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteBtn)
	ON_BN_CLICKED(IDC_SWAP, OnSwapBtn)
	ON_BN_CLICKED(IDC_COPY, OnCopyBtn)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_MORPH, OnMorphBtn)
	ON_BN_CLICKED(IDC_SPLICE, OnSpliceBtn)
	ON_BN_CLICKED(IDC_RANDOMISE, OnRandomiseBtn)
	ON_BN_CLICKED(IDC_SETCURRENTA, OnSetCurrentA)
	ON_BN_CLICKED(IDC_SETCURRENTB, OnSetCurrentB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDialog message handlers

inline float frand()
// returns random value between 0 and 1
// from Waveout.cpp
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

const char *NoteNames[] = {
	"C-", "C#",
	"D-", "D#",
	"E-",
	"F-", "F#",
	"G-", "G#",
	"A-", "A#",
	"B-"
};

inline char *GetMIDINoteName (const int n)
{
	int oct = (int)(n/12) - 1;
	int note = (n % 12);
	static char txt[16];
	sprintf(txt,"%s%i",NoteNames[note],oct);
	return txt;
}

BOOL CEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_slidAmount.SetRange(0,100);
	m_slidAmount.SetTicFreq(5);
	m_slidAmount.SetPos(50);

	char txt[256];
	for (int s=0; s<NUM_SNAPSHOTS; s++)
	{
		sprintf(txt,"%s%i: %s",pmi->GotSnap(s) ? "*" : "",s,pmi->snapname[s]);
		m_comboA.AddString(txt);
		m_comboB.AddString(txt);
	}

	m_comboA.SetCurSel(pmi->nCurrentSnapshot); PopulateTree(&m_treeA, pmi->nCurrentSnapshot);
	m_comboB.SetCurSel(pmi->nCurrentSnapshot); PopulateTree(&m_treeB, pmi->nCurrentSnapshot);
	sprintf(txt,"MIDI note: %s",GetMIDINoteName(pmi->nCurrentSnapshot));
	m_txtMidiNoteA.SetWindowText(txt);
	m_txtMidiNoteB.SetWindowText(txt);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditDialog::OnComboASelchange() 
{
	int s = m_comboA.GetCurSel();
	if (s != CB_ERR)
	{
		PopulateTree(&m_treeA, s);
		char txt[256];
		sprintf(txt,"MIDI note: %s",GetMIDINoteName(s));
		m_txtMidiNoteA.SetWindowText(txt);
	}
}

void CEditDialog::OnComboBSelchange() 
{
	int s = m_comboB.GetCurSel();
	if (s != CB_ERR)
	{
		PopulateTree(&m_treeB, s);
		char txt[256];
		sprintf(txt,"MIDI note: %s",GetMIDINoteName(s));
		m_txtMidiNoteB.SetWindowText(txt);
	}
}

void CEditDialog::PopulateTree(CTreeCtrl *ptree, int s)
{
	ptree->DeleteAllItems();

	char txt[1024];

	for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
	{
		HTREEITEM hi = ptree->InsertItem(pmi->pCB->GetMachineName(p->pMac));
		const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(p->pMac);
		int i;
		if (p->Snapshots[s].pg)
		{
			byte *pb = p->Snapshots[s].pg;
			int v;

			for (i=0; i<pinfo->numGlobalParameters; i++)
			{
				const CMachineParameter *par = pinfo->Parameters[i];
				if (p->vbParamActive[i] && par->Flags & MPF_STATE)
				{
					if (par->Type == pt_word)
						v = *(word *)pb;
					else
						v = *pb;

					sprintf(txt,"[G] %s = %i / 0x%X [%s]",par->Name,v,v,
						GetMI(p->pMac)->DescribeValue(i,v));

					ptree->InsertItem(txt,hi);
				}
				pb++; if (par->Type == pt_word) pb++;
			}
		}
		if (p->Snapshots[s].pt)
		{
			byte *pb = p->Snapshots[s].pt;
			int v;
			int numtracks;
			if (pmi->nHostVersion < 2)
				numtracks = Hack_GetNumTracks(p->pMac);
			else
				numtracks = pmi->pCB->GetNumTracks(p->pMac);

			for (int t=0; t<numtracks; t++)
			{
				for (i=pinfo->numGlobalParameters; i<pinfo->numGlobalParameters+pinfo->numTrackParameters; i++)
				{
					const CMachineParameter *par = pinfo->Parameters[i];
					if (p->vbParamActive[i] && par->Flags & MPF_STATE)
					{
						if (par->Type == pt_word)
							v = *(word *)pb;
						else
							v = *pb;

						sprintf(txt,"[T %i] %s = %i / 0x%X [%s]",t,par->Name,v,v,
							GetMI(p->pMac)->DescribeValue(i,v));

						ptree->InsertItem(txt,hi);
					}
					pb++; if (par->Type == pt_word) pb++;
				}
			}
		}
	}
}

void CEditDialog::OnDeleteBtn() 
{
	int s = m_comboA.GetCurSel();
	if (s != CB_ERR)
	{
		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			if (p->Snapshots[s].pg) delete[] p->Snapshots[s].pg;
			if (p->Snapshots[s].pt) delete[] p->Snapshots[s].pt;
			p->Snapshots[s].pg = p->Snapshots[s].pt = NULL;
		}
		pmi->snapname[s][0] = 0;

		UpdateComboItem(&m_comboA, s);
		UpdateComboItem(&m_comboB, s);
		PopulateTree(&m_treeA, s);
	}	
}

void CEditDialog::OnSwapBtn() 
{
	int a = m_comboA.GetCurSel();
	int b = m_comboB.GetCurSel();

	if (a != CB_ERR && b != CB_ERR && a != b)
	{
		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			byte *t;
			t = p->Snapshots[a].pg; p->Snapshots[a].pg = p->Snapshots[b].pg; p->Snapshots[b].pg = t;
			t = p->Snapshots[a].pt; p->Snapshots[a].pt = p->Snapshots[b].pt; p->Snapshots[b].pt = t;
		}

		char t[32];
		strcpy(t,pmi->snapname[a]); strcpy(pmi->snapname[a],pmi->snapname[b]); strcpy(pmi->snapname[b],t);

		UpdateComboItem(&m_comboA, a);
		UpdateComboItem(&m_comboA, b);
		UpdateComboItem(&m_comboB, a);
		UpdateComboItem(&m_comboB, b);
		PopulateTree(&m_treeA, a);
		PopulateTree(&m_treeB, b);

		pmi->SetModifiedFlag();
	}	
}

void CEditDialog::UpdateComboItem(CComboBox *pcombo, int s)
{
	int sel = pcombo->GetCurSel();

	pcombo->DeleteString(s);
	char txt[256];
	sprintf(txt,"%s%i: %s",pmi->GotSnap(s) ? "*" : "",s,pmi->snapname[s]);
	pcombo->InsertString(s,txt);

	pcombo->SetCurSel(sel);
}

void CEditDialog::OnCopyBtn() 
{
	int a = m_comboA.GetCurSel();
	int b = m_comboB.GetCurSel();

	if (a != CB_ERR && b != CB_ERR && a != b)
	{
		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			if (p->Snapshots[b].pg)
			{
				if (!p->Snapshots[a].pg && p->nGStateSize > 0)
					p->Snapshots[a].pg = new byte[p->nGStateSize];
				memcpy(p->Snapshots[a].pg, p->Snapshots[b].pg, p->nGStateSize);
			}
			else
			{
				if (p->Snapshots[a].pg) delete[] p->Snapshots[a].pg;
				p->Snapshots[a].pg = NULL;
			}

			if (p->Snapshots[b].pt)
			{
				if (!p->Snapshots[a].pt && p->nTStateSize > 0)
					p->Snapshots[a].pt = new byte[p->nTStateSize];
				memcpy(p->Snapshots[a].pt, p->Snapshots[b].pt, p->nTStateSize);
			}
			else
			{
				if (p->Snapshots[a].pt) delete[] p->Snapshots[a].pt;
				p->Snapshots[a].pt = NULL;
			}
		}

		strcpy(pmi->snapname[a],pmi->snapname[b]);

		UpdateComboItem(&m_comboA, a);
		UpdateComboItem(&m_comboB, a);
		PopulateTree(&m_treeA, a);

		pmi->SetModifiedFlag();
	}		
}

void CEditDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char txt[16];
	sprintf(txt,"%i%%",m_slidAmount.GetPos());
	m_txtAmount.SetWindowText(txt);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditDialog::OnMorphBtn() 
{
	int a = m_comboA.GetCurSel();
	int b = m_comboB.GetCurSel();

	if (a != CB_ERR && b != CB_ERR && a != b)
	{
		float fb = (float)m_slidAmount.GetPos() / 100.0f;
		float fa = 1.0f - fb;

		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(p->pMac);
			int i;
			if (p->Snapshots[a].pg && p->Snapshots[b].pg)
			{
				byte *pa = p->Snapshots[a].pg;
				byte *pb = p->Snapshots[b].pg;
				int va, vb;
				const CMachineParameter *par = pinfo->Parameters[0];

				for (i=0; i<pinfo->numGlobalParameters; i++)
				{
					if (p->vbParamActive[i] && par->Flags & MPF_STATE)
					{
						if (par->Type == pt_word)
						{
							va = *(word *)pa;
							vb = *(word *)pb;
						}
						else
						{
							va = *pa;
							vb = *pb;
						}

						va = (int)(va*fa + vb*fb);
						if (par->Type == pt_word)
							*(word *)pa = va;
						else
							*pa = va;
					}
					pa++; pb++; if (par->Type == pt_word) {pa++; pb++;}
					par++;
				}
			}
			if (p->Snapshots[a].pt && p->Snapshots[b].pt)
			{
				byte *pa = p->Snapshots[a].pt;
				byte *pb = p->Snapshots[b].pt;
				int va, vb;
				for (int t=0; t<pinfo->maxTracks; t++)
				{
					const CMachineParameter *par = pinfo->Parameters[pinfo->numGlobalParameters];

					for (i=pinfo->numGlobalParameters; i<pinfo->numGlobalParameters+pinfo->numTrackParameters; i++)
					{
						if (p->vbParamActive[i] && par->Flags & MPF_STATE)
						{
							if (par->Type == pt_word)
							{
								va = *(word *)pa;
								vb = *(word *)pb;
							}
							else
							{
								va = *pa;
								vb = *pb;
							}

							va = (int)(va*fa + vb*fb);
							if (par->Type == pt_word)
								*(word *)pa = va;
							else
								*pa = va;
						}
						pa++; pb++; if (par->Type == pt_word) {pa++; pb++;}
						par++;
					}
				}
			}
		}

		PopulateTree(&m_treeA, a);

		pmi->SetModifiedFlag();
	}
}

void CEditDialog::OnSpliceBtn() 
{
	int a = m_comboA.GetCurSel();
	int b = m_comboB.GetCurSel();

	if (a != CB_ERR && b != CB_ERR && a != b)
	{
		float fb = (float)m_slidAmount.GetPos() / 100.0f;

		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(p->pMac);
			int i;
			if (p->Snapshots[a].pg && p->Snapshots[b].pg)
			{
				byte *pa = p->Snapshots[a].pg;
				byte *pb = p->Snapshots[b].pg;
				int va, vb;
				const CMachineParameter *par = pinfo->Parameters[0];

				for (i=0; i<pinfo->numGlobalParameters; i++)
				{
					if (p->vbParamActive[i] && par->Flags & MPF_STATE)
					{
						if (par->Type == pt_word)
						{
							va = *(word *)pa;
							vb = *(word *)pb;
						}
						else
						{
							va = *pa;
							vb = *pb;
						}

						va = (frand() < fb) ? vb : va;
						if (par->Type == pt_word)
							*(word *)pa = va;
						else
							*pa = va;
					}
					pa++; pb++; if (par->Type == pt_word) {pa++; pb++;}
					par++;
				}
			}
			if (p->Snapshots[a].pt && p->Snapshots[b].pt)
			{
				byte *pa = p->Snapshots[a].pt;
				byte *pb = p->Snapshots[b].pt;
				int va, vb;
				for (int t=0; t<pinfo->maxTracks; t++)
				{
					const CMachineParameter *par = pinfo->Parameters[pinfo->numGlobalParameters];

					for (i=pinfo->numGlobalParameters; i<pinfo->numGlobalParameters+pinfo->numTrackParameters; i++)
					{
						if (p->vbParamActive[i] && par->Flags & MPF_STATE)
						{
							if (par->Type == pt_word)
							{
								va = *(word *)pa;
								vb = *(word *)pb;
							}
							else
							{
								va = *pa;
								vb = *pb;
							}

							va = (frand() < fb) ? vb : va;
							if (par->Type == pt_word)
								*(word *)pa = va;
							else
								*pa = va;
						}
						pa++; pb++; if (par->Type == pt_word) {pa++; pb++;}
						par++;
					}
				}
			}
		}

		PopulateTree(&m_treeA, a);

		pmi->SetModifiedFlag();
	}
}

void CEditDialog::OnRandomiseBtn() 
{
	int a = m_comboA.GetCurSel();

	if (a != CB_ERR)
	{
		float famt = (float)m_slidAmount.GetPos() / 100.0f;

		for (vector<CMachineRec>::iterator p = pmi->peers.begin(); p < pmi->peers.end(); p++)
		{
			const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(p->pMac);
			int i;
			if (p->Snapshots[a].pg)
			{
				byte *pa = p->Snapshots[a].pg;
				int va;
				const CMachineParameter *par = pinfo->Parameters[0];

				for (i=0; i<pinfo->numGlobalParameters; i++)
				{
					if (p->vbParamActive[i] && par->Flags & MPF_STATE)
					{
						if (par->Type == pt_word)
						{
							va = *(word *)pa;
						}
						else
						{
							va = *pa;
						}

						float range = (par->MaxValue - par->MinValue) * famt;
						float min = va - range;
						float max = va + range;
						if (par->Type != pt_switch)
						{
							if (min < (float)par->MinValue) min = (float)par->MinValue;
							if (max > (float)par->MaxValue) max = (float)par->MaxValue;
						}
						else
						{
							if (min < 0.0f) min = 0.0f;
							if (max > 1.0f) max = 1.0f;
						}

						va = (int)(min + frand() * (max - min));
						if (par->Type == pt_word)
							*(word *)pa = va;
						else
							*pa = va;
					}
					pa++; if (par->Type == pt_word) {pa++;}
					par++;
				}
			}
			if (p->Snapshots[a].pt)
			{
				byte *pa = p->Snapshots[a].pt;
				int va;
				for (int t=0; t<pinfo->maxTracks; t++)
				{
					const CMachineParameter *par = pinfo->Parameters[pinfo->numGlobalParameters];

					for (i=pinfo->numGlobalParameters; i<pinfo->numGlobalParameters+pinfo->numTrackParameters; i++)
					{
						if (p->vbParamActive[i] && par->Flags & MPF_STATE)
						{
							if (par->Type == pt_word)
							{
								va = *(word *)pa;
							}
							else
							{
								va = *pa;
							}

							float range = (par->MaxValue - par->MinValue) * famt;
							float min = va - range;
							float max = va + range;
							if (min < (float)par->MinValue) min = (float)par->MinValue;
							if (max > (float)par->MaxValue) max = (float)par->MaxValue;

							va = (int)(min + frand() * (max - min));
							if (par->Type == pt_word)
								*(word *)pa = va;
							else
								*pa = va;
						}
						pa++; if (par->Type == pt_word) {pa++;}
						par++;
					}
				}
			}
		}

		PopulateTree(&m_treeA, a);

		pmi->SetModifiedFlag();
	}
}

void CEditDialog::OnSetCurrentA() 
{
	int s = m_comboA.GetCurSel();
	pmi->pCB->ControlChange(pmi->thismac,1,0,0,s);
}

void CEditDialog::OnSetCurrentB() 
{
	int s = m_comboB.GetCurSel();
	pmi->pCB->ControlChange(pmi->thismac,1,0,0,s);
}
