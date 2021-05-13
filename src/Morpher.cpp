#include "MSM.hpp"
#include "Additional/Resources.hpp"
#include "MSMComponentLibrary.hpp"

struct Morpher : Module
{
    enum ParamIds {
		MORPH_PARAM,
		SWITCHCOUNT,
        NUM_PARAMS
    };
	
    enum InputIds {
        INPUT1,
        INPUT2,
		INPUT3,
		INPUT4,
		INPUT5,
        INPUT6,
		INPUT7,
		INPUT8,
		MORPH_CV,
        NUM_INPUTS
    };
	
    enum OutputIds {
        OUTPUT,
        NUM_OUTPUTS
    };
	
    enum LightIds {
		LIGHT_1,
		LIGHT_2,
		LIGHT_3,
		LIGHT_4,
		LIGHT_5,
		LIGHT_6,
		LIGHT_7,
		LIGHT_8,
        NUM_LIGHTS
    };

	int Theme = 0;
	
	double Crossfader = 0.0f;
	double In1 = 0.0f;
	double In2 = 0.0f;
	double In3 = 0.0f;
	double In4 = 0.0f;
	double In5 = 0.0f;
	double In6 = 0.0f;
	double In7 = 0.0f;
	double In8 = 0.0f;
	double Out = 0.0f ? 0.0f : 0.0f;
	
	int switchcount = 0.0f;
	
    Morpher() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) 
	{
		
	}
	
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
		
	void onReset() override {
		switchcount = 0;
	}
	
	void onRandomize() override {
		switchcount = randomu32() % 7;
	}
			
};

void Morpher::step() {

	In1 = inputs[INPUT1].active ? inputs[INPUT1].value : 0.0f;
	In2 = inputs[INPUT2].active ? inputs[INPUT2].value : 0.0f;
	In3 = inputs[INPUT3].active ? inputs[INPUT3].value : 0.0f;
	In4 = inputs[INPUT4].active ? inputs[INPUT4].value : 0.0f;
	In5 = inputs[INPUT5].active ? inputs[INPUT5].value : 0.0f;
	In6 = inputs[INPUT6].active ? inputs[INPUT6].value : 0.0f;
	In7 = inputs[INPUT7].active ? inputs[INPUT7].value : 0.0f;
	In8 = inputs[INPUT8].active ? inputs[INPUT8].value : 0.0f;
	
	switchcount = params[SWITCHCOUNT].value;

	switch(switchcount) {
		case 6:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 7.0f);
		Crossfader *= 7.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else if(Crossfader < 2.0f)
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			else if(Crossfader < 3.0f)
				Out = crossfade(In3, In4, Crossfader - 2.0f);
			else if(Crossfader < 4.0f)
				Out = crossfade(In4, In5, Crossfader - 3.0f);
			else if(Crossfader < 5.0f)
				Out = crossfade(In5, In6, Crossfader - 4.0f);
			else if(Crossfader < 6.0f)
				Out = crossfade(In6, In7, Crossfader - 5.0f);
			else
				Out = crossfade(In7, In8, Crossfader - 6.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 1.0f;
			lights[LIGHT_5].value = 1.0f;
			lights[LIGHT_6].value = 1.0f;
			lights[LIGHT_7].value = 1.0f;
			lights[LIGHT_8].value = 1.0f;
		break;
		case 5:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 6.0f);
		Crossfader *= 6.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else if(Crossfader < 2.0f)
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			else if(Crossfader < 3.0f)
				Out = crossfade(In3, In4, Crossfader - 2.0f);
			else if(Crossfader < 4.0f)
				Out = crossfade(In4, In5, Crossfader - 3.0f);
			else if(Crossfader < 5.0f)
				Out = crossfade(In5, In6, Crossfader - 4.0f);
			else
				Out = crossfade(In6, In7, Crossfader - 5.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 1.0f;
			lights[LIGHT_5].value = 1.0f;
			lights[LIGHT_6].value = 1.0f;
			lights[LIGHT_7].value = 1.0f;
			lights[LIGHT_8].value = 0.0f;
		break;
		case 4:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 5.0f);
		Crossfader *= 5.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else if(Crossfader < 2.0f)
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			else if(Crossfader < 3.0f)
				Out = crossfade(In3, In4, Crossfader - 2.0f);
			else if(Crossfader < 4.0f)
				Out = crossfade(In4, In5, Crossfader - 3.0f);
			else
				Out = crossfade(In5, In6, Crossfader - 4.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 1.0f;
			lights[LIGHT_5].value = 1.0f;
			lights[LIGHT_6].value = 1.0f;
			lights[LIGHT_7].value = 0.0f;
			lights[LIGHT_8].value = 0.0f;
		break;;
		case 3:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 4.0f);
		Crossfader *= 4.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else if(Crossfader < 2.0f)
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			else if(Crossfader < 3.0f)
				Out = crossfade(In3, In4, Crossfader - 2.0f);
			else
				Out = crossfade(In4, In5, Crossfader - 3.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 1.0f;
			lights[LIGHT_5].value = 1.0f;
			lights[LIGHT_6].value = 0.0f;
			lights[LIGHT_7].value = 0.0f;
			lights[LIGHT_8].value = 0.0f;
		break;
		case 2:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 3.0f);
		Crossfader *= 3.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else if(Crossfader < 2.0f)
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			else
				Out = crossfade(In3, In4, Crossfader - 2.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 1.0f;
			lights[LIGHT_5].value = 0.0f;
			lights[LIGHT_6].value = 0.0f;
			lights[LIGHT_7].value = 0.0f;
			lights[LIGHT_8].value = 0.0f;
		break;
		case 1:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 2.0f);
		Crossfader *= 2.0f;
		
			if(Crossfader < 1.0f) 
				Out = crossfade(In1, In2, Crossfader);
			else 
				Out = crossfade(In2, In3, Crossfader - 1.0f);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 1.0f;
			lights[LIGHT_4].value = 0.0f;
			lights[LIGHT_5].value = 0.0f;
			lights[LIGHT_6].value = 0.0f;
			lights[LIGHT_7].value = 0.0f;
			lights[LIGHT_8].value = 0.0f;
		break;
		case 0:
		Crossfader = clamp(params[MORPH_PARAM].value + inputs[MORPH_CV].value / 10.0f, 0.0f, 1.0f);
		Crossfader *= 1.0f;
			Out = crossfade(In1, In2, Crossfader);
			outputs[OUTPUT].value = Out;
			
			lights[LIGHT_1].value = 1.0f;
			lights[LIGHT_2].value = 1.0f;
			lights[LIGHT_3].value = 0.0f;
			lights[LIGHT_4].value = 0.0f;
			lights[LIGHT_5].value = 0.0f;
			lights[LIGHT_6].value = 0.0f;
			lights[LIGHT_7].value = 0.0f;
			lights[LIGHT_8].value = 0.0f;
		break;
	}
	
	
	
};

struct MorpherWidget : ModuleWidget {
	MorpherWidget(Morpher *module);
};	

MorpherWidget::MorpherWidget(Morpher *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/Morpher.svg")));
	
    addChild(Widget::create<MScrewB>(Vec(0, 0)));
    addChild(Widget::create<MScrewA>(Vec(box.size.x - 15, 0)));
    addChild(Widget::create<MScrewC>(Vec(0, 365)));
    addChild(Widget::create<MScrewD>(Vec(box.size.x - 15, 365)));
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(30, 75), module, Morpher::MORPH_PARAM, 0.0, 1.0, 0.0));
	
	addParam(ParamWidget::create<GreenToggleKnobSmall>(Vec(31, 35), module, Morpher::SWITCHCOUNT, 0.0, 6.0, 0.0));
		
    addInput(Port::create<SilverSixPortA>(Vec(14, 128), Port::INPUT, module, Morpher::INPUT1));
    addInput(Port::create<SilverSixPort>(Vec(52, 128), Port::INPUT, module, Morpher::INPUT2));
	addInput(Port::create<SilverSixPortB>(Vec(14, 178), Port::INPUT, module, Morpher::INPUT3));
    addInput(Port::create<SilverSixPortD>(Vec(52, 178), Port::INPUT, module, Morpher::INPUT4));
	addInput(Port::create<SilverSixPortA>(Vec(14, 228), Port::INPUT, module, Morpher::INPUT5));
    addInput(Port::create<SilverSixPort>(Vec(52, 228), Port::INPUT, module, Morpher::INPUT6));
	addInput(Port::create<SilverSixPortB>(Vec(14, 278), Port::INPUT, module, Morpher::INPUT7));
    addInput(Port::create<SilverSixPortD>(Vec(52, 278), Port::INPUT, module, Morpher::INPUT8));
	
	addInput(Port::create<SilverSixPort>(Vec(14, 328), Port::INPUT, module, Morpher::MORPH_CV));
	
    addOutput(Port::create<SilverSixPortE>(Vec(52, 328), Port::OUTPUT, module, Morpher::OUTPUT));
	
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(24, 120), module, Morpher::LIGHT_1));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(62, 120), module, Morpher::LIGHT_2));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(24, 170), module, Morpher::LIGHT_3));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(62, 170), module, Morpher::LIGHT_4));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(24, 220), module, Morpher::LIGHT_5));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(62, 220), module, Morpher::LIGHT_6));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(24, 270), module, Morpher::LIGHT_7));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(62, 270), module, Morpher::LIGHT_8));
};

Model *modelMorpher = Model::create<Morpher, MorpherWidget>("MSM", "Morph", "Morph", EFFECT_TAG);
