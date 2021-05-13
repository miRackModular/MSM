#include "MSM.hpp"
#include "Additional/WaveShaper.hpp"
#include "MSMComponentLibrary.hpp"

struct WaveShaper : Module {
	
	enum ParamIds {
		SHAPE_1_PARAM,
		SHAPE_2_PARAM,
		SHAPE_3_PARAM,
		SHAPE_1_CV_PARAM,
		SHAPE_2_CV_PARAM,
		SHAPE_3_CV_PARAM,
		OUTPUT_GAIN_PARAM,
		MIX_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		MASTER_INPUT,
		SHAPE_1_CV_INPUT,
		SHAPE_2_CV_INPUT,
		SHAPE_3_CV_INPUT,
		IN_A_INPUT,
		IN_B_INPUT,
		IN_C_INPUT,
		//MIX_CV,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	
	enum LightIds {
		NUM_LIGHTS
	};
	
	int Theme = 0;
	
	double IN_M = 0.0f, IN_A = 0.0f, IN_B = 0.0f, IN_C = 0.0f;
	double SHAPE_MOD1 = 0.0f, SHAPE_CV1 = 0.0f;
	double SHAPE_MOD2 = 0.0f, SHAPE_CV2 = 0.0f;
	double SHAPE_MOD3 = 0.0f, SHAPE_CV3 = 0.0f;
	
	double OUT = 0.0f;
		
	WaveShape<8, 8> waveS;
	
	WaveShaper() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	
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

void WaveShaper::step()
{
	IN_M = inputs[MASTER_INPUT].active ? inputs[MASTER_INPUT].value : 0.0f;
	IN_A = inputs[IN_A_INPUT].active ? inputs[IN_A_INPUT].value : 0.0f;
	IN_B = inputs[IN_B_INPUT].active ? inputs[IN_B_INPUT].value : 0.0f;
	IN_C = inputs[IN_C_INPUT].active ? inputs[IN_C_INPUT].value : 0.0f;
	
	SHAPE_MOD1 = params[SHAPE_1_PARAM].value;
	SHAPE_MOD2 = params[SHAPE_2_PARAM].value;
	SHAPE_MOD3 = params[SHAPE_3_PARAM].value;
	
	SHAPE_CV1 = (inputs[SHAPE_1_CV_INPUT].value * params[SHAPE_1_CV_PARAM].value);
	SHAPE_CV2 = (inputs[SHAPE_2_CV_INPUT].value * params[SHAPE_2_CV_PARAM].value);
	SHAPE_CV3 = (inputs[SHAPE_3_CV_INPUT].value * params[SHAPE_3_CV_PARAM].value);
	
	waveS.process();
	
	waveS.Shape1(IN_M + IN_A,SHAPE_MOD1,SHAPE_CV1, outputs[OUT_OUTPUT].active);
	waveS.Shape2(IN_M + IN_B,SHAPE_MOD2,SHAPE_CV2, outputs[OUT_OUTPUT].active);
	waveS.Shape3(IN_M + IN_C,SHAPE_MOD3,SHAPE_CV3, outputs[OUT_OUTPUT].active);
		
	double mix = params[MIX_PARAM].value;
		OUT = waveS.MasterOutput() *  params[OUTPUT_GAIN_PARAM].value;
	double MixOUT = crossfade((IN_M + IN_A + IN_B + IN_C), OUT, mix);
		
	if(outputs[OUT_OUTPUT].active) {
		outputs[OUT_OUTPUT].value = saturate(MixOUT);
	}
	else {
		outputs[OUT_OUTPUT].value = 0.0f;
	}
	
}

struct WaveShaperWidget : ModuleWidget {
	WaveShaperWidget(WaveShaper *module);
};

WaveShaperWidget::WaveShaperWidget(WaveShaper *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/WaveShaper.svg")));
	
	addChild(Widget::create<MScrewA>(Vec(15, 0)));
	addChild(Widget::create<MScrewD>(Vec(15, 365)));
	addChild(Widget::create<MScrewB>(Vec(135, 0)));
	addChild(Widget::create<MScrewA>(Vec(135, 365)));
	
	addParam(ParamWidget::create<RedLargeKnob>(Vec(25, 25), module, WaveShaper::SHAPE_1_PARAM, 1.0, -0.4, 1.0));
	addParam(ParamWidget::create<BlueLargeKnob>(Vec(95, 25), module, WaveShaper::SHAPE_1_CV_PARAM, -1.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<RedLargeKnob>(Vec(25, 85), module, WaveShaper::SHAPE_2_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<BlueLargeKnob>(Vec(95, 85), module, WaveShaper::SHAPE_2_CV_PARAM, -1.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<RedLargeKnob>(Vec(25, 145), module, WaveShaper::SHAPE_3_PARAM, -1.0, 1.0, -1.0));
	addParam(ParamWidget::create<BlueLargeKnob>(Vec(95, 145), module, WaveShaper::SHAPE_3_CV_PARAM, -1.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(25, 205), module, WaveShaper::MIX_PARAM, 0.0, 1.0, 0.5));
	//addInput(Port::create<SilverSixPortA>(Vec(5, 245), Port::INPUT, module, WaveShaper::MIX_CV));
	
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(95, 205), module, WaveShaper::OUTPUT_GAIN_PARAM, 0.0, 1.0, 0.5));
	
	addInput(Port::create<SilverSixPortA>(Vec(10, 275), Port::INPUT, module, WaveShaper::MASTER_INPUT));

	addInput(Port::create<SilverSixPortC>(Vec(50, 275), Port::INPUT, module, WaveShaper::IN_A_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(90, 275), Port::INPUT, module, WaveShaper::IN_B_INPUT));
	addInput(Port::create<SilverSixPortA>(Vec(130, 275), Port::INPUT, module, WaveShaper::IN_C_INPUT));
	
	addInput(Port::create<SilverSixPortD>(Vec(10, 320), Port::INPUT, module, WaveShaper::SHAPE_1_CV_INPUT));
	addInput(Port::create<SilverSixPortA>(Vec(50, 320), Port::INPUT, module, WaveShaper::SHAPE_2_CV_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(90, 320), Port::INPUT, module, WaveShaper::SHAPE_3_CV_INPUT));

	addOutput(Port::create<SilverSixPortB>(Vec(130, 320), Port::OUTPUT, module, WaveShaper::OUT_OUTPUT));
};

Model *modelWaveShaper = Model::create<WaveShaper, WaveShaperWidget>("MSM", "WaveShaper", "WaveShaper", WAVESHAPER_TAG);