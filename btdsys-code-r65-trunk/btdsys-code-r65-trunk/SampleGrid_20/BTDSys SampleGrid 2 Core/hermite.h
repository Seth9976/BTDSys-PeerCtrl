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

#ifndef __BTDSYS_HERMITE_H
#define __BTDSYS_HERMITE_H

float hermite(const float frac_pos, const float xm1, const float x0, const float x1, const float x2);
/*
template <typename T> float GetSampleM(const float Pos, const T *pSamples, const int numSamples);
template <typename T> float GetSampleL(const float Pos, const T *pSamples, const int numSamples);
template <typename T> float GetSampleR(const float Pos, const T *pSamples, const int numSamples);
*/

//////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline float GetSampleM(const float Pos, T *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;

	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos-1],(float)pSamples[iPos],
						(float)pSamples[iPos+1],(float)pSamples[iPos+2]);
	
	else if (iPos == 0)
		return hermite(frac,
						0.0f,(float)pSamples[0],
						(float)pSamples[1],(float)pSamples[2]);
	
	else if (iPos == numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos-1],(float)pSamples[iPos],
						(float)pSamples[iPos+1],0.0f);
	
	else if (iPos == numSamples-1)
		return hermite(frac,
						(float)pSamples[iPos-1],(float)pSamples[iPos],
						0.0f,0.0f);
	
	else
		return 0.0f;
}

template <typename T>
inline float GetSampleL(const float Pos, T *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;

	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos2-2],(float)pSamples[iPos2],
						(float)pSamples[iPos2+2],(float)pSamples[iPos2+4]);

	else if (iPos == 0)
		return hermite(frac,
						0.0f,(float)pSamples[0],
						(float)pSamples[2],(float)pSamples[4]);

	else if (iPos == numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos2-2],(float)pSamples[iPos2],
						(float)pSamples[iPos2+2],0.0f);

	else if (iPos == numSamples-1)
		return hermite(frac,
						(float)pSamples[iPos2-2],(float)pSamples[iPos2],
						0.0f,0.0f);

	else
		return 0.0f;
}

template <typename T>
inline float GetSampleR(const float Pos, T *pSamples, const int numSamples)
{
	int iPos = (int)Pos;
	float frac = Pos - iPos;
	int iPos2 = iPos * 2;

	if (iPos > 0 && iPos < numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos2-1],(float)pSamples[iPos2+1],
						(float)pSamples[iPos2+3],(float)pSamples[iPos2+5]);
	
	else if (iPos == 0)
		return hermite(frac,
						0.0f,(float)pSamples[1],
						(float)pSamples[3],(float)pSamples[5]);
	
	else if (iPos == numSamples-2)
		return hermite(frac,
						(float)pSamples[iPos2-1],(float)pSamples[iPos2+1],
						(float)pSamples[iPos2+3],0.0f);
	
	else if (iPos == numSamples-1)
		return hermite(frac,
						(float)pSamples[iPos2-1],(float)pSamples[iPos2+1],
						0.0f,0.0f);
	
	else
		return 0.0f;
}

#endif
