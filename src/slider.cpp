#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct SimpleSlider : Module {

	enum ParamIds {
		SLIDER_PARAM,
		TYPE_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		CV_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		MAIN_OUTPUT,
		NUM_OUTPUTS
	};
	
	int Theme = 0;

	SimpleSlider() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
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

void SimpleSlider::step() {
	
	double SLIDER = clamp(params[SLIDER_PARAM].value + inputs[CV_INPUT].value / 10.f, 0.0f, 1.0f);
	double IN1 = inputs[IN1_INPUT].value;
	double IN2 = inputs[IN2_INPUT].value;
	double TYPE = params[TYPE_PARAM].value;
	double OUT = outputs[MAIN_OUTPUT].value;
	
	if(TYPE == 0.0f) {
		OUT = crossfade(IN1, IN2, SLIDER);
	}
	else {
		OUT = (IN1 + IN2) * SLIDER;
	}
	outputs[MAIN_OUTPUT].value = OUT;
	
	
};

struct SimpleSliderWidget : ModuleWidget {
	SimpleSliderWidget(SimpleSlider *module);
};

SimpleSliderWidget::SimpleSliderWidget(SimpleSlider *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/SimpleSlider.svg")));

	addChild(Widget::create<MScrewA>(Vec(15, 0)));
	addChild(Widget::create<MScrewB>(Vec(15, 365)));
	addChild(Widget::create<MScrewC>(Vec(90, 0)));
	addChild(Widget::create<MScrewA>(Vec(90, 365)));
	
	addParam(ParamWidget::create<MSMSlider>(Vec(32, 90), module, SimpleSlider::SLIDER_PARAM, 0.0, 1.0, 0.5));
	
	addParam(ParamWidget::create<VioMSwitch>(Vec(54.5, 40), module, SimpleSlider::TYPE_PARAM, 0.0, 1.0, 0.0));
	
	addInput(Port::create<SilverSixPortA>(Vec(8, 338), Port::INPUT, module, SimpleSlider::IN1_INPUT));
	addInput(Port::create<SilverSixPortE>(Vec(48, 338), Port::INPUT, module, SimpleSlider::IN2_INPUT));
	
	addInput(Port::create<SilverSixPortC>(Vec(88, 338), Port::INPUT, module, SimpleSlider::CV_INPUT));
	
	addOutput(Port::create<SilverSixPortD>(Vec(47, 299), Port::OUTPUT, module, SimpleSlider::MAIN_OUTPUT));
};

Model *modelSimpleSlider = Model::create<SimpleSlider, SimpleSliderWidget>("MSM", "Simple Slider", "Simple Slider", MIXER_TAG, AMPLIFIER_TAG);