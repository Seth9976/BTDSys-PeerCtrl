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
#include "xyplug.h"
#include "prefsdialog.h"

DWORD get_reg_dword(HKEY hk, char *name, DWORD def)
{
	DWORD td,size;
	size = 4;
	if (RegQueryValueEx(hk,name,NULL,NULL,(LPBYTE)&td,&size)==ERROR_SUCCESS)
	{
		return td;
	}
	else
	{
		RegSetValueEx(hk,name,NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&def,size);
		return def;
	}
}



void CXYPlug::LoadRegistrySettings()
{
	HKEY hk;
/*	if (RegCreateKeyEx(
		HKEY_CURRENT_USER,
		"Software\\BTDSys\\XY Window for PeerCtrl",
		0, //reserved
		"", //class
		REG_OPTION_NON_VOLATILE, //options
		KEY_QUERY_VALUE | KEY_WRITE, //access
		0, //security (ignored)
		&hk, //result
		&dwDisposition // REG_CREATED_NEW_KEY or REG_OPENED_EXISTING_KEY
		) == ERROR_SUCCESS)
*/
	if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\XY Window for PeerCtrl",&hk) == ERROR_SUCCESS)
	{
		colL = get_reg_dword(hk,"colL",RGB(255,0,0));
		colM = get_reg_dword(hk,"colM",RGB(0,255,0));
		colR = get_reg_dword(hk,"colR",RGB(0,0,255));
		colBG= get_reg_dword(hk,"colBG",RGB(0,0,0));

		nBlobStyle = (char)get_reg_dword(hk,"nBlobStyle",1);

		RegCloseKey(hk);	
	}
	else
	{
		colL = RGB(255,0,0);
		colM = RGB(0,255,0);
		colR = RGB(0,0,255);
		colBG= RGB(0,0,0);
		nBlobStyle = 1;
	}
}


void CPrefsDialog::OnSavedefbtn() 
{
	// TODO: Add your control notification handler code here
	HKEY hk;

	if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\XY Window for PeerCtrl",&hk) == ERROR_SUCCESS)
	{
		RegSetValueEx(hk,"colL" ,NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&m_LColour.fgcolour,4);
		RegSetValueEx(hk,"colM" ,NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&m_MColour.fgcolour,4);
		RegSetValueEx(hk,"colR" ,NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&m_RColour.fgcolour,4);
		RegSetValueEx(hk,"colBG",NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&m_LColour.bgcolour,4);

		DWORD b = m_BlobStyle.GetCurSel();
		RegSetValueEx(hk,"nBlobStyle",NULL,REG_DWORD_LITTLE_ENDIAN,(LPBYTE)&b,4);

		RegCloseKey(hk);
		
		MessageBox("Settings saved to registry","Okalee dokalee",MB_OK|MB_ICONINFORMATION);
	}
}
