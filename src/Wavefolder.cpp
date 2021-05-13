#include "MSM.hpp"
#include "Additional/Wavefolder.hpp"
#include "MSMComponentLibrary.hpp"

struct Wavefolder : Module {
	
	enum ParamIds {
		SHAPE_PARAM,
		SHAPE_CV_PARAM,
		UP_PARAM,
		DOWN_PARAM,
		GAIN_PARAM,
		SYM_PARAM,
		TYPESWITCH,
		RANGE,
		GAIN_CV_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN_INPUT,
		SHAPE_CV_INPUT,
		UP_INPUT,
		DOWN_INPUT,
		GAIN_INPUT,
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
	
	double IN_1 = 0.0f;

	double DOWN = 0.0f;
	double UP = 0.0f;

	double SHAPE_CV = 0.0f;
	
	char Type = 0.0f;
	
	double SHAPER = 0.0f;
	double SHAPE_SYM = 0.0f;
	double SHAPE_MOD = 0.0f;
	double FOLD = 0.0f;
	
	bool type = 0;
	
	Wavefold wavefold;
	
	Wavefolder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	
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

void Wavefolder::step() {
	
	type = params[TYPESWITCH].value;
	
	IN_1 = inputs[IN_INPUT].active ? inputs[IN_INPUT].value : 0.0f;
	UP = clamp(params[UP_PARAM].value + inputs[UP_INPUT].value / 2.5f, 0.0f, 4.0f);
	DOWN = clamp(params[DOWN_PARAM].value + inputs[DOWN_INPUT].value / 2.5f, 0.0f, 4.0f);
	
		SHAPE_MOD = params[SHAPE_PARAM].value;
	
	SHAPE_CV = (inputs[SHAPE_CV_INPUT].value * params[SHAPE_CV_PARAM].value) / 5.0f;
	SHAPE_SYM = clamp(params[SYM_PARAM].value, -10.0f, 10.0f);
	
	wavefold.Shape(IN_1, SHAPE_MOD, SHAPE_CV, SHAPE_SYM, params[RANGE].value, UP, DOWN, outputs[OUT_OUTPUT].active);
	
	switch(type) {
		case 1:
			wavefold.processB();
		break;
		case 0:
			wavefold.processA();
		break;
	}
	
	FOLD = saturate(wavefold.Output());
	
	double mix = params[GAIN_PARAM].value + clamp(params[GAIN_CV_PARAM].value * inputs[GAIN_INPUT].value / 5.0f,0.0f,1.0f);
	//if (inputs[GAIN_INPUT].active) {
	//	mix *= clamp(params[GAIN_CV_PARAM].value + inputs[GAIN_INPUT].value / 5.0f, 0.0f, 1.0f);
	//}	
	double OUT = crossfade(IN_1, FOLD, mix);
	
	if(outputs[OUT_OUTPUT].active) {
		outputs[OUT_OUTPUT].value = OUT;
	}
};

struct WavefolderWidget : ModuleWidget {
	WavefolderWidget(Wavefolder *module);
};

WavefolderWidget::WavefolderWidget(Wavefolder *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/Wavefolder.svg")));
	
	addChild(Widget::create<MScrewA>(Vec(0, 0)));
	addChild(Widget::create<MScrewC>(Vec(box.size.x-15, 0)));
	addChild(Widget::create<MScrewD>(Vec(0, 365)));
	addChild(Widget::create<MScrewA>(Vec(box.size.x-15, 365)));

	addParam(ParamWidget::create<VioM2Switch>(Vec(5, 55), module, Wavefolder::TYPESWITCH, 0.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<YellowSmallKnob>(Vec(50, 105), module, Wavefolder::RANGE, 0.0, 5.0, 2.5));
	
	addParam(ParamWidget::create<RedLargeKnob>(Vec(22, 40), module, Wavefolder::SHAPE_PARAM, 0.0, 1.5, 0.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(10, 105), module, Wavefolder::SHAPE_CV_PARAM, -1.0, 1.0, 0.0));

	addParam(ParamWidget::create<BlueSmallKnob>(Vec(10, 205), module, Wavefolder::SYM_PARAM, -2.0, 2.0, 0.0));
	
	addParam(ParamWidget::create<RedSmallKnob>(Vec(10, 155), module, Wavefolder::UP_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<RedSmallKnob>(Vec(50, 155), module, Wavefolder::DOWN_PARAM, 0.0, 1.0, 0.0));
			
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(50, 205), module, Wavefolder::GAIN_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(30, 250), module, Wavefolder::GAIN_CV_PARAM, -1.0, 1.0, 0.0));
		
	addInput(Port::create<SilverSixPort>(Vec(5, 295), Port::INPUT, module, Wavefolder::UP_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(33, 295), Port::INPUT, module, Wavefolder::DOWN_INPUT));
	addInput(Port::create<SilverSixPortE>(Vec(60, 295), Port::INPUT, module, Wavefolder::GAIN_INPUT));
	
	addInput(Port::create<SilverSixPortC>(Vec(33, 330), Port::INPUT, module, Wavefolder::SHAPE_CV_INPUT));	
	addInput(Port::create<SilverSixPortA>(Vec(5, 330), Port::INPUT, module, Wavefolder::IN_INPUT));
	addOutput(Port::create<SilverSixPort>(Vec(60, 330), Port::OUTPUT, module, Wavefolder::OUT_OUTPUT));
};

Model *modelWavefolder = Model::create<Wavefolder, WavefolderWidget>("MSM", "Wavefolder", "Wavefolder", WAVESHAPER_TAG);	