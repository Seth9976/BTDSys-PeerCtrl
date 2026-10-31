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

#ifndef __BTDSYS_SGCORE_TYPEDEFS_H
#define __BTDSYS_SGCORE_TYPEDEFS_H

typedef void (__cdecl *PFSGridCallback)(void *pmi, int opcode, void *data[4]);

enum
{						//data[0]			data[1]			data[2]			data[3]
	opGetNumTracks,		//o *num tracks
	opPreviewLayer,		//i *layer			i track num
	opPreviewSample,	//i *sample			i track num
	opEstimateKitSize,	//i save mode		o *bytes
	opGetLayerInfo,		//o *info txt		i *layer
	opGetSampleInfo,	//o *info txt		i *sample
};

typedef void (__cdecl *PFKitInfoDialog)(void *, void *);
typedef int  (__cdecl *PFGetSGCoreVersion)();
typedef void (__cdecl *PFReg)(void *, PFSGridCallback);
typedef void (__cdecl *PFUnreg)(void *);
typedef int  (__cdecl *PFKitSaveModeDialog)(void *, int);




#endif
