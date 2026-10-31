// CAddMachineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys groupie.h"
#include "AddMachineDlg.h"

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
	DDX_Control(pDX, IDC_LIST, m_MacList);
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

bool CAddMachineDlg::ShouldShowMachine(int mode, WIN32_FIND_DATA fdata)
{
	switch (mode)
	{

	case IDC_RSEARCH:
		{
			char word[256];
			char srch[256];
			m_Search.GetWindowText(srch,255);
			strlwr(srch);
			strlwr(fdata.cFileName);

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

					if (!strstr(fdata.cFileName, word))
						return false;

					q = word;
				}
			}

			*q = 0;
			if (!strstr(fdata.cFileName, word))
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

void CAddMachineDlg::PopulateMacList()
{
	WIN32_FIND_DATA fdata;

	int mode = GetCheckedRadioButton(IDC_RALL, IDC_RSEARCH);

	m_MacList.ResetContent();

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
					if (ShouldShowMachine(mode,fdata))
						m_MacList.AddString(fdata.cFileName);
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
					if (ShouldShowMachine(mode,fdata))
						m_MacList.AddString(fdata.cFileName);				
				}
				while (FindNextFile(hfind, &fdata) != 0);
			}

			FindClose(hfind);
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
	
	int s = m_MacList.GetCurSel();
	if (s != LB_ERR)
	{
		m_MacList.GetText(s,m_szMacName);
	}
	else
		m_szMacName[0] = 0;

	CDialog::OnOK();
}
