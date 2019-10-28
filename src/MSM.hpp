// INCLUDES
#include "rack.hpp"
#include "app.hpp"
#include "asset.hpp"

#include "dsp/digital.hpp"
#include "dsp/functions.hpp"
#include "dsp/resampler.hpp"
#include "dsp/filter.hpp"
#include "dsp/decimator.hpp"
#include "dsp/samplerate.hpp"
#include "dsp/ringbuffer.hpp"
#include "dsp/frame.hpp"

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Additional/DSP.hpp"
#include "Additional/Linear_Interpolator.hpp"
#include "Additional/samplerate.h"

using namespace rack;

extern Plugin *plugin;

extern Model *modelVCO;
extern Model *modelBVCO;
extern Model *modelExperimentalVCO;
extern Model *modelNoise;

extern Model *modelLFO;
extern Model *modelVCA;
extern Model *modelADSR;
extern Model *modelDelay;
extern Model *modelWaveShaper;
extern Model *modelWavefolder;
extern Model *modelBitcrusher;
extern Model *modelPhaserModule;
extern Model *modelMorpher;
extern Model *modelRingMod;
extern Model *modelRandomSource;
extern Model *modelMult;
extern Model *modelCrazyMult;
extern Model *modelFade;
extern Model *modelSimpleSlider;
extern Model *modelxseq;
extern Model *modelBlankPanel;
