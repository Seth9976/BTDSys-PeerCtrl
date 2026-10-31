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
#include "BandLimit.h"

CHalfBandFilter::CHalfBandFilter()
{
	reset();
};


CHalfBandFilter::~CHalfBandFilter()
{
};

void CHalfBandFilter::reset()
{
	for (int i=0; i<11; i++)
		xvl[i] = yvl[i] = xvr[i] = yvr[i] = 0.0f;
}

/*
filtertype 	 = 	 Chebyshev
passtype 	= 	Lowpass
ripple 	= 	-.5
order 	= 	8
samplerate 	= 	88200
corner1 	= 	18000
*/

void CHalfBandFilter::process(float *buf, int n, int sampleskip)
{
	int bufstep = sampleskip*2 - 1;
	float nodenormal = 1e-15f;
	switch (quality)
	{
	case 1:
		for (; n>0; n--)
		{
			xvl[0] = xvl[1]; xvl[1] = xvl[2]; xvl[2] = xvl[3]; xvl[3] = xvl[4]; 
			xvl[4] = (*buf + nodenormal) / 2.885947405e+01f;
			yvl[0] = yvl[1]; yvl[1] = yvl[2]; yvl[2] = yvl[3]; yvl[3] = yvl[4]; 
			yvl[4] =   (xvl[0] + xvl[4]) + 4.f * (xvl[1] + xvl[3]) + 6.f * xvl[2]
						 + ( -0.2221620437f * yvl[0]) + (  0.7646030415f * yvl[1])
						 + ( -1.4122919932f * yvl[2]) + (  1.3154403373f * yvl[3]);
			*buf++ = yvl[4];

			xvr[0] = xvr[1]; xvr[1] = xvr[2]; xvr[2] = xvr[3]; xvr[3] = xvr[4]; 
			xvr[4] = (*buf + nodenormal) / 2.885947405e+01f;
			yvr[0] = yvr[1]; yvr[1] = yvr[2]; yvr[2] = yvr[3]; yvr[3] = yvr[4]; 
			yvr[4] =   (xvr[0] + xvr[4]) + 4.f * (xvr[1] + xvr[3]) + 6.f * xvr[2]
						 + ( -0.2221620437f * yvr[0]) + (  0.7646030415f * yvr[1])
						 + ( -1.4122919932f * yvr[2]) + (  1.3154403373f * yvr[3]);
			*buf = yvr[4];

			buf += bufstep;
			nodenormal = -nodenormal;
		}
		break;

	case 2:
		for (; n>0; n--)
		{
			xvl[0] = xvl[1]; xvl[1] = xvl[2]; xvl[2] = xvl[3]; xvl[3] = xvl[4]; xvl[4] = xvl[5]; xvl[5] = xvl[6]; 
			xvl[6] = (*buf + nodenormal) / 2.574457982e+02f;
			yvl[0] = yvl[1]; yvl[1] = yvl[2]; yvl[2] = yvl[3]; yvl[3] = yvl[4]; yvl[4] = yvl[5]; yvl[5] = yvl[6]; 
			yvl[6] =   (xvl[0] + xvl[6]) + 6.f * (xvl[1] + xvl[5]) + 15.f * (xvl[2] + xvl[4])
						 + 20.f * xvl[3]
						 + ( -0.2285624836f * yvl[0]) + (  1.0755014942f * yvl[1])
						 + ( -2.5686752329f * yvl[2]) + (  3.8636198500f * yvl[3])
						 + ( -3.9177027529f * yvl[4]) + (  2.5272231083f * yvl[5]);
			*buf++ = yvl[6];

			xvr[0] = xvr[1]; xvr[1] = xvr[2]; xvr[2] = xvr[3]; xvr[3] = xvr[4]; xvr[4] = xvr[5]; xvr[5] = xvr[6]; 
			xvr[6] = (*buf + nodenormal) / 2.574457982e+02f;
			yvr[0] = yvr[1]; yvr[1] = yvr[2]; yvr[2] = yvr[3]; yvr[3] = yvr[4]; yvr[4] = yvr[5]; yvr[5] = yvr[6]; 
			yvr[6] =   (xvr[0] + xvr[6]) + 6.f * (xvr[1] + xvr[5]) + 15.f * (xvr[2] + xvr[4])
						 + 20.f * xvr[3]
						 + ( -0.2285624836f * yvr[0]) + (  1.0755014942f * yvr[1])
						 + ( -2.5686752329f * yvr[2]) + (  3.8636198500f * yvr[3])
						 + ( -3.9177027529f * yvr[4]) + (  2.5272231083f * yvr[5]);
			*buf = yvr[6];

			buf += bufstep;
			nodenormal = -nodenormal;
		}
		break;

	case 3:
	default:
		for (; n>0; n--)
		{
			// left
			xvl[0] = xvl[1]; xvl[1] = xvl[2]; xvl[2] = xvl[3]; xvl[3] = xvl[4]; xvl[4] = xvl[5]; xvl[5] = xvl[6]; xvl[6] = xvl[7]; xvl[7] = xvl[8]; 
			xvl[8] = (*buf + nodenormal) / 2.321332450e+03f;
			yvl[0] = yvl[1]; yvl[1] = yvl[2]; yvl[2] = yvl[3]; yvl[3] = yvl[4]; yvl[4] = yvl[5]; yvl[5] = yvl[6]; yvl[6] = yvl[7]; yvl[7] = yvl[8]; 
			yvl[8] =   (xvl[0] + xvl[8]) + 8.f * (xvl[1] + xvl[7]) + 28.f * (xvl[2] + xvl[6])
						 + 56.f * (xvl[3] + xvl[5]) + 70.f * xvl[4]
						 + ( -0.2313146139f * yvl[0]) + (  1.3669622502f * yvl[1])
						 + ( -4.1067748090f * yvl[2]) + (  8.0132345890f * yvl[3])
						 + (-11.0399764930f * yvl[4]) + ( 11.0112785220f * yvl[5])
						 + ( -7.8593704622f * yvl[6]) + (  3.7356795276f * yvl[7]);
			*buf++ = yvl[8];
			
			// right
			xvr[0] = xvr[1]; xvr[1] = xvr[2]; xvr[2] = xvr[3]; xvr[3] = xvr[4]; xvr[4] = xvr[5]; xvr[5] = xvr[6]; xvr[6] = xvr[7]; xvr[7] = xvr[8]; 
			xvr[8] = (*buf + nodenormal) / 2.321332450e+03f;
			yvr[0] = yvr[1]; yvr[1] = yvr[2]; yvr[2] = yvr[3]; yvr[3] = yvr[4]; yvr[4] = yvr[5]; yvr[5] = yvr[6]; yvr[6] = yvr[7]; yvr[7] = yvr[8]; 
			yvr[8] =   (xvr[0] + xvr[8]) + 8.f * (xvr[1] + xvr[7]) + 28.f * (xvr[2] + xvr[6])
						 + 56.f * (xvr[3] + xvr[5]) + 70.f * xvr[4]
						 + ( -0.2313146139f * yvr[0]) + (  1.3669622502f * yvr[1])
						 + ( -4.1067748090f * yvr[2]) + (  8.0132345890f * yvr[3])
						 + (-11.0399764930f * yvr[4]) + ( 11.0112785220f * yvr[5])
						 + ( -7.8593704622f * yvr[6]) + (  3.7356795276f * yvr[7]);
			*buf = yvr[8];

			buf += bufstep;
			nodenormal = -nodenormal;
		}
		break;
	}
}
