#include "MSM.hpp"



struct SwitchedMult : Module {
	
	enum ParamIds {
		SWITCH2OR4_1_PARAM,
		SWITCH2OR4_2_PARAM,
		SWITCH2OR4_3_PARAM,
		SWITCH2OR4_4_PARAM,
		SWITCH2OR4_5_PARAM,
		SWITCH2OR4_6_PARAM,
		SWITCH2OR4_7_PARAM,
		SWITCH2OR4_8_PARAM,
		MUTE_1_PARAM,
		MUTE_2_PARAM,
		MUTE_3_PARAM,
		MUTE_4_PARAM,
		MUTE_5_PARAM,
		MUTE_6_PARAM,
		MUTE_7_PARAM,
		MUTE_8_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN_1_INPUT,
		IN_2_INPUT,
		IN_3_INPUT,
		IN_4_INPUT,
		IN_5_INPUT,
		IN_6_INPUT,
		IN_7_INPUT,
		IN_8_INPUT,
		CV_1_INPUT,
		CV_2_INPUT,
		CV_3_INPUT,
		CV_4_INPUT,
		CV_5_INPUT,
		CV_6_INPUT,
		CV_7_INPUT,
		CV_8_INPUT,
		CV_11_INPUT,
		CV_12_INPUT,
		CV_13_INPUT,
		CV_14_INPUT,
		CV_15_INPUT,
		CV_16_INPUT,
		CV_17_INPUT,
		CV_18_INPUT,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUT_1_OUTPUT,
		OUT_2_OUTPUT,
		OUT_3_OUTPUT,
		OUT_4_OUTPUT,
		OUT_5_OUTPUT,
		OUT_6_OUTPUT,
		OUT_7_OUTPUT,
		OUT_8_OUTPUT,
		NUM_OUTPUTS
	};
		
	SwitchedMult() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;

};

void SwitchedMult::step() {

	//Inputs
	float IN_1 = inputs[IN_1_INPUT].active ? inputs[IN_1_INPUT].value : 0.0f;
	float IN_2 = inputs[IN_2_INPUT].active ? inputs[IN_2_INPUT].value : 0.0f;
	float IN_3 = inputs[IN_3_INPUT].active ? inputs[IN_3_INPUT].value : 0.0f;
	float IN_4 = inputs[IN_4_INPUT].active ? inputs[IN_4_INPUT].value : 0.0f;
	float IN_5 = inputs[IN_5_INPUT].active ? inputs[IN_5_INPUT].value : 0.0f;
	float IN_6 = inputs[IN_6_INPUT].active ? inputs[IN_6_INPUT].value : 0.0f;
	float IN_7 = inputs[IN_7_INPUT].active ? inputs[IN_7_INPUT].value : 0.0f;
	float IN_8 = inputs[IN_8_INPUT].active ? inputs[IN_8_INPUT].value : 0.0f;	
	//Mutes
	bool MUTE_1 = params[MUTE_1_PARAM].value + inputs[CV_1_INPUT].value > 0.0;
	bool MUTE_2 = params[MUTE_2_PARAM].value + inputs[CV_2_INPUT].value > 0.0;
	bool MUTE_3 = params[MUTE_3_PARAM].value + inputs[CV_3_INPUT].value > 0.0;
	bool MUTE_4 = params[MUTE_4_PARAM].value + inputs[CV_4_INPUT].value > 0.0;
	bool MUTE_5 = params[MUTE_5_PARAM].value + inputs[CV_5_INPUT].value > 0.0;
	bool MUTE_6 = params[MUTE_6_PARAM].value + inputs[CV_6_INPUT].value > 0.0;
	bool MUTE_7 = params[MUTE_7_PARAM].value + inputs[CV_7_INPUT].value > 0.0;
	bool MUTE_8 = params[MUTE_8_PARAM].value + inputs[CV_8_INPUT].value > 0.0;
	
	
	if(MUTE_1) {
		IN_1 = 0.0;
	}
	if(MUTE_2) {
		IN_2 = 0.0;
	}
	if(MUTE_3) {
		IN_3 = 0.0;
	}
	if(MUTE_4) {
		IN_4 = 0.0;
	}
	if(MUTE_5) {
		IN_5 = 0.0;
	}
	if(MUTE_6) {
		IN_6 = 0.0;
	}
	if(MUTE_7) {
		IN_7 = 0.0;
	}
	if(MUTE_8) {
		IN_8 = 0.0;
	}
	
	//Switch 2or4
	bool Switch2or4_1 = params[SWITCH2OR4_1_PARAM].value + inputs[CV_11_INPUT].value > 0.0;
	bool Switch2or4_2 = params[SWITCH2OR4_2_PARAM].value + inputs[CV_12_INPUT].value > 0.0;
	bool Switch2or4_3 = params[SWITCH2OR4_3_PARAM].value + inputs[CV_13_INPUT].value > 0.0;
	bool Switch2or4_4 = params[SWITCH2OR4_4_PARAM].value + inputs[CV_14_INPUT].value > 0.0;
	bool Switch2or4_5 = params[SWITCH2OR4_5_PARAM].value + inputs[CV_15_INPUT].value > 0.0;
	bool Switch2or4_6 = params[SWITCH2OR4_6_PARAM].value + inputs[CV_16_INPUT].value > 0.0;
	bool Switch2or4_7 = params[SWITCH2OR4_7_PARAM].value + inputs[CV_17_INPUT].value > 0.0;
	bool Switch2or4_8 = params[SWITCH2OR4_8_PARAM].value + inputs[CV_18_INPUT].value > 0.0;
	
	float Out_1, Out_2, Out_3, Out_4, Out_5, Out_6, Out_7, Out_8;
	
	if(Switch2or4_1) {
		Out_1 = IN_1 + IN_2;		
		outputs[OUT_1_OUTPUT].value = Out_1;
		outputs[OUT_2_OUTPUT].value = Out_1;
	}
	else {
		Out_1 = IN_1 + IN_2;
		outputs[OUT_1_OUTPUT].value = Out_1;
		outputs[OUT_2_OUTPUT].value = Out_1;
		outputs[OUT_3_OUTPUT].value = Out_1;		
		outputs[OUT_4_OUTPUT].value = Out_1;
	}
	if(Switch2or4_2) {
		Out_2 = IN_1 + IN_2;
		outputs[OUT_1_OUTPUT].value = Out_2;
		outputs[OUT_2_OUTPUT].value = Out_2;
	}
	else {
		Out_2 = IN_1 + IN_2;
		outputs[OUT_1_OUTPUT].value = Out_2;
		outputs[OUT_2_OUTPUT].value = Out_2;
		outputs[OUT_3_OUTPUT].value = Out_2;		
		outputs[OUT_4_OUTPUT].value = Out_2;
	}
	if(Switch2or4_3) {
		Out_3 = IN_3 + IN_4;
		outputs[OUT_3_OUTPUT].value = Out_3;		
		outputs[OUT_4_OUTPUT].value = Out_3;	
	}
	else {
		Out_3 = IN_3 + IN_4;		
		outputs[OUT_1_OUTPUT].value = Out_3;
		outputs[OUT_2_OUTPUT].value = Out_3;
		outputs[OUT_3_OUTPUT].value = Out_3;		
		outputs[OUT_4_OUTPUT].value = Out_3;		
	}
	if(Switch2or4_4) {
		Out_4 = IN_3 + IN_4;
		outputs[OUT_3_OUTPUT].value = Out_4;		
		outputs[OUT_4_OUTPUT].value = Out_4;	
	}
	else {
		Out_4 = IN_3 + IN_4;
		outputs[OUT_1_OUTPUT].value = Out_4;
		outputs[OUT_2_OUTPUT].value = Out_4;
		outputs[OUT_3_OUTPUT].value = Out_4;		
		outputs[OUT_4_OUTPUT].value = Out_4;		
	}
	if(Switch2or4_5) {
		Out_5 = IN_5 + IN_6;
		outputs[OUT_5_OUTPUT].value = Out_5;
		outputs[OUT_6_OUTPUT].value = Out_5;	
	}
	else {
		Out_5 = IN_5 + IN_6;
		outputs[OUT_5_OUTPUT].value = Out_5;
		outputs[OUT_6_OUTPUT].value = Out_5;		
		outputs[OUT_7_OUTPUT].value = Out_5;
		outputs[OUT_8_OUTPUT].value = Out_5;		
	}
	if(Switch2or4_6) {
		Out_6 = IN_5 + IN_6;	
		outputs[OUT_5_OUTPUT].value = Out_6;
		outputs[OUT_6_OUTPUT].value = Out_6;	
	}
	else {
		Out_6 = IN_5 + IN_6;
		outputs[OUT_5_OUTPUT].value = Out_6;
		outputs[OUT_6_OUTPUT].value = Out_6;		
		outputs[OUT_7_OUTPUT].value = Out_6;
		outputs[OUT_8_OUTPUT].value = Out_6;		
	}
	if(Switch2or4_7) {
		Out_7 = IN_7 + IN_8;
		outputs[OUT_7_OUTPUT].value = Out_7;		
		outputs[OUT_8_OUTPUT].value = Out_7;	
	}
	else {
		Out_7 = IN_7 + IN_8;
		outputs[OUT_5_OUTPUT].value = Out_7;
		outputs[OUT_6_OUTPUT].value = Out_7;		
		outputs[OUT_7_OUTPUT].value = Out_7;
		outputs[OUT_8_OUTPUT].value = Out_7;		
	}
	if(Switch2or4_8) {
		Out_8 = IN_7 + IN_8;
		outputs[OUT_7_OUTPUT].value = Out_8;		
		outputs[OUT_8_OUTPUT].value = Out_8;	
	}
	else {
		Out_8 = IN_7 + IN_8;
		outputs[OUT_5_OUTPUT].value = Out_8;
		outputs[OUT_6_OUTPUT].value = Out_8;		
		outputs[OUT_7_OUTPUT].value = Out_8;
		outputs[OUT_8_OUTPUT].value = Out_8;		
	}

};

SwitchedMultWidget::SwitchedMultWidget() 
{
	SwitchedMult *module = new SwitchedMult();
    setModule(module);
    box.size = Vec(195, 380);
	
	{
        SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "/res/SwitchedMult.svg")));
		addChild(panel);
    }
	
	int space = 25;
	
	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(165, 0)));
	addChild(createScrew<ScrewSilver>(Vec(165, 365)));
	//PARAM
	addParam(createParam<CKSS>(Vec(105, 35 + space), module, SwitchedMult::SWITCH2OR4_1_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 70 + space), module, SwitchedMult::SWITCH2OR4_2_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 105 + space), module, SwitchedMult::SWITCH2OR4_3_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 140 + space), module, SwitchedMult::SWITCH2OR4_4_PARAM, 0.0, 1.0, 1.0));
	
	addParam(createParam<CKSS>(Vec(105, 188 + space), module, SwitchedMult::SWITCH2OR4_5_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 222 + space), module, SwitchedMult::SWITCH2OR4_6_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 257 + space), module, SwitchedMult::SWITCH2OR4_7_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<CKSS>(Vec(105, 292 + space), module, SwitchedMult::SWITCH2OR4_8_PARAM, 0.0, 1.0, 1.0));
	
	addParam(createParam<CKSS>(Vec(75, 35 + space), module, SwitchedMult::MUTE_1_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 70 + space), module, SwitchedMult::MUTE_2_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 105 + space), module, SwitchedMult::MUTE_3_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 140 + space), module, SwitchedMult::MUTE_4_PARAM, 0.0, 1.0, 0.0));
	
	addParam(createParam<CKSS>(Vec(75, 188 + space), module, SwitchedMult::MUTE_5_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 222 + space), module, SwitchedMult::MUTE_6_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 257 + space), module, SwitchedMult::MUTE_7_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(75, 292 + space), module, SwitchedMult::MUTE_8_PARAM, 0.0, 1.0, 0.0));
	
	//INPUTS
	addInput(createInput<SilverSixPort>(Vec(10, 35 + space), module, SwitchedMult::IN_1_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 70 + space), module, SwitchedMult::IN_2_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 105 + space), module, SwitchedMult::IN_3_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 140 + space), module, SwitchedMult::IN_4_INPUT));
	
	addInput(createInput<SilverSixPort>(Vec(10, 187 + space), module, SwitchedMult::IN_5_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 222 + space), module, SwitchedMult::IN_6_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 257 + space), module, SwitchedMult::IN_7_INPUT));
	addInput(createInput<SilverSixPort>(Vec(10, 292	+ space), module, SwitchedMult::IN_8_INPUT));
	
	//CV
	addInput(createInput<SilverSixPort>(Vec(40, 35 + space), module, SwitchedMult::CV_1_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 70 + space), module, SwitchedMult::CV_2_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 105 + space), module, SwitchedMult::CV_3_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 140 + space), module, SwitchedMult::CV_4_INPUT));
	
	addInput(createInput<SilverSixPort>(Vec(40, 187 + space), module, SwitchedMult::CV_5_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 222 + space), module, SwitchedMult::CV_6_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 257 + space), module, SwitchedMult::CV_7_INPUT));
	addInput(createInput<SilverSixPort>(Vec(40, 292 + space), module, SwitchedMult::CV_8_INPUT));
	
	addInput(createInput<SilverSixPort>(Vec(130, 35 + space), module, SwitchedMult::CV_11_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 70 + space), module, SwitchedMult::CV_12_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 105 + space), module, SwitchedMult::CV_13_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 140 + space), module, SwitchedMult::CV_14_INPUT));
	
	addInput(createInput<SilverSixPort>(Vec(130, 187 + space), module, SwitchedMult::CV_15_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 222 + space), module, SwitchedMult::CV_16_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 257 + space), module, SwitchedMult::CV_17_INPUT));
	addInput(createInput<SilverSixPort>(Vec(130, 292 + space), module, SwitchedMult::CV_18_INPUT));
	
	//OUTPUTS
	addOutput(createOutput<SilverSixPort>(Vec(162, 35 + space), module, SwitchedMult::OUT_1_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 70 + space), module, SwitchedMult::OUT_2_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 105 + space), module, SwitchedMult::OUT_3_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 140 + space), module, SwitchedMult::OUT_4_OUTPUT));
	
	addOutput(createOutput<SilverSixPort>(Vec(162, 187 + space), module, SwitchedMult::OUT_5_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 222 + space), module, SwitchedMult::OUT_6_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 257 + space), module, SwitchedMult::OUT_7_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 292 + space), module, SwitchedMult::OUT_8_OUTPUT));
	
};
