#include "MSM.hpp"

Plugin *plugin;


void init(rack::Plugin *p) {
	plugin = p;		
	p->slug = TOSTRING(SLUG);

	p->version = TOSTRING(VERSION);

	p->addModel(modelVCO);
	p->addModel(modelBVCO);
	p->addModel(modelExperimentalVCO);
	p->addModel(modelNoise);

	p->addModel(modelLFO);
	p->addModel(modelVCA);
	p->addModel(modelADSR);
	p->addModel(modelDelay);
	p->addModel(modelWaveShaper);
	p->addModel(modelWavefolder);
	p->addModel(modelBitcrusher);
	p->addModel(modelPhaserModule);
	p->addModel(modelMorpher);
	p->addModel(modelRingMod);
	p->addModel(modelRandomSource);
	p->addModel(modelMult);
	p->addModel(modelCrazyMult);
	p->addModel(modelFade);
	p->addModel(modelSimpleSlider);
	p->addModel(modelxseq);
	p->addModel(modelBlankPanel);
	
};