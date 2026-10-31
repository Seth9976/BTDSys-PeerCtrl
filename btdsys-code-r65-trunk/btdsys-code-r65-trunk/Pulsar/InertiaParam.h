/*
Copyright (c) 2004-2006, Edward Powley
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

#ifndef __BTDSYS_INERTIAPARAM_H
#define __BTDSYS_INERTIAPARAM_H

#include <vector>

#define PI	3.1415926535897932384626433832795f

enum InertiaType
{
	etLinear,
	etRecip
};

class CMICallbacks;

class LFO
{
public:
	LFO() : fValue(0.0f), fRandValue(0.0f), fLastRandValue(0.0f) {};

	inline float GetValue() {return fValue;};

	void Update(float numsamples);

	inline void SetPeriod(float f) {fStep = 2.0f*PI / f;}

	int nWaveType;
	int nWTWave;
	float fAmount;

	inline void Reset() {fPhase = 0.0f;}

	CMICallbacks *pCB;

private:
	float CalcValue();

	float fPhase;
	float fStep;
	float fValue;

	float fRandValue;
	float fLastRandValue;
};

class InertiaParam
{
public:
	InertiaParam() : bSliding(false), fCurrent(0.0f), pSlaveTo(NULL) {};

	inline void SetMinMax(float f1, float f2) {fMin = f1; fMax = f2;}

	float GetValue();

	inline float GetTargetValue() {return fTarget;};

	void SetValue(float f, int len, InertiaType type);

	inline void SetValueNoInertia(float f)
	{
		fTarget = fCurrent = f;
		bSliding = false;
	}

	void Update(float numsamples);

	void AttachLFO(LFO *p);
	void DetachLFO(LFO *p);

	void SlaveTo(InertiaParam *p);
	void Unslave();

private:
	float fMin, fMax;
	float fCurrent, fStep, fTarget;
	bool bSliding;
	float fCountdown;
	InertiaType nType;
	std::vector<LFO *> vpLFO;
	InertiaParam *pSlaveTo;
};









#endif