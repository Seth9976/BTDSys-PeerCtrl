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

#include <windows.h>
#include "../../mdk/machineinterface.h"

#ifdef B04
	const char g_ConfigString[] = "b\x04  [sgrid] g_ConfigString patchme";
								// 12   34567890123456789012345678901234567890
#endif
#ifdef B08
	const char g_ConfigString[] = "b\x08  [sgrid] g_ConfigString patchme";
#endif
#ifdef B16
	const char g_ConfigString[] = "b\x10  [sgrid] g_ConfigString patchme";
#endif
#ifdef B32
	const char g_ConfigString[] = "b\x20  [sgrid] g_ConfigString patchme";
#endif
#ifdef S04
	const char g_ConfigString[] = "s\x04  [sgrid] g_ConfigString patchme";
#endif
#ifdef S08
	const char g_ConfigString[] = "s\x08  [sgrid] g_ConfigString patchme";
#endif
#ifdef S16
	const char g_ConfigString[] = "s\x10  [sgrid] g_ConfigString patchme";
#endif
#ifdef S32
	const char g_ConfigString[] = "s\x20  [sgrid] g_ConfigString patchme";
#endif


HINSTANCE hinst = 0;

typedef CMachineInterface * (__cdecl *PFCreateMachine)(int n, char type);
typedef void (__cdecl *PFGetInfo)(int n, char type, CMachineParameter **pParameters, CMachineInfo *minfo);

HINSTANCE LoadCore()
{
	if (hinst) return hinst;

	hinst = LoadLibrary("Gear\\BTDSys SampleGrid 2 Core BETA 050911.dat");

	if (!hinst)
		MessageBox(NULL,"FATAL ERROR: 'Gear\\BTDSys SampleGrid 2 Core.dat' could not be loaded",NULL,MB_OK);

	return hinst;
}

BOOL WINAPI DllMain ( HANDLE hModule, DWORD fwdreason, LPVOID lpReserved )
//GUI code from Cyanphase's "small" guide to making Buzz effects
{
	switch (fwdreason) {
	case DLL_PROCESS_ATTACH:
		LoadCore();
		break;

	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;

	case DLL_PROCESS_DETACH:
		if (hinst) FreeLibrary(hinst);
		break;
	}

	return TRUE;
}

extern "C" {
	__declspec(dllexport) CMachineInfo const * __cdecl GetInfo()
	{
		static bool HaveInfo = false;
		static CMachineInfo macinfo;
		static CMachineParameter paras[100];
		static CMachineParameter *pparas[100];

		if (HaveInfo) return &macinfo;

		for (int i=0; i<100; i++) pparas[i] = &paras[i];

		if (hinst)
		{
			PFGetInfo f = (PFGetInfo)GetProcAddress(hinst,"GetInfo");
			if (f)
			{
				f(g_ConfigString[1],g_ConfigString[0],pparas,&macinfo);
				static char txt[256];
				sprintf(txt,"BTDSys SampleGrid 2 - %c%i", g_ConfigString[0] - 'a' + 'A', g_ConfigString[1]);
				macinfo.Name = txt;
				HaveInfo = true;
			}
		}
		return &macinfo;
	}

	__declspec(dllexport) CMachineInterface * __cdecl CreateMachine()
	{
		if (hinst)
		{
			PFCreateMachine f = (PFCreateMachine)GetProcAddress(hinst,"CreateMachine");
		
			if (f)
				return f(g_ConfigString[1],g_ConfigString[0]);
		}

		return NULL;
	}
} 
