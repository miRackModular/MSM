#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct RingMod : Module
{
    enum ParamIds {
		CARRIER_A_PARAM,
		MODULATOR_A_PARAM,
		MIXA,
		MIXB,
        NUM_PARAMS
    };
	
    enum InputIds {
        CARRIER_A,
        MODULATOR_A,
		MIXA_CV,
		CARRIER_B,
		MODULATOR_B,
		MIXB_CV,
        NUM_INPUTS
    };
	
    enum OutputIds {
        OUTPUT_A,
		OUTPUT_B,
        NUM_OUTPUTS
    };
	
    enum LightIds
    {
        NUM_LIGHTS
    };

	int Theme = 0;
	
    RingMod() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) 
	{
		
	}
	
    void step() override;

	float carA = 0.0f;
	float modA = 0.0f;
	float carB = 0.0f;
	float modB  = 0.0f;
	
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

void RingMod::step()
{
	carA = inputs[CARRIER_A].value / 5.0;
	modA = inputs[MODULATOR_A].value / 5.0;

	float waveA = clamp(params[MIXA].value + inputs[MIXA_CV].value / 10.0f, 0.0f, 1.0f);
	float ringoutA = carA * modA * 5.0;
	float outA = crossfade(inputs[CARRIER_A].value, ringoutA, waveA);
	
	outputs[OUTPUT_A].value = outA;

	carB = inputs[CARRIER_B].value / 5.0;
	modB = inputs[MODULATOR_B].value / 5.0;
	
	float waveB = clamp(params[MIXB].value + inputs[MIXB_CV].value / 10.0f, 0.0f, 1.0f);
	float ringoutB = carB * modB * 5.0;
	float outB = crossfade(inputs[CARRIER_B].value, ringoutB, waveB);
	
	outputs[OUTPUT_B].value = outB;
};

struct RingModWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	RingModWidget(RingMod *module);
	void step() override;
	
	Menu* createContextMenu() override;
};	

RingModWidget::RingModWidget(RingMod *module) : ModuleWidget(module) {
	box.size = Vec(5 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/RingMod.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/RingMod-Dark.svg")));
	addChild(panelNightMode);
	
    addChild(Widget::create<MScrewB>(Vec(0, 0)));
    addChild(Widget::create<MScrewA>(Vec(box.size.x - 15, 0)));
    addChild(Widget::create<MScrewD>(Vec(0, 365)));
    addChild(Widget::create<MScrewC>(Vec(box.size.x - 15, 365)));

	addParam(ParamWidget::create<GreenSmallKnob>(Vec(22, 98), module, RingMod::MIXA, 0.0f, 1.0f, 0.5f));
	addInput(Port::create<SilverSixPortD>(Vec(5, 144), Port::INPUT, module, RingMod::MIXA_CV));
    addInput(Port::create<SilverSixPortA>(Vec(45, 67), Port::INPUT, module, RingMod::MODULATOR_A));
    addInput(Port::create<SilverSixPort>(Vec(5, 67), Port::INPUT, module, RingMod::CARRIER_A));
    addOutput(Port::create<SilverSixPortC>(Vec(45, 144), Port::OUTPUT, module, RingMod::OUTPUT_A));
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(22, 242), module, RingMod::MIXB, 0.0f, 1.0f, 0.5f));
	addInput(Port::create<SilverSixPortC>(Vec(5, 288), Port::INPUT, module, RingMod::MIXB_CV));
	addInput(Port::create<SilverSixPortE>(Vec(45, 211), Port::INPUT, module, RingMod::MODULATOR_B));
    addInput(Port::create<SilverSixPortB>(Vec(5, 211), Port::INPUT, module, RingMod::CARRIER_B));
    addOutput(Port::create<SilverSixPort>(Vec(45, 288), Port::OUTPUT, module, RingMod::OUTPUT_B));

};

void RingModWidget::step() {
	RingMod *ringmod = dynamic_cast<RingMod*>(module);
	assert(ringmod);
	panelClassic->visible = (ringmod->Theme == 0);
	panelNightMode->visible = (ringmod->Theme == 1);
	ModuleWidget::step();
}

struct RMClassicMenu : MenuItem {
	RingMod *ringmod;
	void onAction(EventAction &e) override {
		ringmod->Theme = 0;
	}
	void step() override {
		rightText = (ringmod->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct RMNightModeMenu : MenuItem {
	RingMod *ringmod;
	void onAction(EventAction &e) override {
		ringmod->Theme = 1;
	}
	void step() override {
		rightText = (ringmod->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* RingModWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	RingMod *ringmod = dynamic_cast<RingMod*>(module);
	assert(ringmod);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<RMClassicMenu>(&RMClassicMenu::text, "Classic (default)", &RMClassicMenu::ringmod, ringmod));
	menu->addChild(construct<RMNightModeMenu>(&RMNightModeMenu::text, "Night Mode", &RMNightModeMenu::ringmod, ringmod));
	return menu;
}

Model *modelRingMod = Model::create<RingMod, RingModWidget>("MSM", "Ring", "Ring", RING_MODULATOR_TAG, EFFECT_TAG);
