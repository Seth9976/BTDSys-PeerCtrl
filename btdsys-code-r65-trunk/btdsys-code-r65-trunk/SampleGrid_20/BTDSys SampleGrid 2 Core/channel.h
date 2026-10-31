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

#ifndef __BTDSYS_SGRID_CHANNEL_H
#define __BTDSYS_SGRID_CHANNEL_H

#include "../../mdk/mdk.h"
#include "drumkit.h"
#include "fuzzpilz_envelope.h"

class mi;
class CTrack;
class CTriggerInfo;
struct STrigInfo;

class CChannel
{
public:
	CChannel();
	~CChannel();

	void TriggerOn(	int sample, STrigInfo ti, int ramp);

	void TriggerOn(	CDrumkitSample *pPSISamp, STrigInfo ti, int ramp);

	void TriggerOn(	CDrumkitLayer *pLayer, STrigInfo ti, int ramp);

	void TriggerOff(int RampLen);
	void TriggerOffHard();

	bool Work(float *psamples, int numsamples, int samplerate);

	inline bool IsActive() {return bActive; };
	inline int GetAge() {return nAge; };

	mi *pmi;
	CMICallbacks *pCB;
	CTrack *ptr;
	int tag;

	CEnvelope envVol, envPan, envPos;
	bool bForceVolEnv, bForcePanEnv, bUsePosEnv;

    bool bSleepy;

private:
	bool bActive;
	int nAge;
	int nCut;

	int nWave;
//	CDrumkitSample *pSamp;
	CDrumkitLayer *pLyr;
	bool bKitMode;
	double fPos;
	
	float fVel;
	float fPan;
	float fPosStep;

	int nRampingOn, nRampingOff;
	float fRampVol;
	float fRampVolStep;

	int nPeerValue;
	int nPeerCountDown;
};










#endif
