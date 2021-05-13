#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct Mult : Module 
{
		enum ParamIds {
			MODE1_PARAM,
			MODE2_PARAM,
			MODE3_PARAM,
			NUM_PARAMS
		};
		
		enum InputIds {
			IN_1_INPUT,
			IN_2_INPUT,
			IN_3_INPUT,
			IN_4_INPUT,
			IN_5_INPUT,
			IN_6_INPUT,
			NUM_INPUTS
		};
		
		enum OutputIds {
			OUT_11_OUTPUT,
			OUT_12_OUTPUT,
			OUT_13_OUTPUT,
			OUT_14_OUTPUT,
			OUT_15_OUTPUT,
			OUT_16_OUTPUT,
			OUT_21_OUTPUT,
			OUT_22_OUTPUT,
			OUT_23_OUTPUT,
			OUT_24_OUTPUT,
			OUT_25_OUTPUT,
			OUT_26_OUTPUT,
			OUT_31_OUTPUT,
			OUT_32_OUTPUT,
			OUT_33_OUTPUT,
			OUT_34_OUTPUT,
			OUT_35_OUTPUT,
			OUT_36_OUTPUT,
			NUM_OUTPUTS
			
		};
				
		int Theme = 0;
				
		Mult() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
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
		
void Mult::step() {

	double IN_1, IN_2, IN_3;
	
	char IN_1Mode = params[MODE1_PARAM].value > 0.0;
	char IN_2Mode = params[MODE2_PARAM].value > 0.0;
	char IN_3Mode = params[MODE3_PARAM].value > 0.0;
	
	switch(IN_1Mode) {
		case 1:
		IN_1 = inputs[IN_1_INPUT].value + inputs[IN_2_INPUT].value;
		break;
		default:
		IN_1 = inputs[IN_1_INPUT].value - inputs[IN_2_INPUT].value;
		break;
	}
	
	switch(IN_2Mode) {
		case 1:
		IN_2 = inputs[IN_3_INPUT].value + inputs[IN_4_INPUT].value;
		break;
		default:
		IN_2 = inputs[IN_3_INPUT].value - inputs[IN_4_INPUT].value;
		break;
	}
	switch(IN_3Mode) {
		case 1:
		IN_3 = inputs[IN_5_INPUT].value + inputs[IN_6_INPUT].value;
		break;
		default:
		IN_3 = inputs[IN_5_INPUT].value - inputs[IN_6_INPUT].value;
		break;
	}
	
	
	
	//Mult1	
	outputs[OUT_11_OUTPUT].value = IN_1;
	outputs[OUT_12_OUTPUT].value = IN_1;
	outputs[OUT_13_OUTPUT].value = IN_1;
	outputs[OUT_14_OUTPUT].value = IN_1;
	outputs[OUT_15_OUTPUT].value = IN_1;
	outputs[OUT_16_OUTPUT].value = IN_1;
	
	//Mult2	
	outputs[OUT_21_OUTPUT].value = IN_2;
	outputs[OUT_22_OUTPUT].value = IN_2;
	outputs[OUT_23_OUTPUT].value = IN_2;
	outputs[OUT_24_OUTPUT].value = IN_2;
	outputs[OUT_25_OUTPUT].value = IN_2;
	outputs[OUT_26_OUTPUT].value = IN_2;
	
	//Mult3	
	outputs[OUT_31_OUTPUT].value = IN_3;
	outputs[OUT_32_OUTPUT].value = IN_3;
	outputs[OUT_33_OUTPUT].value = IN_3;
	outputs[OUT_34_OUTPUT].value = IN_3;
	outputs[OUT_35_OUTPUT].value = IN_3;
	outputs[OUT_36_OUTPUT].value = IN_3;
		
};

struct MultWidget : ModuleWidget {
	MultWidget(Mult *module);
};

MultWidget::MultWidget(Mult *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/Mult.svg")));

    addChild(Widget::create<MScrewA>(Vec(15, 0)));
    addChild(Widget::create<MScrewA>(Vec(box.size.x - 30, 0)));
    addChild(Widget::create<MScrewB>(Vec(15, 365)));
    addChild(Widget::create<MScrewD>(Vec(box.size.x - 30, 365)));

	addInput(Port::create<SilverSixPortC>(Vec(15, 30), Port::INPUT, module, Mult::IN_1_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(15, 60), Port::INPUT, module, Mult::IN_2_INPUT));
	addInput(Port::create<SilverSixPortE>(Vec(55, 30), Port::INPUT, module, Mult::IN_3_INPUT));
	addInput(Port::create<SilverSixPortA>(Vec(55, 60), Port::INPUT, module, Mult::IN_4_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(95, 30), Port::INPUT, module, Mult::IN_5_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(95, 60), Port::INPUT, module, Mult::IN_6_INPUT));
	
	addParam(ParamWidget::create<VioMSwitch>(Vec(20, 94), module, Mult::MODE1_PARAM, 0.0, 1.0, 1.0));
	addParam(ParamWidget::create<VioMSwitch>(Vec(60, 94), module, Mult::MODE2_PARAM, 0.0, 1.0, 1.0));
	addParam(ParamWidget::create<VioMSwitch>(Vec(100, 94), module, Mult::MODE3_PARAM, 0.0, 1.0, 1.0));
	
	addOutput(Port::create<SilverSixPortA>(Vec(15, 120), Port::OUTPUT, module, Mult::OUT_11_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(15, 160), Port::OUTPUT, module, Mult::OUT_12_OUTPUT));
	addOutput(Port::create<SilverSixPortC>(Vec(15, 200), Port::OUTPUT, module, Mult::OUT_13_OUTPUT));
	addOutput(Port::create<SilverSixPortB>(Vec(15, 240), Port::OUTPUT, module, Mult::OUT_14_OUTPUT));
	addOutput(Port::create<SilverSixPortA>(Vec(15, 280), Port::OUTPUT, module, Mult::OUT_15_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(15, 320), Port::OUTPUT, module, Mult::OUT_16_OUTPUT));

	addOutput(Port::create<SilverSixPortE>(Vec(55, 120), Port::OUTPUT, module, Mult::OUT_21_OUTPUT));
	addOutput(Port::create<SilverSixPortD>(Vec(55, 160), Port::OUTPUT, module, Mult::OUT_22_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(55, 200), Port::OUTPUT, module, Mult::OUT_23_OUTPUT));
	addOutput(Port::create<SilverSixPortB>(Vec(55, 240), Port::OUTPUT, module, Mult::OUT_24_OUTPUT));
	addOutput(Port::create<SilverSixPortE>(Vec(55, 280), Port::OUTPUT, module, Mult::OUT_25_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(55, 320), Port::OUTPUT, module, Mult::OUT_26_OUTPUT));
	
	addOutput(Port::create<SilverSixPort>(Vec(95, 120), Port::OUTPUT, module, Mult::OUT_31_OUTPUT));
	addOutput(Port::create<SilverSixPortA>(Vec(95, 160), Port::OUTPUT, module, Mult::OUT_32_OUTPUT));
	addOutput(Port::create<SilverSixPortE>(Vec(95, 200), Port::OUTPUT, module, Mult::OUT_33_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(95, 240), Port::OUTPUT, module, Mult::OUT_34_OUTPUT));
	addOutput(Port::create<SilverSixPortC>(Vec(95, 280), Port::OUTPUT, module, Mult::OUT_35_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(95, 320), Port::OUTPUT, module, Mult::OUT_36_OUTPUT));
};

Model *modelMult = Model::create<Mult, MultWidget>("MSM", "Mult", "Mult", MULTIPLE_TAG);
