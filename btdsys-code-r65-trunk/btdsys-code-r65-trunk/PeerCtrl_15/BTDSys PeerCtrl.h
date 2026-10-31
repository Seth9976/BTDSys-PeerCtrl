// BTDSys PeerCtrl.h : main header file for the BTDSYS PEERCTRL DLL
//

#if !defined(AFX_BTDSYSPEERCTRL_H__95FD9646_B85A_11D6_BDC3_50584FC1BB0B__INCLUDED_)
#define AFX_BTDSYSPEERCTRL_H__95FD9646_B85A_11D6_BDC3_50584FC1BB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBTDSysPeerCtrlApp
// See BTDSys PeerCtrl.cpp for the implementation of this class
//

class CBTDSysPeerCtrlApp : public CWinApp
{
public:
	CBTDSysPeerCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBTDSysPeerCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBTDSysPeerCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BTDSYSPEERCTRL_H__95FD9646_B85A_11D6_BDC3_50584FC1BB0B__INCLUDED_)
