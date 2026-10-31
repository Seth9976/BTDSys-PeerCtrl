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

#include "stdafx.h"

#include "../PCtrlPlug.h"

#include "Test2.h"
#include "TestDlg.h"

SPlugInfo *CTestPlug2::GetInfo()
{
	static SPlugInfo info =
	{
		PCTRL_VERSION,								//Version
		"Test Plug 2",								//Name
		"Ed Powley (BTDSys)",						//Author
		"This is just a test plugin.\n"				//Description
			"It does nothing of any consequence.",
		"&Open...",									//Menus
		0,											//Flags
	};

	return &info;
}

bool CTestPlug2::Init()
{
	pCB->AddAxis("Slider A");
	pCB->AddAxis("Slider B");

	return true;
}

void CTestPlug2::MenuCommand(int nCommand)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pdlg)
		pdlg->SetActiveWindow();
	else
		pdlg = new CTestDlg(this);
}

void CTestPlug2::ReadFileData(CPCtrlDataRead *dr)
{
}

void CTestPlug2::WriteFileData(CPCtrlDataWrite *dw)
{
}

void CTestPlug2::Timer(int numsamples)
{


}

void CTestPlug2::ValueChanged(int nAxis, float fValue)
{
	char txt[256];
	sprintf(txt,"ValueChanged(%i, %.4f)", nAxis, fValue);
	OutputDebugString(txt);

	if (pdlg)
	{
		switch (nAxis)
		{
		case 0:
			pdlg->m_ScrollBarA.SetPos((int)(fValue * 1000.0f));
			break;
		case 1:
			pdlg->m_ScrollBarB.SetPos((int)(fValue * 1000.0f));
			break;
		}
	}
}

DLL_EXPORT(CTestPlug2);
