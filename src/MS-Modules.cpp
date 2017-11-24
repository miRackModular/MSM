#include "MS-Modules.hpp"

Plugin *plugin;


void init(rack::Plugin *p) {
	plugin = p;		
	p->slug = "MS-Modules";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->website = "https://github.com/Phal-anx/MS-Modules";
	
	p->addModel(createModel<RandomSourceV1Widget>("MS-Modules", "Random SourceV1", "Random Source-1", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<RandomSourceV2Widget>("MS-Modules", "Random SourceV2", "Random Source-2", SAMPLE_AND_HOLD_TAG));
	p->addModel(createModel<MultWidget>("MS-Modules", "Mult", "Mult", MULTIPLE_TAG));
		
}
