#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct xseq : Module {

	enum ParamIds {
		BUT1_PARAM,
		BUT2_PARAM,
		BUT3_PARAM,
		BUT4_PARAM,
		BUT5_PARAM,
		BUT6_PARAM,
		BUT7_PARAM,
		BUT8_PARAM,
		BUT9_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		
		B1_INPUT,
		B2_INPUT,
		B3_INPUT,
		B4_INPUT,
		B5_INPUT,
		B6_INPUT,
		B7_INPUT,
		B8_INPUT,
		B9_INPUT,
		
		NUM_INPUTS
	};
	
	enum OutputIds {
		AND1_OUTPUT,
		AND2_OUTPUT,
		OR1_OUTPUT,
		OR2_OUTPUT,
		NOR1_OUTPUT,
		NOR2_OUTPUT,
		NAND1_OUTPUT,
		NAND2_OUTPUT,
		XOR1_OUTPUT,
		XNOR1_OUTPUT,
		NOT1_OUTPUT,
		NOT2_OUTPUT,
		NOT3_OUTPUT,
		NUM_OUTPUTS
	};
	
	enum LightIds {
		NOR1_LIGHT,
		NAND1_LIGHT,
		OR1_LIGHT,
		NAND2_LIGHT,
		AND1_LIGHT,
		AND2_LIGHT,
		XOR1_LIGHT,
		NOR2_LIGHT,
		XNOR1_LIGHT,
		NUM_LIGHTS
	};

	bool IN_1 = false;
	bool IN_2 = false;
	bool IN_3 = false;
	SchmittTrigger trigger1;
	SchmittTrigger trigger2;
	SchmittTrigger trigger3;
	
	int Theme = 0;
	
	xseq() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
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

void xseq::step() {
	
	bool NOR1 = true, NOR2 = false, NAND1 = false, NAND2 = false, OR1 = false, XOR1 = false, XNOR1 = true, NOT1 = false, NOT2 = false, NOT3 = false, AND1 = false, AND2 = false;
	
	char Button1 = 0.0f, Button2 = 0.0f, Button3 = 0.0f, Button4 = 0.0f, Button5 = 0.0f, Button6 = 0.0f, Button7 = 0.0f, Button8 = 0.0f, Button9 = false;
	
	IN_1 = trigger1.process(inputs[IN1_INPUT].value >= 10.0f);
	IN_2 = trigger2.process(inputs[IN2_INPUT].value >= 10.0f);
	IN_3 = trigger3.process(inputs[IN3_INPUT].value >= 10.0f);
	
	//////////////////////////////////////////////////////////////////////
	
	// NOR 1 or AND
	
	IN_1 = false;
	IN_2 = false;
	//NOR1 = true;

		
	if(inputs[IN1_INPUT].active) {
		IN_1 = inputs[IN1_INPUT].value;
	}
		
	if(inputs[IN2_INPUT].active) {
		IN_2 = inputs[IN2_INPUT].value;
	}
	
	
	Button1 = clamp(params[BUT1_PARAM].value + quadraticBipolar(inputs[B1_INPUT].value) / 10.0f, -1.0f, 1.0f);

	switch(Button1) {
		case 1:
			NOR1 = IN_1 && IN_2;		// <- AND
		break;
		case 0:
			NOR1 = !(IN_1 || IN_2);		// <- NOR
		break;
	};
	
	//Output
	if(outputs[NOR1_OUTPUT].active)
       outputs[NOR1_OUTPUT].value =  10.0 * NOR1;
   lights[NOR1_LIGHT].value = 10.0 * NOR1;
   
	//////////////////////////////////////////////////////////////////////
	
	// NAND 1 or OR
		
	IN_1 = false;
	IN_2 = false;
	//NAND1 = false;
	
	if(inputs[IN1_INPUT].active) {
		IN_1 = inputs[IN1_INPUT].value;
	}
		
	if(inputs[IN3_INPUT].active) {
		IN_2 = inputs[IN3_INPUT].value;
	}
	
	Button2 = clamp(params[BUT2_PARAM].value + quadraticBipolar(inputs[B2_INPUT].value) / 10.0f, -1.0f, 1.0f);
		
	switch(Button2) {
		case 1:
			NAND1 = IN_1 || IN_2;		// <- OR
		break;
		case 0:
			NAND1 = !(IN_1 && IN_2);	// <- NAND
		break;
	}
	
	//Output
	if(outputs[NAND1_OUTPUT].active)
	outputs[NAND1_OUTPUT].value =  10.0 * NAND1;
   lights[NAND1_LIGHT].value = 10.0 * NAND1;

	//////////////////////////////////////////////////////////////////////
	
	// NOT 1
	
	IN_1 = false;
	IN_2 = false;
	//NOT1 = false;
		
	if(NOR1) {
		IN_2 = NOR1;		
	}
		
	NOT1 = !(IN_3); // <- NOT
		
	//Output
	if(outputs[NOT1_OUTPUT].active)
       outputs[NOT1_OUTPUT].value = 10.0 * NOT1;
   
	//////////////////////////////////////////////////////////////////////
	
	// OR 1 or AND
	
	IN_1 = false;
	IN_2 = false;
	//OR1 = false;
		
	if(NOR1) {
		IN_1 = NOR1;
	}
	
	if(NOT1) {
		IN_2 = NOT1;
	}
	
	
	Button3 = clamp(params[BUT3_PARAM].value + quadraticBipolar(inputs[B3_INPUT].value) / 10.0f, -1.0f, 1.0f);
	
	switch(Button3) {
		case 1:
			OR1 = !(IN_1 && IN_2);		// <- NAND
		break;
		case 0:
			OR1 = !(IN_1 ^ IN_2);	// <- XNOR
		break;
	}
	
	//Output
	if(outputs[OR1_OUTPUT].active)
       outputs[OR1_OUTPUT].value = 10.0 * OR1;
   lights[OR1_LIGHT].value = 10.0 * OR1;
   
   	//////////////////////////////////////////////////////////////////////
	
	// XOR 2 or NAND
	
	IN_1 = false;
	IN_2 = false;
	//NAND2 = false;
		
	if(NOT1) {
		IN_1 = NOT1;
	}
	
	if(OR1) {
		IN_2 = OR1;
	}

	Button4 = clamp(params[BUT4_PARAM].value + quadraticBipolar(inputs[B4_INPUT].value) / 10.0f, -1.0f, 1.0f);
		
	switch(Button4) {
		case 1:
		NAND2 = !(IN_1 && IN_2);		// <- NAND
		break;
		case 0:
			NAND2 = !(IN_1 ^ IN_2);		// <- XNOR
		break;

	}
		
	//Output
	if(outputs[NAND2_OUTPUT].active)
       outputs[NAND2_OUTPUT].value = 10 * NAND2;
   lights[NAND2_LIGHT].value = 10.0 * NAND2;
   
   	//////////////////////////////////////////////////////////////////////
	
	// NOT 2
	
	IN_1 = false;
	IN_2 = false;
	//NOT2 = false;
		
	if(NAND1) {
		IN_2 = NAND1;		
	}
		
	NOT2 = !(IN_3); // <- NOT
				
	//Output
	if(outputs[NOT2_OUTPUT].active)
       outputs[NOT2_OUTPUT].value = 10.0 * NOT2;

	//////////////////////////////////////////////////////////////////////
		
	// AND 1 or NAND
	
	IN_1 = false;
	IN_2 = true;
	//AND1 = false;
		
	if(NOT2) {
		IN_1 = NOT2;
	}
	
	if(inputs[IN2_INPUT].active) {
		IN_2 = inputs[IN2_INPUT].value;
	}
	
	Button5 = clamp(params[BUT5_PARAM].value + quadraticBipolar(inputs[B5_INPUT].value) / 10.0f, -1.0f, 1.0f);
	
	switch(Button5) {
		case 1:
			AND1 = !(IN_1 && IN_2);			// <- NAND
		break;
		case 0:
			AND1 = IN_1 && IN_2;		// <- AND
		break;
	}
		
	//Output
	if(outputs[AND1_OUTPUT].active)
		outputs[AND1_OUTPUT].value = 10.0 * AND1;
   lights[AND1_LIGHT].value = 10.0 * AND1;
   
	//////////////////////////////////////////////////////////////////////
	
	// NOT 3

	IN_1 = false;
	IN_2 = false;
	//NOT3 = false;
		
	if(NAND2) {
		IN_2 = NAND2;		
	}
		
	NOT3 = !(IN_3); // <- NOT
				
	//Output
	if(outputs[NOT3_OUTPUT].active)
       outputs[NOT3_OUTPUT].value = 10.0 * NOT3;
   
	//////////////////////////////////////////////////////////////////////
	
	// AND 2
		
	IN_1 = false;
	IN_2 = true;
	//AND2 = false;
		
	if(NOT3) {
		IN_1 = NOT3;
	}
	
	if(inputs[IN2_INPUT].active) {
		IN_2 = inputs[IN2_INPUT].value;
	}
	
	Button6 = clamp(params[BUT6_PARAM].value + quadraticBipolar(inputs[B6_INPUT].value) / 10.0f, -1.0f, 1.0f);
		
	switch(Button6) {
		case 1:
			AND2 = !(IN_1 && IN_2) && (IN_1 != IN_2);		// <- NAND AND OR
		break;
		case 0:
			AND2 = IN_1 && IN_2;						// <- AND
		break;
	}
	
	//Output
	if(outputs[AND2_OUTPUT].active)
		outputs[AND2_OUTPUT].value = 10.0 * AND2;
   lights[AND2_LIGHT].value = 10.0 * AND2;
	
	//////////////////////////////////////////////////////////////////////
	
	// XOR 1
	
	IN_1 = false;
	IN_2 = true;
	//XOR1 = false;
		
	if(AND2) {
		IN_1 = AND2;
	}
	
	if(inputs[IN3_INPUT].active) {
		IN_2 = inputs[IN3_INPUT].value;
	}
	
	Button7 = clamp(params[BUT7_PARAM].value + quadraticBipolar(inputs[B7_INPUT].value) / 10.0f, -1.0f, 1.0f);
	
	switch(Button7) {
		case 1:
			XOR1 = !(IN_1 ^ IN_2); // <- XNOR
		break;
		case 0:
			XOR1 = (IN_1 ^ IN_2);  // <- XOR
		break;
	}
	
	//Output
	if(outputs[XOR1_OUTPUT].active)
		outputs[XOR1_OUTPUT].value = 10.0 * XOR1;
   lights[XOR1_LIGHT].value = 10.0 * XOR1;
   
	//////////////////////////////////////////////////////////////////////
	
	// NOR 2
	
	IN_1 = true;
	IN_2 = false;
	//NOR2 = false;
		
		
	if(AND2) {
		IN_1 = AND2;
	}
		
	if(OR1) {
		IN_2 = OR1;
	}
	
	Button8 = clamp(params[BUT8_PARAM].value + quadraticBipolar(inputs[B8_INPUT].value) / 10.0f, -1.0f, 1.0f);
	
	switch(Button8) {
		case 1:
			NOR2 = IN_1 ^ IN_2;								// <- XOR
		break;
		case 0:
			NOR2 = ((IN_1 && IN_2) || (!IN_1 && IN_3)); // <- AND - OR - AND
		break;
	}
		
	//Output
	if(outputs[NOR2_OUTPUT].active)
       outputs[NOR2_OUTPUT].value =  10.0 * NOR2;
   lights[NOR2_LIGHT].value = 10.0 * NOR2;
   
	//////////////////////////////////////////////////////////////////////
	
	// XNOR 1
	
	IN_1 = false;
	IN_2 = false;
	//XNOR1 = true;
	
	if(inputs[IN1_INPUT].active) {
		IN_1 = inputs[IN1_INPUT].value;
	}
	
	if(XOR1) {
		IN_2 = XOR1;
	}
	
	Button9 = clamp(params[BUT9_PARAM].value + quadraticBipolar(inputs[B9_INPUT].value) / 10.0f, -1.0f, 1.0f);
	
	switch(Button9) {
		case 1:
			XNOR1 = !(IN_2);								// <- Not In 2
		break;
		case 0:
			XNOR1 = ((IN_1 && IN_2) || (!IN_1 && !IN_3));	// <- AND - OR - AND
		break;
	}
	
	//Output
	if(outputs[XNOR1_OUTPUT].active)
		outputs[XNOR1_OUTPUT].value = 10.0 * XNOR1;
   lights[XNOR1_LIGHT].value = 10.0 * XNOR1;
   
	//////////////////////////////////////////////////////////////////////


	
};

struct xseqWidget : ModuleWidget {
	xseqWidget(xseq *module);
};

xseqWidget::xseqWidget(xseq *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/XSEQ-Expension.svg")));

	addChild(Widget::create<MScrewA>(Vec(15, 0)));
	addChild(Widget::create<MScrewB>(Vec(15, 365)));
	addChild(Widget::create<MScrewD>(Vec(box.size.x-90, 0)));
	addChild(Widget::create<MScrewC>(Vec(box.size.x-90, 365)));
	
	addChild(Widget::create<MScrewC>(Vec(box.size.x-15, 0)));
	addChild(Widget::create<MScrewD>(Vec(box.size.x-60, 365)));
		
	addParam(ParamWidget::create<MToggleGrey>(Vec(19,85), module, xseq::BUT1_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(77,85), module, xseq::BUT2_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(137,85), module, xseq::BUT3_PARAM, 0.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<MToggleGrey>(Vec(19,175), module, xseq::BUT4_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(77,175), module, xseq::BUT5_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(137,175), module, xseq::BUT6_PARAM, 0.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<MToggleGrey>(Vec(19,265), module, xseq::BUT7_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(77,265), module, xseq::BUT8_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<MToggleGrey>(Vec(137,265), module, xseq::BUT9_PARAM, 0.0, 1.0, 0.0));
	
	addInput(Port::create<SilverSixPortB>(Vec(20, 45), Port::INPUT, module, xseq::IN1_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(78, 45), Port::INPUT, module, xseq::IN2_INPUT));
	addInput(Port::create<SilverSixPortA>(Vec(138, 45), Port::INPUT, module, xseq::IN3_INPUT));
	
	addOutput(Port::create<SilverSixPort>(Vec(20, 120), Port::OUTPUT, module, xseq::NOR1_OUTPUT));
	addOutput(Port::create<SilverSixPortC>(Vec(78, 120), Port::OUTPUT, module, xseq::NAND1_OUTPUT));
	addOutput(Port::create<SilverSixPortE>(Vec(138, 120), Port::OUTPUT, module, xseq::OR1_OUTPUT));
	
	addOutput(Port::create<SilverSixPortB>(Vec(20, 210), Port::OUTPUT, module, xseq::NAND2_OUTPUT));
	addOutput(Port::create<SilverSixPortA>(Vec(78, 210), Port::OUTPUT, module, xseq::AND1_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(138, 210), Port::OUTPUT, module, xseq::AND2_OUTPUT));
	
	addOutput(Port::create<SilverSixPortC>(Vec(20, 300), Port::OUTPUT, module, xseq::XOR1_OUTPUT));
	addOutput(Port::create<SilverSixPortA>(Vec(78, 300), Port::OUTPUT, module, xseq::NOR2_OUTPUT));
	addOutput(Port::create<SilverSixPortB>(Vec(138, 300), Port::OUTPUT, module, xseq::XNOR1_OUTPUT));
	
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(28, 150), module, xseq::NOR1_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(86, 150), module, xseq::NAND1_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(146, 150), module, xseq::OR1_LIGHT));
	
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(28, 240), module, xseq::NAND2_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(86, 240), module, xseq::AND1_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(146, 240), module, xseq::AND2_LIGHT));
	
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(28, 330), module, xseq::XOR1_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(86, 330), module, xseq::NOR2_LIGHT));
	addChild(ModuleLightWidget::create<MediumLight<GreenLight>>(Vec(146, 330), module, xseq::XNOR1_LIGHT));
	
	addInput(Port::create<SilverSixPortB>(Vec(197, 20), Port::INPUT, module, xseq::B1_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(197, 55), Port::INPUT, module, xseq::B2_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(197, 90), Port::INPUT, module, xseq::B3_INPUT));
	
	addInput(Port::create<SilverSixPortB>(Vec(197, 125), Port::INPUT, module, xseq::B4_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(197, 160), Port::INPUT, module, xseq::B5_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(197, 195), Port::INPUT, module, xseq::B6_INPUT));
	
	addInput(Port::create<SilverSixPortB>(Vec(197, 230), Port::INPUT, module, xseq::B7_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(197, 265), Port::INPUT, module, xseq::B8_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(197, 300), Port::INPUT, module, xseq::B9_INPUT));
};

Model *modelxseq = Model::create<xseq, xseqWidget>("MSM", "XSEQ", "XSEQ", LOGIC_TAG, RANDOM_TAG);