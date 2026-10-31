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

// CAddMachineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys groupie.h"
#include "AddMachineDlg.h"
#include "dbgprintf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddMachineDlg dialog


CAddMachineDlg::CAddMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMachineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddMachineDlg)
	//}}AFX_DATA_INIT
}


void CAddMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddMachineDlg)
	DDX_Control(pDX, IDC_TREE, m_MacTree);
	DDX_Control(pDX, IDC_SEARCHBOX, m_Search);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddMachineDlg, CDialog)
	//{{AFX_MSG_MAP(CAddMachineDlg)
	ON_EN_CHANGE(IDC_SEARCHBOX, OnChangeSearchbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddMachineDlg message handlers

BOOL CAddMachineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CheckRadioButton(IDC_RALL,IDC_RSEARCH,IDC_RALL);

	PopulateMacList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CAddMachineDlg::ShouldShowMachine(int mode, char *_name)
{
	switch (mode)
	{

	case IDC_RSEARCH:
		{
			char word[256];
			char srch[256];
			char name[256];
			strcpy(name,_name);
			m_Search.GetWindowText(srch,255);
			strlwr(srch);
			strlwr(name);

			char *p = srch;
			char *q = word;

			while (*p)
			{
				if (*p != ' ')
					*q++ = *p++;
				else
				{
					*q = 0;
					p++;

					if (!strstr(name, word))
						return false;

					q = word;
				}
			}

			*q = 0;
			if (!strstr(name, word))
				return false;

			return true;
		}			
		
	case IDC_RRECENT:
	case IDC_RALL:
	case IDC_RGENS:
	case IDC_REFFS:
	default:
		return true;


	}


}


void GetTrimmedLine(FILE *f, char *buf)
{
	char *c = buf;
	do {fread(c,1,1,f);} while ((*c == ' ' || *c == '\t') && !feof(f));
	while (*c != '\n' && *c != '\r' && !feof(f)) fread(++c,1,1,f);
	*c-- = 0;
	while (*c == ' ' || *c == '\t') *c-- = 0;
}

void CAddMachineDlg::PopulateMacList()
{
//	int m_MacList;

	WIN32_FIND_DATA fdata;

	int mode = GetCheckedRadioButton(IDC_RALL, IDC_RRECENT);

	dbgprintf("%i is maybe %i",mode,IDC_RRECENT);

	m_MacTree.DeleteAllItems();

	char buzz_path[256];
	buzz_path[0] = 0;
	
	HKEY hk;	
	if (RegOpenKey(HKEY_CURRENT_USER,"Software\\Jeskola\\Buzz\\Settings",&hk)==ERROR_SUCCESS)
	{		
		DWORD dwType;
		DWORD dwSize=MAX_PATH;					
		if (RegQueryValueEx(hk,"BuzzPath",NULL,&dwType,(BYTE *)buzz_path,&dwSize)==ERROR_SUCCESS)
		{							
			int len=strlen(buzz_path);
			if (buzz_path[len-1]!='\\') 
			{
				buzz_path[len]='\\';
				buzz_path[len+1]=0;
			}
		}
		RegCloseKey(hk);
	}	

	if (buzz_path[0])
	{
		if (mode == IDC_RRECENT)
		{
			char fname[256];
			strcpy(fname,buzz_path);
			strcat(fname,"gear\\index.txt");

			FILE *fidx = fopen(fname,"rt");
			if (fidx)
			{
				HTREEITEM hParent = TVI_ROOT;

				while (!feof(fidx))
				{
					char buf[1024];
					GetTrimmedLine(fidx,buf);
					if (buf[0] == '/')
					{
						if (strcmp(buf,"/..") == 0)
						{
							if (hParent != TVI_ROOT) hParent = m_MacTree.GetParentItem(hParent);
						}
						else
						{
							hParent = m_MacTree.InsertItem(buf+1,hParent);
						}
					}
					else
					{
						char *c = buf;
						while (*c) if (*c == ',') *c = 0; else c++;
						strcpy(c,".dll");

						FILE *f = NULL;
						char fname[256];

// fixme: opening files like this is SLOW
/*						strcpy(fname,buzz_path);
						strcat(fname,"gear\\generators\\");
						strcat(fname,buf);

						f = fopen(fname,"rb");

						if (!f)
						{
							strcpy(fname,buzz_path);
							strcat(fname,"gear\\effects\\");
							strcat(fname,buf);

							f = fopen(fname,"rb");
						}

						if (f)
*/						{
m_MacTree.InsertItem(buf,hParent);
//							fclose(f);
						}

					}
				}

				fclose(fidx);
			}
		}
		else
		{
			if (mode != IDC_REFFS)
			{
				char srchpatt[256];
				strcpy(srchpatt, buzz_path);
				strcat(srchpatt, "gear\\generators\\*.dll");
				HANDLE hfind = FindFirstFile(
					srchpatt,
					&fdata);

				if (hfind != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (ShouldShowMachine(mode,fdata.cFileName))
							m_MacTree.InsertItem(fdata.cFileName);
					}
					while (FindNextFile(hfind, &fdata) != 0);
				}

				FindClose(hfind);
			}

			if (mode != IDC_RGENS)
			{
				char srchpatt[256];
				strcpy(srchpatt, buzz_path);
				strcat(srchpatt, "gear\\effects\\*.dll");
				HANDLE hfind = FindFirstFile(
					srchpatt,
					&fdata);

				if (hfind != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (ShouldShowMachine(mode,fdata.cFileName))
							m_MacTree.InsertItem(fdata.cFileName);
					}
					while (FindNextFile(hfind, &fdata) != 0);
				}

				FindClose(hfind);
			}
		}
	}
}

BOOL CAddMachineDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	int wID = LOWORD(wParam);
	if (wID == IDC_RALL || wID == IDC_RGENS || wID == IDC_REFFS ||
		wID == IDC_RRECENT || wID == IDC_RSEARCH)
	{
		PopulateMacList();
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CAddMachineDlg::OnChangeSearchbox() 
{
	CheckRadioButton(IDC_RALL, IDC_RSEARCH, IDC_RSEARCH);
	PopulateMacList();
}

void CAddMachineDlg::OnOK() 
{
	// TODO: Add extra validation here
	
/*	int s = m_MacList.GetCurSel();
	if (s != LB_ERR)
	{
		m_MacList.GetText(s,m_szMacName);
	}
	else
		m_szMacName[0] = 0;
*/
	HTREEITEM h = m_MacTree.GetSelectedItem();
	strcpy(m_szMacName,m_MacTree.GetItemText(h));

	CDialog::OnOK();
}

/*void CAddMachineDlg::OnIndextxt() 
{
	PopulateMacList();
}
*/
