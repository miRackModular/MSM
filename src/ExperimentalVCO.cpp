#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"
#include "Additional/ExperimentalVCO.hpp"

struct ExperimentalVCO : Module {
	
	enum ParamIds {
		LFOMODE,
		FREQ_OCT_PARAM,
		FREQ_PARAM,
		FINE_PARAM,
		FM_PARAM,
		TYPE,
		WINDOW,
		MOD1,
		MOD2,
		MOD3,
		MOD1_CV_PARAM,
		MOD2_CV_PARAM,
		MOD3_CV_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		VOCT_INPUT,
		RESET_INPUT,
		TYPE_INPUT,
		WINDOW_CV,
		MOD1_CV,
		MOD2_CV,
		MOD3_CV,
		LIN_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUTPUT,
		NUM_OUTPUTS
	};
	
	MSVCO<8, 8> oscillator;
	
	int Oct = 0;
	
	// Panel Theme
	int Theme = 0;
			
	ExperimentalVCO() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
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
};

void ExperimentalVCO::step() {
	
	char type = 0;
	// Pitch	
	//float pitchCv = 0.0f;
	float FreqFine = params[FREQ_PARAM].value;
	float Fine = params[FINE_PARAM].value;
	//float window = 0.0f;
	float mod1, mod2, mod3;
	
	Oct = clamp(params[FREQ_OCT_PARAM].value, 0.0, 12.0) * 12;
	
	bool lfomode = params[LFOMODE].value;
	
	float pitchCv = 12.0f * inputs[VOCT_INPUT].value;
	
	pitchCv += (quadraticBipolar(params[FM_PARAM].value) * 12.0f * inputs[LIN_INPUT].value);
	
	oscillator.setPitch(Oct, pitchCv + FreqFine + Fine, lfomode);
	oscillator.process(1.0f / engineGetSampleRate(), inputs[RESET_INPUT].value, inputs[RESET_INPUT].active);
	
	float window = clamp(params[WINDOW].value + (120.0f * inputs[WINDOW_CV].value), 512.0f, 2047.0f);
	
	if(inputs[MOD1_CV].active) {
		mod1 = clamp(params[MOD1].value + (quadraticBipolar(params[MOD1_CV_PARAM].value) * 12.0f * inputs[MOD1_CV].value) / 10.0f, 0.01f, 1.0f);
	}
	else {
		mod1 = clamp(params[MOD1].value, 0.01f, 1.0f);
	}
	if(inputs[MOD2_CV].active) {
		mod2 = clamp(params[MOD2].value + (quadraticBipolar(params[MOD2_CV_PARAM].value) * 12.0f * inputs[MOD2_CV].value) / 10.0f, 0.01f, 1.0f);
	}
	else {
		mod2 = clamp(params[MOD2].value, 0.01f, 1.0f);
	}
	if(inputs[MOD3_CV].active) {
		mod3 = clamp(params[MOD3].value + (quadraticBipolar(params[MOD3_CV_PARAM].value) * 12.0f * inputs[MOD3_CV].value) / 10.0f, 0.01f, 1.0f);
	}
	else {
		mod3 = clamp(params[MOD3].value, 0.01f, 1.0f);
	}
	if(inputs[TYPE_INPUT].active) {
		type = clamp(params[TYPE].value + 3 * inputs[TYPE_INPUT].value, 0.0, 13.0);
	}
	else {
		type = clamp(params[TYPE].value, 0.0, 13.0);
	}
	
	oscillator.TYPE(type);	
	oscillator.Window(window);
	oscillator.Mods(mod1, mod2, mod3);

	oscillator.outEnabled(outputs[OUTPUT].active);
	
	if(outputs[OUTPUT].active) {
		outputs[OUTPUT].value = saturate2(7.0f * oscillator.getOutput());
	}
	
};

struct ExperimentalVCOWidget : ModuleWidget {
	ExperimentalVCOWidget(ExperimentalVCO *module);
};

ExperimentalVCOWidget::ExperimentalVCOWidget(ExperimentalVCO *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/MSVCO.svg")));
	
	addChild(Widget::create<MScrewA>(Vec(15, 0)));
	addChild(Widget::create<MScrewC>(Vec(box.size.x-30, 0)));
	addChild(Widget::create<MScrewD>(Vec(15, 365)));
	addChild(Widget::create<MScrewB>(Vec(box.size.x-30, 365)));
	
	// Params
	addParam(ParamWidget::create<VioM2Switch>(Vec(98, 293), module, ExperimentalVCO::LFOMODE, 0.0, 1.0, 1.0));	
	
	addParam(ParamWidget::create<RedLargeToggleKnob>(Vec(81.5, 26), module, ExperimentalVCO::FREQ_OCT_PARAM, 0.0, 6.0, 3.0));	
	addParam(ParamWidget::create<RedSmallToggleKnob>(Vec(29.5, 34.5), module, ExperimentalVCO::FREQ_PARAM, 0.0, 12.0, 0.0));	
	addParam(ParamWidget::create<RedSmallKnob>(Vec(149.5, 34.5), module, ExperimentalVCO::FINE_PARAM, -1.0, 1.0, 0.0));	
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(29.5, 99.5), module, ExperimentalVCO::MOD1, 0.01f, 1.0, 1.0));	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(90, 99.5), module, ExperimentalVCO::MOD2, 0.01f, 1.0, 1.0));	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(149.5, 99.5), module, ExperimentalVCO::MOD3, 0.01f, 1.0, 1.0));	
	
	addParam(ParamWidget::create<BlueSmallKnob>(Vec(29.5, 164.5), module, ExperimentalVCO::MOD1_CV_PARAM, 1.0f, -1.0, 0.0));	
	addParam(ParamWidget::create<BlueSmallKnob>(Vec(90, 164.5), module, ExperimentalVCO::MOD2_CV_PARAM, 1.0f, -1.0, 0.0));	
	addParam(ParamWidget::create<BlueSmallKnob>(Vec(149.5, 164.5), module, ExperimentalVCO::MOD3_CV_PARAM, 1.0f, -1.0, 0.0));	
	
	addParam(ParamWidget::create<GreenToggleKnobSmall>(Vec(90, 229.5), module, ExperimentalVCO::TYPE, 0.0, 13.0, 0.0));	
	addParam(ParamWidget::create<YellowSmallKnob>(Vec(29.5, 229.5), module, ExperimentalVCO::WINDOW, 512.0, 2047.0, 1024.0)); 
	addParam(ParamWidget::create<RedSmallKnob>(Vec(149.5, 229.5), module, ExperimentalVCO::FM_PARAM, -1.0, 1.0, 0.0));	
	
	// Ports
	addInput(Port::create<SilverSixPortB>(Vec(12.5, 287.5), Port::INPUT, module, ExperimentalVCO::WINDOW_CV));
	addInput(Port::create<SilverSixPortB>(Vec(57, 287.5), Port::INPUT, module, ExperimentalVCO::TYPE_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(127.5, 287.5), Port::INPUT, module, ExperimentalVCO::RESET_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(173, 287.5), Port::INPUT, module, ExperimentalVCO::LIN_INPUT));	

	addInput(Port::create<SilverSixPortA>(Vec(12.5, 328), Port::INPUT, module, ExperimentalVCO::VOCT_INPUT));	
	addInput(Port::create<SilverSixPortC>(Vec(57, 328), Port::INPUT, module, ExperimentalVCO::MOD1_CV));	
	addInput(Port::create<SilverSixPortD>(Vec(92.5, 328), Port::INPUT, module, ExperimentalVCO::MOD2_CV));	
	addInput(Port::create<SilverSixPortE>(Vec(127.5, 328), Port::INPUT, module, ExperimentalVCO::MOD3_CV));		
	addOutput(Port::create<SilverSixPort>(Vec(173, 328), Port::OUTPUT, module, ExperimentalVCO::OUTPUT));
	
};

Model *modelExperimentalVCO = Model::create<ExperimentalVCO, ExperimentalVCOWidget>("MSM", "Treasure VCO", "Treasure VCO", OSCILLATOR_TAG);