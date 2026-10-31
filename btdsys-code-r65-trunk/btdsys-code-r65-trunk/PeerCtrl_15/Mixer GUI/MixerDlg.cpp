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

// MixerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MixerDlg.h"
#include "MixerInfo.h"

#include "Mixer GUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMixerDlg dialog


CMixerDlg::CMixerDlg(CMixerGuiPlug *p_pl, CWnd* pParent /*=NULL*/)
	: CDialog(CMixerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMixerDlg)
	//}}AFX_DATA_INIT

	ppl = p_pl;

	for (int i=0; i<MAX_CHANNELS; i++)
		channel[i] = NULL;

	numMixerChannels = 0;
	nKnobMouseMode = mm_rotwrap;

	if (Create(CMixerDlg::IDD, pParent))
	{
		ShowWindow(SW_SHOW);
	}
}

CMixerDlg::~CMixerDlg()
{
	for (int i=0; i<MAX_CHANNELS; i++)
		if (channel[i] != NULL)
			delete channel[i];
}

void CMixerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMixerDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMixerDlg, CDialog)
	//{{AFX_MSG_MAP(CMixerDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDM_ADD, OnAdd)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_HELP, OnHelp)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_KNOBCOLOUR, OnKnobcolour)
	ON_COMMAND(IDM_SLIDERCOLOUR, OnSlidercolour)
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(IDM_KNOBMIN, OnKnobmin)
	ON_COMMAND(IDM_KNOBMAX, OnKnobmax)
	ON_COMMAND(IDM_KNOBCENTER, OnKnobcenter)
	ON_COMMAND(IDM_SLIDERMIN, OnSlidermin)
	ON_COMMAND(IDM_SLIDERMAX, OnSlidermax)
	ON_COMMAND(IDM_SLIDERCENTER, OnSlidercenter)
	ON_COMMAND(ID_KNOBMODE_ROTWRAP, OnKnobmodeRotwrap)
	ON_COMMAND(ID_KNOBMODE_ROTNOWRAP, OnKnobmodeRotnowrap)
	ON_COMMAND(ID_KNOBMODE_LINHORZ, OnKnobmodeLinhorz)
	ON_COMMAND(ID_KNOBMODE_LINVERT, OnKnobmodeLinvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixerDlg message handlers

#define S_COMMON	WS_CHILD | WS_VISIBLE | WS_TABSTOP

CMixerChannel::CMixerChannel(CMixerDlg *pParent, int xpos, UINT id_base)
{
	m_pParent = pParent;
	CRect r(xpos+7,2,xpos+33,28);
	for (int i=0; i<NUM_KNOBS; i++)
	{
		knob[i] = new CMyKnobCtrl;
		knob[i]->Create(S_COMMON | TBS_HORZ, r, pParent, id_base + i);
		knob[i]->SetTicRadius(4);
		knob[i]->SetRange(0, CONTROL_RANGE - 1);
		knob[i]->SetLineSize(656);
		knob[i]->SetPageSize(6554);
		knob[i]->SetMouseMode(pParent->nKnobMouseMode);
		knob[i]->SetLinearSize(200);
		r.OffsetRect(0, 26+15);

//		TrackBinds[i] = -1;
	}

//	TrackBinds[NUM_KNOBS] = -1;

	CRect cr;
	pParent->GetClientRect(&cr);

	r.left = xpos;
	r.right = xpos + 40;
	r.top += 15;
	r.bottom = cr.bottom - 2-15;

	slider = new CMySliderCtrl;
	slider->Create(S_COMMON | TBS_VERT | TBS_AUTOTICKS | TBS_BOTH, 
		r, pParent, id_base + NUM_KNOBS);
	slider->SetTicFreq(CONTROL_RANGE / 10);
	slider->SetRange(0, CONTROL_RANGE - 1);
	slider->SetLineSize(656);
	slider->SetPageSize(6554);

	r.top = 28;
	r.bottom = 28+14;
	for (i=0; i<NUM_KNOBS; i++)
	{
		label[i] = new CEdit;
		label[i]->Create(S_COMMON | ES_AUTOHSCROLL | ES_CENTER,
			r, pParent, id_base + NUM_KNOBS + i + 1);
		label[i]->SetFont(pParent->GetFont());
		r.OffsetRect(0, 26+15);
	}

	r.OffsetRect(0, cr.bottom - r.bottom - 2);
	label[NUM_KNOBS] = new CEdit;
	label[NUM_KNOBS]->Create(S_COMMON | ES_AUTOHSCROLL | ES_CENTER,
		r, pParent, id_base + 2*NUM_KNOBS + 1);
	label[NUM_KNOBS]->SetFont(pParent->GetFont());

	r.top = 2 + (26+15)*NUM_KNOBS;
	r.left = xpos;
	r.bottom = r.top + 15;
	r.right = xpos + 18;
	
	mute = new CButton;
	mute->Create("M", S_COMMON | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		r, pParent, id_base + 2*NUM_KNOBS + 2);
	mute->SetFont(pParent->GetFont());
	r.OffsetRect(22,0);
	solo = new CButton;
	solo->Create("S", S_COMMON | BS_AUTOCHECKBOX | BS_PUSHLIKE,
		r, pParent, id_base + 2*NUM_KNOBS + 3);
	solo->SetFont(pParent->GetFont());
}

CMixerChannel::~CMixerChannel()
{
	for (int i=0; i<NUM_KNOBS; i++)
	{
		delete knob[i];
		delete label[i];
	}

	delete label[NUM_KNOBS];
	delete slider;
	delete mute;
	delete solo;
}

void CMixerChannel::Resize(int h)
{
	CRect r;

	label[NUM_KNOBS]->GetWindowRect(r);
	m_pParent->ScreenToClient(r);
	r.OffsetRect(0, h - r.bottom - 2);
	label[NUM_KNOBS]->MoveWindow(r);

	slider->GetWindowRect(r);
	m_pParent->ScreenToClient(r);
	r.bottom = h - 2-15;
	slider->MoveWindow(r);
}

BOOL CMixerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pEditBkBrush = new CBrush(RGB(0, 0, 0));

	if (ppl->mixerinfo.Valid)
	{
		CMixerInfo *minf = &ppl->mixerinfo;
		numMixerChannels = 0;

		for (int c=0; c<minf->NumChannels; c++)
		{
			channel[c] = new CMixerChannel(this, c*43 + 2, c*20 + 3000);
			for (int i=0; i<NUM_KNOBS; i++)
			{
				if (minf->chans[c]->Colours[i] != -1)
					channel[c]->knob[i]->SetKnobColour(minf->chans[c]->Colours[i]);
			}
			if (minf->chans[c]->Colours[NUM_KNOBS] != -1)
				channel[c]->slider->SetThumbColour(minf->chans[c]->Colours[NUM_KNOBS]);

			for (i=0; i<NUM_KNOBS+1; i++)
			{
				channel[c]->label[i]->SetWindowText(minf->chans[c]->Labels[i]);
//				channel[c]->TrackBinds[i] = minf->chans[c]->TrackBinds[i];
			}
			numMixerChannels++;
		}
	}
	else
	{
		numMixerChannels = 1;

		channel[0] = new CMixerChannel(this, 2, 3000);

		CMixerInfo *minf = &ppl->mixerinfo;

		minf->Valid = true;
		minf->NumChannels = 1;
		minf->chans[0] = new CChannelInfo;
		for (int i=0; i<NUM_KNOBS+1; i++)
		{
			minf->chans[0]->Colours[i] = -1;
//			minf->chans[0]->TrackBinds[i] = -1;
//			minf->chans[0]->Labels[i] = new char[128];
			minf->chans[0]->Labels[i][0] = 0;
		}
	}

	float f;
	for (int i=0; i<numMixerChannels; i++)
	{
		for (int j=0; j<NUM_KNOBS; j++)
		{
			f = ppl->pCB->GetValue(ppl->mixerinfo.chans[i]->nAxes[j]);
			if (f >= 0.0f)
				channel[i]->knob[j]->SetPos((int)(f * CONTROL_RANGE));
		}
		f = ppl->pCB->GetValue(ppl->mixerinfo.chans[i]->nAxes[NUM_KNOBS]);
		if (f >= 0.0f)
			channel[i]->slider->SetPos((int)(f * CONTROL_RANGE));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMixerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	delete m_pEditBkBrush;
	
}

HBRUSH CMixerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    switch (nCtlColor) {
    case CTLCOLOR_EDIT:
    case CTLCOLOR_MSGBOX:
         // Set color to green on black and return the background brush.
         pDC->SetTextColor(RGB(0, 255, 0));
         pDC->SetBkColor(RGB(0, 0, 0));
         return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
    default:
         return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CMixerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	for (int i=0; i<numMixerChannels; i++)
	{
		channel[i]->Resize(cy);
	}
}

void CMixerDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	lpMMI->ptMinTrackSize.y = 110 + NUM_KNOBS*(26+15);

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CMixerDlg::OnAdd() 
{
	// TODO: Add your command handler code here
	
	if (numMixerChannels == MAX_CHANNELS - 1)
		MessageBox("Sorry, you are at the maximum number of channels (i.e. stop being greedy)");
	else
	{
		channel[numMixerChannels] = new CMixerChannel(this, numMixerChannels*43 + 2, numMixerChannels*20 + 3000);
		ppl->mixerinfo.chans[numMixerChannels] = new CChannelInfo;
		for (int i=0; i<NUM_KNOBS+1; i++)
		{
			ppl->mixerinfo.chans[numMixerChannels]->Colours[i] = -1;
//			ppl->mixerinfo.chans[numMixerChannels]->TrackBinds[i] = -1;
//			ppl->mixerinfo.chans[numMixerChannels]->Labels[i] = new char[128];
			ppl->mixerinfo.chans[numMixerChannels]->Labels[i][0] = 0;
		}

		char txt[256];
		for (i=0; i<NUM_KNOBS; i++)
		{
			sprintf(txt,"Channel %i knob %i", numMixerChannels, i);
			ppl->mixerinfo.chans[numMixerChannels]->nAxes[i] = ppl->pCB->AddAxis(txt);
		}

		sprintf(txt,"Channel %i fader", numMixerChannels);
		ppl->mixerinfo.chans[numMixerChannels]->nAxes[NUM_KNOBS] = ppl->pCB->AddAxis(txt);

		numMixerChannels++;
		ppl->mixerinfo.NumChannels++;
	}
}

void CMixerDlg::OnDelete() 
{
	// TODO: Add your command handler code here
	
	if (numMixerChannels <= 1)
		MessageBox("You should keep at least 1 mixer channel!");
	else
	{
		if (MessageBox("Really delete last channel?",NULL,MB_YESNO) == IDYES)
		{
			numMixerChannels--;
			delete channel[numMixerChannels];
			channel[numMixerChannels] = NULL;

			for (int i=NUM_KNOBS; i>=0; i--)
				ppl->pCB->DelAxis(ppl->mixerinfo.chans[numMixerChannels]->nAxes[i]);

			ppl->mixerinfo.NumChannels--;
			delete ppl->mixerinfo.chans[numMixerChannels];
			ppl->mixerinfo.chans[numMixerChannels] = NULL;
		}
	}
}

void CMixerDlg::OnHelp() 
{
	// TODO: Add your command handler code here

	MessageBox("Help is on its way. It will reach you in mid 2003");
}

void CMixerDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
	bool DisplayedPopup = false;

	for (int i=0; i<numMixerChannels; i++)
	{
		for (int j=0; j<NUM_KNOBS; j++)
		{
			if (channel[i]->knob[j] == pWnd)
			{
				DisplayedPopup = true;
				m_PopupChannel = i;
				m_PopupKnob = j;

				CMenu mPopup;
				mPopup.LoadMenu(IDR_MIXERPOPUP);

				TrackPopupMenu(GetSubMenu(mPopup.m_hMenu, 0),
					TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
					point.x, point.y, 0,
					m_hWnd,
					NULL);
			}
		}
		if (channel[i]->slider == pWnd)
		{
			DisplayedPopup = true;
			m_PopupChannel = i;
			m_PopupKnob = NUM_KNOBS;

			CMenu mPopup;
			mPopup.LoadMenu(IDR_MIXERPOPUP);

			TrackPopupMenu(mPopup.GetSubMenu(1)->m_hMenu,
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				point.x, point.y, 0,
				m_hWnd,
				NULL);
		}
	}

	if (!DisplayedPopup)
		CDialog::OnContextMenu(pWnd, point);
}

void CMixerDlg::OnKnobcolour() 
{
	// TODO: Add your command handler code here
	
	static COLORREF customs[16];

	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = NULL;
	cc.rgbResult = channel[m_PopupChannel]->knob[m_PopupKnob]->GetKnobColour();
	cc.lpCustColors = customs;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		channel[m_PopupChannel]->knob[m_PopupKnob]->SetKnobColour(cc.rgbResult);
		ppl->mixerinfo.chans[m_PopupChannel]->Colours[m_PopupKnob] = cc.rgbResult;
	}
}

void CMixerDlg::OnSlidercolour() 
{
	// TODO: Add your command handler code here
	
	static COLORREF customs[16];

	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = NULL;
	cc.rgbResult = channel[m_PopupChannel]->slider->GetThumbColour();
	cc.lpCustColors = customs;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		channel[m_PopupChannel]->slider->SetThumbColour(cc.rgbResult);
		ppl->mixerinfo.chans[m_PopupChannel]->Colours[NUM_KNOBS] = cc.rgbResult;
	}
}

void CMixerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	ppl->placement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&ppl->placement);

	CDialog::OnClose();

	ppl->pmixerdlg = NULL;
	DestroyWindow();
}

BOOL CMixerDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	HWND hWnd = (HWND)lParam;

	if (HIWORD(wParam) == EN_CHANGE)
	{
		for (int c=0; c<numMixerChannels; c++)
		{
			for (int k=0; k<NUM_KNOBS+1; k++)
			{
				if (hWnd == channel[c]->label[k]->m_hWnd)
				{
					channel[c]->label[k]->GetWindowText(
									ppl->mixerinfo.chans[c]->Labels[k],
									128);

					char txt[256];
					sprintf(txt,"Channel %i",c);
					if (k < NUM_KNOBS)
						sprintf(txt,"%s knob %i",txt,k);
					else
						sprintf(txt,"%s fader",txt);

					if (ppl->mixerinfo.chans[c]->Labels[k][0])
					{
						sprintf(txt,"%s - \"%s\"",txt,ppl->mixerinfo.chans[c]->Labels[k]);
					}

					ppl->pCB->RenameAxis(ppl->mixerinfo.chans[c]->nAxes[k],txt);
				}
			}
		}
	}
	else if (HIWORD(wParam) == BN_CLICKED)
	{
		for (int c=0; c<numMixerChannels; c++)
		{
			if (hWnd == channel[c]->mute->m_hWnd)
			{
				if (channel[c]->mute->GetCheck() == BST_CHECKED)
					ppl->pCB->SetValue(
						ppl->mixerinfo.chans[c]->nAxes[NUM_KNOBS],
						0.0f);
				else
					ppl->pCB->SetValue(
						ppl->mixerinfo.chans[c]->nAxes[NUM_KNOBS],
						(float)channel[c]->slider->GetPos() / (float)CONTROL_RANGE);
			}
			else if (hWnd == channel[c]->solo->m_hWnd)
			{
				if (channel[c]->solo->GetCheck() == BST_CHECKED)
				{
					bSoloMode = true;
					ppl->pCB->SetValue(
						ppl->mixerinfo.chans[c]->nAxes[NUM_KNOBS],
						(float)channel[c]->slider->GetPos() / (float)CONTROL_RANGE);					
					
					for (int d=0; d<numMixerChannels; d++)
					{
						if (d != c)
						{
							channel[d]->solo->SetCheck(BST_UNCHECKED);
							ppl->pCB->SetValue(
								ppl->mixerinfo.chans[d]->nAxes[NUM_KNOBS],
								0.0f);
						}
					}
				}
				else
				{
					bSoloMode = false;
					for (int d=0; d<numMixerChannels; d++)
					{
						if (channel[d]->mute->GetCheck() == BST_CHECKED)
							ppl->pCB->SetValue(
								ppl->mixerinfo.chans[d]->nAxes[NUM_KNOBS],
								0.0f);
						else
							ppl->pCB->SetValue(
								ppl->mixerinfo.chans[d]->nAxes[NUM_KNOBS],
								(float)channel[d]->slider->GetPos() / (float)CONTROL_RANGE);
					}
				}
			}
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CMixerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	for (int c=0; c<numMixerChannels; c++)
	{
		for (int k=0; k<NUM_KNOBS; k++)
		{
			if (pScrollBar == (CScrollBar*)channel[c]->knob[k])
				ppl->pCB->SetValue(
					ppl->mixerinfo.chans[c]->nAxes[k],
					(float)channel[c]->knob[k]->GetPos() / (float)CONTROL_RANGE);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMixerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	for (int c=0; c<numMixerChannels; c++)
	{
		if (pScrollBar == (CScrollBar*)channel[c]->slider)
		{
			if (channel[c]->mute->GetCheck() != BST_CHECKED && !bSoloMode)
			{
				ppl->pCB->SetValue(
						ppl->mixerinfo.chans[c]->nAxes[NUM_KNOBS],
						(float)channel[c]->slider->GetPos() / (float)CONTROL_RANGE);
			}
		}
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMixerDlg::OnKnobmin() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->knob[m_PopupKnob]->SetPos(0);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[m_PopupKnob], 0.0f);
}

void CMixerDlg::OnKnobmax() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->knob[m_PopupKnob]->SetPos(CONTROL_RANGE-1);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[m_PopupKnob], 1.0f);
}

void CMixerDlg::OnKnobcenter() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->knob[m_PopupKnob]->SetPos(CONTROL_RANGE/2);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[m_PopupKnob], 0.5f);
}

void CMixerDlg::OnSlidermin() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->slider->SetPos(0);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[NUM_KNOBS], 0.0f);
}

void CMixerDlg::OnSlidermax() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->slider->SetPos(CONTROL_RANGE-1);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[NUM_KNOBS], 1.0f);
}

void CMixerDlg::OnSlidercenter() 
{
	// TODO: Add your command handler code here
	channel[m_PopupChannel]->slider->SetPos(CONTROL_RANGE/2);

	ppl->pCB->SetValue(ppl->mixerinfo.chans[m_PopupChannel]->nAxes[NUM_KNOBS], 0.5f);
}

void CMixerDlg::UpdateKnobMouseModes()
{
	for (int c=0; c<numMixerChannels; c++)
	{
		for (int k=0; k<NUM_KNOBS; k++)
		{
			channel[c]->knob[k]->SetMouseMode(nKnobMouseMode);
		}
	}
}

void CMixerDlg::OnKnobmodeRotwrap() 
{
	CMenu *pm = GetMenu();
	pm->CheckMenuItem(ID_KNOBMODE_ROTWRAP  ,MF_CHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_ROTNOWRAP,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINHORZ  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINVERT  ,MF_UNCHECKED);

	nKnobMouseMode = mm_rotwrap;
	UpdateKnobMouseModes();
}

void CMixerDlg::OnKnobmodeRotnowrap() 
{
	CMenu *pm = GetMenu();
	pm->CheckMenuItem(ID_KNOBMODE_ROTWRAP  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_ROTNOWRAP,MF_CHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINHORZ  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINVERT  ,MF_UNCHECKED);

	nKnobMouseMode = mm_rotnowrap;
	UpdateKnobMouseModes();
}

void CMixerDlg::OnKnobmodeLinhorz() 
{
	CMenu *pm = GetMenu();
	pm->CheckMenuItem(ID_KNOBMODE_ROTWRAP  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_ROTNOWRAP,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINHORZ  ,MF_CHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINVERT  ,MF_UNCHECKED);

	nKnobMouseMode = mm_linhoriz;
	UpdateKnobMouseModes();
}

void CMixerDlg::OnKnobmodeLinvert() 
{
	CMenu *pm = GetMenu();
	pm->CheckMenuItem(ID_KNOBMODE_ROTWRAP  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_ROTNOWRAP,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINHORZ  ,MF_UNCHECKED);
	pm->CheckMenuItem(ID_KNOBMODE_LINVERT  ,MF_CHECKED);

	nKnobMouseMode = mm_linvert;
	UpdateKnobMouseModes();
}

