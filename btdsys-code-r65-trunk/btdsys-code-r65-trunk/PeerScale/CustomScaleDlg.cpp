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

// CustomScaleDlg.cpp : implementation file
//

#include "peerscale.h"
#include "CustomScaleDlg.h"
#include "machine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomScaleDlg dialog


CCustomScaleDlg::CCustomScaleDlg(CWnd* pParent /*=NULL*/)
:
CDialog(CCustomScaleDlg::IDD, pParent)
{
}

CCustomScaleDlg::~CCustomScaleDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCustomScaleDlg message handlers

BOOL CCustomScaleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Attach controls
	m_edit.AttachDlgItem(IDC_EDIT, this);
	
	if (!pmi->userscales.empty())
    {
		m_edit.SetWindowText(pmi->userscales.c_str());
    }
    else
    {
        m_edit.SetWindowText(
            "Enter user scales in this box.\r\n"
            "Format: notes followed by name, for example \"3 1 1 3 2 1 1 Chromatic Phrygian\"\r\n"
            "For best (non-experimental) results, make sure the numbers add up to 12.\r\n"
            "Put each scale on a new line. Any lines not in the above format are ignored.\r\n\r\n"
			);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomScaleDlg::OnOK() 
{
	pmi->userscales = m_edit.GetWindowText();

	pmi->AddScales(pmi->userscales.c_str(), pmi->numBuiltinScales);

	CDialog::OnOK();
}
