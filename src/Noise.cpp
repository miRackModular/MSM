#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"
#include "Additional/VRand.hpp"

struct Noise : Module {
	
	enum ParamIds {
		LP_PARAM,
		HP_PARAM,
		MIX_PARAM,
		TYPE,
		NUM_PARAMS
	};
	
	enum InputIds {
		CV_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		WNOISE_OUTPUT,
		CNOISE_OUTPUT,
		NUM_OUTPUTS
	};
	
	 enum LightIds {
        NUM_LIGHTS
	};	
	
	// Panel Theme
	int Theme = 0;
	
	VRand *VR = new VRand();
	
	RCFilter filterL;
	RCFilter filterH;
	
	float NoiSetyPe = 0.0f;

	float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f, b3 = 0.0f, b4 = 0.0f, b5 = 0.0f, b6 = 0.0f, pink = 0.0f;
	
	Noise() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	void step() override;
	
	

	//Json for Panel Theme
	json_t *toJson() override	{
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "Theme", json_integer(Theme));
		return rootJ;
	}
	void fromJson(json_t *rootJ) override	{
		json_t *ThemeJ = json_object_get(rootJ, "Theme");
		if (ThemeJ)
			Theme = json_integer_value(ThemeJ);
	}
	void reset() override {}
		
};

void Noise::step() {
		
	float WNoise = VR->white();
	float BNoise = VR->brown();
	float CNoise = WNoise;
	
		  b0 = 0.99886 * b0 + WNoise * 0.0555179;
		  b1 = 0.99332 * b1 + WNoise * 0.0750759;
		  b2 = 0.96900 * b2 + WNoise * 0.1538520;
		  b3 = 0.86650 * b3 + WNoise * 0.3104856;
		  b4 = 0.55000 * b4 + WNoise * 0.5329522;
		  b5 = -0.7616 * b5 - WNoise * 0.0168980;
		  pink = b0 + b1 + b2 + b3 + b4 + b5 + b6 + WNoise * 0.5362;
		  b6 = WNoise * 0.115926;
	
	char Type = params[TYPE].value;
	switch(Type) {
		case 2:
		NoiSetyPe = 8.0f * BNoise;
		break;
		case 1:
		NoiSetyPe = pink / 5.0f;
		break;
		case 0:
		NoiSetyPe = 1.0f * WNoise;
		break;
	}
	
	// Filter
	float LP = params[LP_PARAM].value;
	float lowpassFreq = 10000.0f * powf(5.0f, clamp(2.0f*LP, 0.0f, 1.0f));
	filterL.setCutoff(lowpassFreq / engineGetSampleRate());
	filterL.process(CNoise);
	CNoise = filterL.lowpass();
	
	float HP = params[HP_PARAM].value;
	float highpassFreq = 500.0f * powf(5.0f, clamp(2.0f*HP, 0.0f, 1.0f));
	filterH.setCutoff(highpassFreq / engineGetSampleRate());
	filterH.process(CNoise);
	CNoise = filterH.highpass();
		
	float Fast_RandFloat = 1.5f * CNoise;

	float mixcontrol = params[MIX_PARAM].value;
	
	float mix = crossfade(WNoise, Fast_RandFloat, mixcontrol);

	// Noise
	if(inputs[CV_INPUT].active) {
		NoiSetyPe *= clamp(inputs[CV_INPUT].value / 10.0f, 0.0f, 1.0f);
		outputs[WNOISE_OUTPUT].value = saturate2(NoiSetyPe);
	}
	else {
		outputs[WNOISE_OUTPUT].value = saturate2(NoiSetyPe);
	}
	// Colored Noise
	if(inputs[CV_INPUT].active) {
		mix *= clamp(inputs[CV_INPUT].value / 10.0f, 0.0f, 1.0f);
		outputs[CNOISE_OUTPUT].value = saturate2(mix);
	}
	else {
		outputs[CNOISE_OUTPUT].value = saturate2(mix);
	}
};

struct NoiseWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	NoiseWidget(Noise *module);
	void step() override;
	
	Menu* createContextMenu() override;
};


NoiseWidget::NoiseWidget(Noise *module) : ModuleWidget(module) {
	
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Noise.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Noise-Dark.svg")));
	addChild(panelNightMode);
		
	addChild(Widget::create<MScrewD>(Vec(0, 0)));
	addChild(Widget::create<MScrewA>(Vec(0, 365)));
	addChild(Widget::create<MScrewC>(Vec(45, 0)));
	addChild(Widget::create<MScrewB>(Vec(45, 365)));
	
	addParam(ParamWidget::create<MThree>(Vec(12, 28), module, Noise::TYPE, 0.0, 2.0, 0.0));
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 48), module, Noise::LP_PARAM, 0.0, 1.0, 1.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 96), module, Noise::HP_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 154), module, Noise::MIX_PARAM, 0.0, 1.0, 0.5));	
	
	addInput(Port::create<SilverSixPortA>(Vec(18, 210), Port::INPUT, module, Noise::CV_INPUT));
		
	addOutput(Port::create<SilverSixPortB>(Vec(18, 250), Port::OUTPUT, module, Noise::WNOISE_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(18, 300), Port::OUTPUT, module, Noise::CNOISE_OUTPUT));

};

void NoiseWidget::step() {
	Noise *noise = dynamic_cast<Noise*>(module);
	assert(noise);
	panelClassic->visible = (noise->Theme == 0);
	panelNightMode->visible = (noise->Theme == 1);
	ModuleWidget::step();
}

struct NClassicMenu : MenuItem {
	Noise *noise;
	void onAction(EventAction &e) override {
		noise->Theme = 0;
	}
	void step() override {
		rightText = (noise->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct NNightModeMenu : MenuItem {
	Noise *noise;
	void onAction(EventAction &e) override {
		noise->Theme = 1;
	}
	void step() override {
		rightText = (noise->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* NoiseWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	Noise *noise = dynamic_cast<Noise*>(module);
	assert(noise);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<NClassicMenu>(&NClassicMenu::text, "Classic (default)", &NClassicMenu::noise, noise));
	menu->addChild(construct<NNightModeMenu>(&NNightModeMenu::text, "Night Mode", &NNightModeMenu::noise, noise));
	return menu;
}

Model *modelNoise = Model::create<Noise, NoiseWidget>("MSM", "Noise", "Noise", NOISE_TAG);