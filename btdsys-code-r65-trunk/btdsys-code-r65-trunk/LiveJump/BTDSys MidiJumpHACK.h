// BTDSys MidiJumpHACK.h : main header file for the BTDSYS MIDIJUMPHACK DLL
//

#if !defined(AFX_BTDSYSMIDIJUMPHACK_H__F0F3E88E_B064_42BC_B170_EA7E400B4D16__INCLUDED_)
#define AFX_BTDSYSMIDIJUMPHACK_H__F0F3E88E_B064_42BC_B170_EA7E400B4D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBTDSysMidiJumpHACKApp
// See BTDSys MidiJumpHACK.cpp for the implementation of this class
//

class CBTDSysMidiJumpHACKApp : public CWinApp
{
public:
	CBTDSysMidiJumpHACKApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBTDSysMidiJumpHACKApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBTDSysMidiJumpHACKApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTDSYSMIDIJUMPHACK_H__F0F3E88E_B064_42BC_B170_EA7E400B4D16__INCLUDED_)
