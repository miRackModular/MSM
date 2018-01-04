#include "MSM.hpp"

Plugin *plugin;


void init(rack::Plugin *p) {
	plugin = p;		
	p->slug = "MSM";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://github.com/Phal-anx/MSM";
	
	p->addModel(createModel<RandomSourceV1Widget>("MSM", "Random Source-1", "Random Source-1", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<RandomSourceV2Widget>("MSM", "Random Source-2", "Random Source-2", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<MultWidget>("MSM", "Mult", "Mult", MULTIPLE_TAG));
	p->addModel(createModel<SwitchedMultWidget>("MSM", "SwitchedMult", "SwitchedMult", MULTIPLE_TAG));
	p->addModel(createModel<FadeWidget>("MSM", "Fade", "Fade", MIXER_TAG));
	p->addModel(createModel<LFOWidget>("MSM", "MLFO", "MLFO", LFO_TAG));
	p->addModel(createModel<BlankPanelWidget>("MSM", "Blank Panel", "Blank Panel", BLANK_TAG));
	p->addModel(createModel<WaveShaperWidget>("MSM", "WaveShaper", "WaveShaper", UTILITY_TAG));

}