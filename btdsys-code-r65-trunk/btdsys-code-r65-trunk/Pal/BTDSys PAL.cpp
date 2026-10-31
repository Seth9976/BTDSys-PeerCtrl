/*
BTDSys PAL.cpp: Defines the initialization routines for the DLL.
Copyright (c) 2005, Edward Powley
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
#include "BTDSys PAL.h"
#include "PALWindow.h"

#include "../mdk/machineinterface.h"
#include "CMachine.h"
#include "palpalfuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CBTDSysPALApp

BEGIN_MESSAGE_MAP(CBTDSysPALApp, CWinApp)
	//{{AFX_MSG_MAP(CBTDSysPALApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBTDSysPALApp construction

CBTDSysPALApp::CBTDSysPALApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	ppw = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBTDSysPALApp object

CBTDSysPALApp theApp;


HWND FindBUZZhwnd()
{
  char t[1024];
  HWND wnd = GetActiveWindow();

  if(wnd == NULL)
    return NULL;

  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDNEXT);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDPREV);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  return NULL;
}

void CMachineDataOutput::Write(void *pbuf, int const numbytes)
{
	CMachine *mac = theApp.pCB->GetMachine((char *)pbuf);
	char txt[256];
	sprintf(txt,"%s %X @ %.3f %.3f",(char *)pbuf,mac,mac->posx,mac->posy);
	MessageBox(NULL,txt,"",MB_OK);
}


LRESULT CALLBACK HookFn(
    int nCode,	// hook code
    WPARAM wParam,	// current-process flag
    LPARAM lParam	// address of structure with message data
   )
{
//	OutputDebugString("hookfn");
	if (nCode < 0) return CallNextHookEx(theApp.hHook, nCode, wParam, lParam);
	else
	{
		MSG *cwprs = (MSG *)lParam;

		if (nCode == HC_ACTION && cwprs->hwnd == theApp.hBuzz && wParam == PM_REMOVE)
		{
			if (cwprs->message == WM_COMMAND && LOWORD(cwprs->wParam) == 0xB7D1)
			{
				mb("Calling openpalwindow");
				theApp.OpenPALWindow();
			}
		}

		CallNextHookEx(theApp.hHook, nCode, wParam, lParam);
		return 0;
	}

}

void CBTDSysPALApp::OpenPALWindow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	mb("Hooky\n");
	try
	{
		mb("Getting mac\n");
		CMachine *mac = *(CMachine **)0x4E0EB4;
		if (mac)
		{
			mb("Getting mi\n");
			CMachineInterface *mi = mac->pmi;
			mb("Getting pCB\n");
			theApp.pCB = mi->pCB;
			mb("Wikid\n");

			if (theApp.ppw)
			{
				theApp.ppw->UpdateRecs();
				theApp.ppw->SetActiveWindow();
			}
			else
			{
				theApp.ppw = new CPalWindow;
				theApp.ppw->app = &theApp;
				if (theApp.ppw->Create(IDD_PALWINDOW))
				{
					mb("Created window");
					theApp.ppw->ShowWindow(SW_SHOW);
					mb("Showed window");
					theApp.ppw->UpdateRecs();
					mb("Updated window");
				}
			}
		}
		else
		{	
			MessageBox(NULL,"An error occurred.\n"
				"Possible solution: please load at least one machine before "
				"clicking this menu item.","BTDSys PAL",MB_OK);
		}

	}
	catch (...)
	{
		MessageBox(NULL,"An error occurred.\n"
			"Possible solution: please load at least one machine before "
			"clicking this menu item.","BTDSys PAL",MB_OK);
	}

}

BOOL CBTDSysPALApp::InitInstance() 
{
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

	WIN32_FIND_DATA fd;

	char fff[256];
	strcpy(fff,buzz_path);
	strcat(fff,"Gear\\BTDSys PAL\\*.pll");

	HANDLE hFind = FindFirstFile(fff,&fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			OutputDebugString("howow");
			HMODULE hm = NULL;
			
			try
			{
				char fname[256];
				strcpy(fname,"Gear\\BTDSys PAL\\");
				strcat(fname,fd.cFileName);
				OutputDebugString(fname);
				hm = LoadLibrary(fname);
				if (!hm) {OutputDebugString("no hm"); continue;}
				PFGetSupportedMacs pf = (PFGetSupportedMacs)GetProcAddress(hm,"GetSupportedMacs");
				if (!pf) {OutputDebugString("no pf"); continue;}
				char **p = pf();
				for (char **q = p; *q; q++)
				{
					SupportedMachineRecord r;
					strcpy(r.name, *q);
					strcpy(r.dllname, fname);
					vSupportedMacs.push_back(r);
				}
			}
			catch (...)
			{
			}

			if (hm) FreeLibrary(hm);
		}
		while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}

	hBuzz = FindBUZZhwnd();
	HMENU hMenu = GetMenu(hBuzz);
	HMENU hViewMenu = GetSubMenu(hMenu,2);

	AppendMenu(hViewMenu,MF_SEPARATOR,0,0);
	AppendMenu(hViewMenu,MF_STRING,0xB7D1,"Peer &Assignments...");

	hHook = SetWindowsHookEx(WH_GETMESSAGE, &HookFn, NULL, GetCurrentThreadId());

	return CWinApp::InitInstance();
}

int CBTDSysPALApp::ExitInstance() 
{
	UnhookWindowsHookEx(hHook);
	
	return CWinApp::ExitInstance();
}
