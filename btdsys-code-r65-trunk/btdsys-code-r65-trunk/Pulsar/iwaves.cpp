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

#include "waves/vela_short.h"
#include "waves/vela_long.h"
#include "waves/sine_multicycle.h"
#include "waves/sinc.h"
#include "waves/B0329_long.h"
#include "waves/B0329_short.h"
#include "waves/dc.h"
#include "waves/antimatter.h"
#include "waves/white_dwarf.h"
#include "waves/black_hole.h"
#include "waves/redshift.h"
#include "waves/quasar.h"
#include "waves/neuron.h"
#include "waves/magnetar.h"
#include "waves/xray.h"
#include "waves/gammaray.h"

#define NUM_INTERNAL_WAVES 16
short *g_pWaveData[] = {ps_vela_short, ps_vela_long, ps_sine_multicycle, ps_sinc, ps_B0329_long, ps_B0329_short, ps_dc, ps_antimatter, ps_white_dwarf, ps_black_hole, ps_redshift, ps_quasar, ps_neuron, ps_magnetar, ps_xray, ps_gammaray, 0};
int g_pWaveLen[] = {NS_vela_short, NS_vela_long, NS_sine_multicycle, NS_sinc, NS_B0329_long, NS_B0329_short, NS_dc, NS_antimatter, NS_white_dwarf, NS_black_hole, NS_redshift, NS_quasar, NS_neuron, NS_magnetar, NS_xray, NS_gammaray, 0};
bool g_bWaveStereo[] = {BS_vela_short, BS_vela_long, BS_sine_multicycle, BS_sinc, BS_B0329_long, BS_B0329_short, BS_dc, BS_antimatter, BS_white_dwarf, BS_black_hole, BS_redshift, BS_quasar, BS_neuron, BS_magnetar, BS_xray, BS_gammaray, false};
char *g_pWaveName[] = {"vela_short", "vela_long", "sine_multicycle", "sinc", "B0329_long", "B0329_short", "dc", "antimatter", "white_dwarf", "black_hole", "redshift", "quasar", "neuron", "magnetar", "xray", "gammaray", ""};
