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


#include "stdafx.h"
#include "hermite.h"
/*
inline float hermite(float x, float y0, float y1, float y2, float y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    float c2 = y0 - y1 + c1 - c3;

    return ((c3 * x + c2) * x + c1) * x + c0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

inline float GetSampleM(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],pSamples[iPos+2]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[1],pSamples[2]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleL(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],pSamples[iPos2+4]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[2],pSamples[4]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleR(const float Pos, const short *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],pSamples[iPos2+5]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[1],pSamples[3],pSamples[5]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],0.0f,0.0f);
	else
		return 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float GetSampleM(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],pSamples[iPos+2]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[1],pSamples[2]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],pSamples[iPos+1],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos-1],pSamples[iPos],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleL(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],pSamples[iPos2+4]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[0],pSamples[2],pSamples[4]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],pSamples[iPos2+2],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-2],pSamples[iPos2],0.0f,0.0f);
	else
		return 0.0f;
}

inline float GetSampleR(const float Pos, const float *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;
	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],pSamples[iPos2+5]);
	else if (iPos == 0)
		return hermite(frac,0.0f,pSamples[1],pSamples[3],pSamples[5]);
	else if (iPos == numSamples-2)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],pSamples[iPos2+3],0.0f);
	else if (iPos == numSamples-1)
		return hermite(frac,pSamples[iPos2-1],pSamples[iPos2+1],0.0f,0.0f);
	else
		return 0.0f;
}
*/