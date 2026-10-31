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

//Code for "hacking" CMachine in Buzz machines
//Ed Powley (BTDSys) July 2002
//e@btd2001.freeserve.co.uk

#ifndef _BTDSYS_CMACHINE_HACK_
#define _BTDSYS_CMACHINE_HACK_

#include "../mdk/mdk.h"

bool FindMIOffset (CMachine *Machine, CMachineInterface *MachineInterface);

CMachineInterface *GetMI (CMachine *Machine);

long GetParamOffset (const CMachineInfo *minfo, 
					 byte Group, byte Track, byte ParaNum);

byte *GetParamByte(const CMachineInterface *mint,
				   const CMachineInfo *minfo,
				   byte Group, byte Track, byte ParaNum);

word *GetParamWord(const CMachineInterface *mint,
				   const CMachineInfo *minfo,
				   byte Group, byte Track, byte ParaNum);

void Hack_ControlChange (const CMachineInfo *minfo, 
						 CMachine *pmac, 
						 int group, int track, int param, int value);

void Hack_ControlsClear(const CMachineInfo *minfo, 
						CMachine *pmac);

int Hack_GetNumTracks(const CMachine *mac);

#endif
