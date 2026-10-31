// BTDSys SampleGrid 2 Core.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BTDSys SampleGrid 2 Core.h"

#include "overloaderdata.h"

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
// CBTDSysSampleGrid2CoreApp

BEGIN_MESSAGE_MAP(CBTDSysSampleGrid2CoreApp, CWinApp)
	//{{AFX_MSG_MAP(CBTDSysSampleGrid2CoreApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBTDSysSampleGrid2CoreApp construction

CBTDSysSampleGrid2CoreApp::CBTDSysSampleGrid2CoreApp()
{
AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBTDSysSampleGrid2CoreApp object

CBTDSysSampleGrid2CoreApp theApp;

#include <process.h>
#include <vector>
using namespace std;

HINSTANCE dllInstance;
class mi;
vector<mi *> g_vpmi;

bool g_bExitThread;

extern void __cdecl ThreadFunc(void *poo); //thread.cpp

BOOL CBTDSysSampleGrid2CoreApp::InitInstance() 
{
OutputDebugString("sg InitInstance");

	dllInstance = theApp.m_hInstance;

	overloaderdata_initialize();
	overloaderdata_registerimage("--------sg2",m_hInstance,"separator");

	AfxInitRichEdit();

	g_bExitThread = false;
	_beginthread(::ThreadFunc,0,this);

	return CWinApp::InitInstance();
}

int CBTDSysSampleGrid2CoreApp::ExitInstance() 
{
OutputDebugString("sg ExitInstance");

	g_bExitThread = true;
	Sleep(200);

	return CWinApp::ExitInstance();
}
