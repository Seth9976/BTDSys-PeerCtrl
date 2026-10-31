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
#include "transparency.h"

typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA pSetLayeredWindowAttributes = NULL;
static BOOL initialized = FALSE;

#define WS_EX_LAYERED           0x00080000    

#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

BOOL MakeWindowTransparent(HWND hWnd, unsigned char factor, bool transp)
{
   /* First, see if we can get the API call we need. If we've tried
    * once, we don't need to try again. */
   if (!initialized)
   {
      HMODULE hDLL = LoadLibrary ("user32");

      pSetLayeredWindowAttributes = 
         (PSLWA) GetProcAddress(hDLL, "SetLayeredWindowAttributes");

      initialized = TRUE;
   }

   if (pSetLayeredWindowAttributes == NULL) 
      return FALSE;

   /* Windows need to be layered to be made transparent. This is done
    * by modifying the extended style bits to contain WS_EX_LAYARED. */
   SetLastError(0);

   if (transp)
	   SetWindowLong(hWnd, 
					 GWL_EXSTYLE , 
					 GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
   else
	   SetWindowLong(hWnd, 
					 GWL_EXSTYLE , 
					 GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

   if (GetLastError())
      return FALSE;

   /* Now, we need to set the 'layered window attributes'. This
    * is where the alpha values get set. */
   if (transp)
	   return pSetLayeredWindowAttributes (hWnd, 
										   RGB(255,0,255), 
										   factor,
										   LWA_COLORKEY|LWA_ALPHA);
   else
	   return TRUE;
}
