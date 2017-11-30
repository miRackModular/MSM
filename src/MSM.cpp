#include "MSM.hpp"

Plugin *plugin;


void init(rack::Plugin *p) {
	plugin = p;		
	p->slug = "MSM";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://github.com/Phal-anx/MS-Modules";
	
	p->addModel(createModel<RandomSourceV1Widget>("MSM", "Random SourceV1", "Random Source-1", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<RandomSourceV2Widget>("MSM", "Random SourceV2", "Random Source-2", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<MultWidget>("MSM", "Mult", "Mult", MULTIPLE_TAG));
	p->addModel(createModel<SwitchedMultWidget>("MSM", "SwitchedMult", "SwitchedMult", MULTIPLE_TAG));
	p->addModel(createModel<BlankPanelWidget>("MSM", "Blank Panel", "Blank Panel", BLANK_TAG));
		
}
