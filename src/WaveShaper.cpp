#include "MSM.hpp"
#include <math.h>

# define M_PI           3.14159265358979323846  /* pi */


struct WaveShaper : Module {
	
	enum ParamIds {
		SHAPE_1_PARAM,
		SHAPE_2_PARAM,
		SHAPE_3_PARAM,
		SHAPE_1_CV_PARAM,
		SHAPE_2_CV_PARAM,
		SHAPE_3_CV_PARAM,
		INPUT_GAIN_PARAM,
		OUTPUT_GAIN_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN_1_INPUT,
		SHAPE_1_CV_INPUT,
		SHAPE_2_CV_INPUT,
		SHAPE_3_CV_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUT_1_OUTPUT,
		NUM_OUTPUTS
	};
	
	enum LightIds {
		NUM_LIGHTS
	};
	
	WaveShaper() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	
	void step() override;	
};

void WaveShaper::step()
{
	//SHAPER 1
	float IN_1 = inputs[IN_1_INPUT].value * params[INPUT_GAIN_PARAM].value;
	
	float SHAPE_1_CV = inputs[SHAPE_1_CV_INPUT].value * params[SHAPE_1_CV_PARAM].value;
	float SHAPE_1_MOD = SHAPE_1_CV + params[SHAPE_1_PARAM].value;
	
	float knob1 = SHAPE_1_MOD *(SHAPE_1_MOD - 4.0);
	
	float SIGNAL_1 = IN_1;
	
	float COMBINE1 = SIGNAL_1*(1.0 - knob1 + 1.0);
	
	float SIG_1 = -(COMBINE1 - 1.0) * 2.0;
	float X = COMBINE1 > 2.0;
	float Y = SIG_1 * X;
	
	float SIG_2 = -(COMBINE1 + 1.0) * 2.0;
	float Z = COMBINE1 <- 2.0;
	float W = SIG_2 * Z;
	
	float SHAPER_1 = COMBINE1 + Y + W;
	float SHAPER_1_OUT = 2.0 * asin(sin(SHAPER_1*M_PI/2.0))/M_PI;
	
	float OUT_1 = SHAPER_1_OUT;
	
	
	
	//SHAPER 2
	
	float SHAPE_2_CV = inputs[SHAPE_2_CV_INPUT].value * params[SHAPE_2_CV_PARAM].value;
	float SHAPE_2_MOD = SHAPE_2_CV + params[SHAPE_2_PARAM].value;
		
	float SIGNAL_2 = IN_1;
	float multiplein = IN_1 + 1.0;
	
	float multiple = SIGNAL_2 * (SHAPE_2_MOD * multiplein + 4.0);
	
	float SHAPER_2_OUT = 2.0 * asin(sin(multiple*M_PI/2.0))/M_PI;
	
	float OUT_2 = SHAPER_2_OUT;
	
	
	//SHAPER 3
	
	float SHAPE_3_CV = inputs[SHAPE_3_CV_INPUT].value * params[SHAPE_3_CV_PARAM].value;
	float SHAPE_3_MOD = SHAPE_3_CV + params[SHAPE_3_PARAM].value;
		
	float knob2 = SHAPE_3_MOD *(SHAPE_3_MOD - 4.0);
	
	float SIGNAL_3 = IN_1;
	
	float COMBINE2 = SIGNAL_3*(1.0 + knob2);

	float SIG_3 = -(COMBINE2 - 1.0) * 2.0;
	float O = COMBINE2 > 2.0;
	float P = SIG_3 * O;
	float SIN1 = sin(P*3.0)*2.0;
	
	float SIG_4 = -(COMBINE2 + 1.0) * 2.0;
	float L = COMBINE2 <- 2.0;
	float K = SIG_4 * L;
	float SIN2 = -sin(K*-3.0)*2.0;
	
		
	float SHAPER_3 = COMBINE2 + SIN1 + SIN2;
	
	float SHAPER_3_OUT = 2.0 * asin(sin(SHAPER_3*M_PI/2.0))/M_PI;
	
	float OUT_3 = SHAPER_3_OUT;
	
	//OUTPUT
	
	float OUT = OUT_1 + OUT_2 + OUT_3;
	float OUTPUT = OUT * params[OUTPUT_GAIN_PARAM].value;
	
	outputs[OUT_1_OUTPUT].value = OUTPUT * 3.5;
	
}

WaveShaperWidget::WaveShaperWidget()

{
	WaveShaper *module = new WaveShaper();
	setModule(module);
	box.size = Vec(165, 380);

    panel = new SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/WaveShaper.svg")));
    addChild(panel);
	
	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(135, 0)));
	addChild(createScrew<ScrewSilver>(Vec(135, 365)));
	
	addParam(createParam<GreyLargeKnob>(Vec(25, 40), module, WaveShaper::SHAPE_1_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreyLargeKnob>(Vec(95, 40), module, WaveShaper::SHAPE_1_CV_PARAM, 0.0, 1.0, 0.0));
	
	addParam(createParam<GreyLargeKnob>(Vec(25, 100), module, WaveShaper::SHAPE_2_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreyLargeKnob>(Vec(95, 100), module, WaveShaper::SHAPE_2_CV_PARAM, 0.0, 1.0, 0.0));
	
	addParam(createParam<GreyLargeKnob>(Vec(25, 160), module, WaveShaper::SHAPE_3_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreyLargeKnob>(Vec(95, 160), module, WaveShaper::SHAPE_3_CV_PARAM, 0.0, 1.0, 0.0));
	
	addParam(createParam<GreyLargeKnob>(Vec(25, 235), module, WaveShaper::INPUT_GAIN_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreyLargeKnob>(Vec(95, 235), module, WaveShaper::OUTPUT_GAIN_PARAM, 0.0, 1.0, 0.0));
	
	addInput(createInput<SilverSixPort>(Vec(15, 330), module, WaveShaper::IN_1_INPUT));
	
	addInput(createInput<SilverSixPort>(Vec(15, 300), module, WaveShaper::SHAPE_1_CV_INPUT));
	addInput(createInput<SilverSixPort>(Vec(70, 300), module, WaveShaper::SHAPE_2_CV_INPUT));
	addInput(createInput<SilverSixPort>(Vec(125, 300), module, WaveShaper::SHAPE_3_CV_INPUT));

	
	addOutput(createOutput<SilverSixPort>(Vec(125, 330), module, WaveShaper::OUT_1_OUTPUT));

}
