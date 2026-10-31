/*
Copyright (c) 2005-2011, Edward Powley
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

// MidiDialog.cpp : implementation file
//

#include "peerscale.h"
#include "machine.h"
#include "MidiDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MidiDialog dialog


MidiDialog::MidiDialog(CWnd* pParent /*=NULL*/)
:
CDialog(MidiDialog::IDD, pParent)
{
}

MidiDialog::~MidiDialog()
{
}

/////////////////////////////////////////////////////////////////////////////
// MidiDialog message handlers

BOOL MidiDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cmbMode.AttachDlgItem(IDC_MODECOMBO, this);
	m_cmbChannel.AttachDlgItem(IDC_CHANNELCOMBO, this);
	
	m_cmbChannel.AddString("Off");
	for (int i=1; i<=16; i++)
	{
		char txt[16];
		sprintf(txt,"%i",i);
		m_cmbChannel.AddString(txt);
	}
	m_cmbChannel.AddString("All");
	m_cmbChannel.SetCurSel(pmi->midiChannel);

	m_cmbMode.AddString("Silence non-scale notes");
	m_cmbMode.AddString("Find next note upwards");
	m_cmbMode.AddString("Find next note downwards");
	m_cmbMode.SetCurSel(pmi->midiMode);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MidiDialog::OnOK() 
{
	int sel = m_cmbChannel.GetCurSel();
	if (sel != CB_ERR)
	{
		pmi->midiChannel = sel;
	}

	sel = m_cmbMode.GetCurSel();
	if (sel != CB_ERR)
	{
		pmi->midiMode = sel;
	}
	
	CDialog::OnOK();
}
