#include "MSM.hpp"

struct Fade : Module {
	
	enum ParamIds {
		CF_A_PARAM,
		CF_B_PARAM,
		CF_AB_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN_1_INPUT,
		IN_2_INPUT,
		IN_3_INPUT,
		IN_4_INPUT,
		CVA_INPUT,
		CVB_INPUT,
		CVAB_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUT_A_OUTPUT,
		OUT_B_OUTPUT,
		OUT_AB_OUTPUT,
		NUM_OUTPUTS
	};
	
	 enum LightIds {
        NUM_LIGHTS
	};
		
	Fade() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	
	void step() override;	
};

void Fade::step() {
	
	float CrossfadeA = clampf(params[CF_A_PARAM].value + inputs[CVA_INPUT].value, 0.0, 1.0);
	float IN_1 = inputs[IN_1_INPUT].value;
	float IN_2 = inputs[IN_2_INPUT].value;
	float OutA;
	
	if(CrossfadeA < 0.5)
		OutA = crossf(IN_1, IN_1, CrossfadeA);
	else(CrossfadeA > 1.0);
		OutA = crossf(IN_1, IN_2, CrossfadeA);
	outputs[OUT_A_OUTPUT].value = OutA;
	
	float CrossfadeB = clampf(params[CF_B_PARAM].value + inputs[CVB_INPUT].value, 0.0, 1.0);
	float IN_3 = inputs[IN_3_INPUT].value;
	float IN_4 = inputs[IN_4_INPUT].value;
	float OutB;
	
	if(CrossfadeB < 0.5) 
		OutB = crossf(IN_3, IN_3, CrossfadeB);
	else(CrossfadeB > 1.0);
		OutB = crossf(IN_3, IN_4, CrossfadeB);
	outputs[OUT_B_OUTPUT].value = OutB;
	
	float CrossfadeAB = clampf(params[CF_AB_PARAM].value + inputs[CVAB_INPUT].value, 0.0, 1.0);
	float IN_A = OutA;
	float IN_B = OutB;
	float OutAB;
	
	if(CrossfadeAB < 0.5) 
		OutAB = crossf(IN_A, IN_A, CrossfadeAB);
	else(CrossfadeAB > 1.0);
		OutAB = crossf(IN_A, IN_B, CrossfadeAB);
	outputs[OUT_AB_OUTPUT].value = OutAB;
	
};

FadeWidget::FadeWidget() 
{
	Fade *module = new Fade();
	setModule(module);
	box.size = Vec(120, 380);

    panel = new SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/Fade.svg")));
    addChild(panel);
	
	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(90, 0)));
	addChild(createScrew<ScrewSilver>(Vec(90, 365)));
	
	//Params
	addParam(createParam<GreyLargeKnob>(Vec(10, 65), module, Fade::CF_A_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<GreyLargeKnob>(Vec(64, 65), module, Fade::CF_B_PARAM, 0.0, 1.0, 0.5));
	addParam(createParam<GreyLargeKnob>(Vec(38, 110), module, Fade::CF_AB_PARAM, 0.0, 1.0, 0.5));

	//Inputs
	addInput(createInput<SilverSixPort>(Vec(10, 240), module, Fade::IN_1_INPUT));
	addInput(createInput<SilverSixPort>(Vec(49, 240), module, Fade::IN_2_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 280), module, Fade::IN_3_INPUT));
	addInput(createInput<SilverSixPort>(Vec(49, 280), module, Fade::IN_4_INPUT));
		
	addInput(createInput<SilverSixPort>(Vec(10, 320), module, Fade::CVA_INPUT));
	addInput(createInput<SilverSixPort>(Vec(49, 320), module, Fade::CVB_INPUT));
	addInput(createInput<SilverSixPort>(Vec(49, 200), module, Fade::CVAB_INPUT));

	//Outputs
	addOutput(createOutput<SilverSixPort>(Vec(87, 240), module, Fade::OUT_A_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(87, 280), module, Fade::OUT_B_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(87, 320), module, Fade::OUT_AB_OUTPUT));
	
};

