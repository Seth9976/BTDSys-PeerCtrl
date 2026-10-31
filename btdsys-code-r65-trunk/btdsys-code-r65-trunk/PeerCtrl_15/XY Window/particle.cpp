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

#include "particle.h"


// returns random value between 0 and 1
// from Waveout.cpp
inline float frand()
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

void CParticleStream::Update()
{
	int dx = LastLastPoint.x - LastPoint.x;
	int dy = LastLastPoint.y - LastPoint.y;

	for (int i=0; i<64; i++)
	{
		Particles[i].strength --;
		Particles[i].x += Particles[i].dx;
		Particles[i].y += Particles[i].dy;
	}

	for (int p=1; p<=4; p++)
	{
		Particles[NextParticle].x = (float)LastLastPoint.x + (dx*p * 0.25f);
		Particles[NextParticle].y = (float)LastLastPoint.y + (dy*p * 0.25f);
		Particles[NextParticle].dx = (dx * 0.5f + frand() - 0.5f);
		Particles[NextParticle].dy = (dy * 0.5f + frand() - 0.5f);
		Particles[NextParticle].strength = 15;

		NextParticle++;
		if (NextParticle > 63) NextParticle = 0;
	}

	LastLastPoint = LastPoint;
}
