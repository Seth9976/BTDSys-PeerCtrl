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
#include <math.h>

#include "pulsar.h"
#include "iwaves.h"

char const* mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	switch (param)
	{
	case numInertia:
		if (value)
		{
			sprintf(txt,"%i %i/16 tick", value / 16, value % 16);
			return txt;
		}
		else
			return "Off";
		break;

	case numOversample:
		if (value == 0)
			return "Off";
		else
		{
			sprintf(txt,"%ix",1<<value);
			return txt;
		}
		break;

	case numDCRemoval:
		if (value == SWITCH_ON)
			return "On";
		else
			return "Off";
		break;

	case numOverlapping:
		if (value == 1)
			return "1 (off)";
		else
		{
			sprintf(txt,"%i (on)",value);
			return txt;
		}
		break;

	case numFreqSliderMode:
		if (value == 0)
			return "Linear";
		else
			return "Logarithmic";
		break;

	case numMinPFreq:
	case numMinDFreq:
		sprintf(txt,"%.3fHz",value / 1000.0f);
		return txt;

	case numFreqP:
	case numFreqD:
		{
			float f;
			float c = (param == numFreqP) ? fLogPFreqC : fLogDFreqC;
			if (nFreqSliderMode == 0)
				f = value * ((param == numFreqP) ? fLinPFreqMult : fLinDFreqMult);
			else
				f = powf(2.0f,value / 2048.0f + c);

			char *fmt;
			if		(f >= 10000.0f)	fmt = "%.1fHz";
			else if (f >=  1000.0f) fmt = "%.2fHz";
			else if (f >=   100.0f) fmt = "%.3fHz";
			else if (f >=    10.0f) fmt = "%.4fHz";
			else if (f >=     1.0f) fmt = "%.5fHz";
			else					fmt = "%.6fHz";

			sprintf(txt,fmt,f);
			return txt;
		}
		break;

	case numPeriodP:
		sprintf(txt,"%i %i/16 tick", value / 16, value % 16);
		return txt;
		break;

	case numPParam:
		switch (value)
		{
		case 0: return "Freq";
		case 1: return "Period";
		case 2: return "Note";
		case 3: return "Slave";
		}
		break;

	case numOnOff:
	case numEnvNorm:
		if (value == SWITCH_ON)
			return "On";
		else
			return "Off";
		break;

#	ifndef PULSARFX
		case numWave:
#	endif
	case numLFO1WTWave:
	case numLFO2WTWave:
	case numEnvWTWave:
		if (value <= 200)
		{
			sprintf(txt, "%i..%s", value, pCB->GetWaveName(value));
			return txt;
		}
#		ifndef PULSARFX
			else if (value <= 206)
			{
				switch (value)
				{
				case 201: return "Sine";
				case 202: return "Sawtooth";
				case 203: return "Pulse";
				case 204: return "Triangle";
				case 205: return "Noise";
				case 206: return "303 Sawtooth";
				}
			}
			else if (value < 207 + NUM_INTERNAL_WAVES)
			{
				return g_pWaveName[value - 207];
			}
#		endif
		else
			return "Not impl.";

		break;

	case numWStart:
	case numWEnd:
		sprintf(txt,"%.3f%%", value/655.34f);
		return txt;
		break;

	case numVolume:
	case numStochMask:
	case numRandomPan:
		sprintf(txt,"%.3f%%", value/327.68f);
		return txt;
		break;

	case numPan:
		if (value == 0x80) return "Centre";
		else
		{
			sprintf(txt,"%s %.2f%%", value<0x80 ? "L" : "R", fabsf((float)value - 128.0f) / 1.28f);
			return txt;
		}
		break;

	case numEnvelope:
		switch (value)
		{
		case 0: return "Rectangular";
		case 1: return "Gaussian";
		case 2: return "Lin decay";
		case 3: return "Exp decay";
		case 4: return "Lin attack";
		case 5: return "Exp attack";
		case 6: return "Wavetable";
		default: return "Not impl.";
		}
		break;

	case numEnvParam:
		sprintf(txt,"%.3f",value / 128.0f);
		return txt;
		break;
		
	case numLFO1Period:
	case numLFO2Period:
		sprintf(txt,"%i %i/16 tick", value / 16, value % 16);
		return txt;
		break;

	case numLFO1Shape:
	case numLFO2Shape:
		switch (value)
		{
		case 0: return "Sine";
		case 1: return "Square";
		case 2: return "Tri";
		case 3: return "Ramp Up";
		case 4: return "Ramp Dn";
		case 5: return "Random";
		case 6: return "Lin Wander";
		case 7: return "Cos Wander";
		case 8: return "Wavetable";
		default: return "Not impl.";
		}
		break;

	case numLFO1Target:
	case numLFO2Target:
		switch (value)
		{
		case lfoOff:		return "Off";
		case lfoFreqP:		return "P freq";
		case lfoFreqD:		return "D freq";
		case lfoWStart:		return "Start";
		case lfoWEnd:		return "End";
		case lfoStochMask:	return "Stoch Mask";
		case lfoRandomPan:	return "Random Pan";
		case lfoVolume:		return "Volume";
		case lfoPan:		return "Pan";
		case lfoEnvParam:	return "Env Param";
		case lfoBurst:		return "Burst";
		case lfoRest:		return "Rest";
		case lfoOverlap:	return "Overlap";
		case lfoLFO1Period:	return "LFO1 Period";
		case lfoLFO1Amount:	return "LFO1 Amount";
		default: return "Not impl.";
		}
		break;

	case numLFO1TargetTrack:
	case numLFO2TargetTrack:
		if (value == 254) return "All";
		else if (value >= numTracks) return "No";
		else
		{
			sprintf(txt,"%i",value);
			return txt;
		}
		break;

	case numLFO1Amount:
	case numLFO2Amount:
		{
			float f;
			if (!bLFOAmtExpInvert)
				f = powf(value / 32768.0f, fLFOAmtExp);
			else
				f = 1.0f - powf(1.0f - value / 32768.0f, fLFOAmtExp);

			sprintf(txt,"±%.3f%%",f * 100.0f);
			return txt;
		}
		break;

	case numSeparator1:
	case numSeparator2:
	case numSeparator3:
		return "----------0";
		break;

	case numTrackSep:
		if (bShowGraphicalSeparator)
		{
#			ifdef PULSARFX
				return "pulsarfx_tracksep";
#			else
				return "pulsar_tracksep";
#			endif
		}
		else
			return "----------0";

		break;
	}

	return NULL;
}

char const *miex::DescribeParam(const int param)
{
	switch (param)
	{
	case numTrackSep:
	case numSeparator1:
	case numSeparator2:
	case numSeparator3:
		return "----------";
	}

	return NULL;
}
