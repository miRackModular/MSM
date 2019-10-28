#include "MSM.hpp"
#include "Additional/Resources.hpp"
#include "MSMComponentLibrary.hpp"

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
	
	int Theme = 0;
	
	Fade() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)	{}
	
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

void Fade::step()
{
	double CrossfadeA = clamp(params[CF_A_PARAM].value + inputs[CVA_INPUT].value, 0.0f, 1.0f);
	double IN_1 = inputs[IN_1_INPUT].value;
	double IN_2 = inputs[IN_2_INPUT].value;
	double OutA;
	
	if(CrossfadeA < 0.5f) {
		OutA = crossfade(IN_1, IN_1, CrossfadeA);
	}
	else(CrossfadeA > 1.0f); 
		OutA = crossfade(IN_1, IN_2, CrossfadeA);
	outputs[OUT_A_OUTPUT].value = OutA;
	
	
	double CrossfadeB = clamp(params[CF_B_PARAM].value + inputs[CVB_INPUT].value, 0.0f, 1.0f);
	double IN_3 = inputs[IN_3_INPUT].value;
	double IN_4 = inputs[IN_4_INPUT].value;
	double OutB;
	
	if(CrossfadeB < 0.5f) {
		OutB = crossfade(IN_3, IN_3, CrossfadeB);
	}
	else(CrossfadeB > 1.0f);
		OutB = crossfade(IN_3, IN_4, CrossfadeB);
	outputs[OUT_B_OUTPUT].value = OutB;
	
	
	double CrossfadeAB = clamp(params[CF_AB_PARAM].value + inputs[CVAB_INPUT].value, 0.0f, 1.0f);
	double IN_A = OutA;
	double IN_B = OutB;
	double OutAB;
	
	if(CrossfadeAB < 0.5f) {
		OutAB = crossfade(IN_A, IN_A, CrossfadeAB);
	}
	else(CrossfadeAB > 1.0f); 
		OutAB = crossfade(IN_A, IN_B, CrossfadeAB);
	outputs[OUT_AB_OUTPUT].value = OutAB;
	
	
};

struct FadeWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	FadeWidget(Fade *module);
	void step() override;
	
	Menu* createContextMenu() override;
};	

FadeWidget::FadeWidget(Fade *module) : ModuleWidget(module) {
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Fade.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Fade-Dark.svg")));
	addChild(panelNightMode);
	
	addChild(Widget::create<MScrewB>(Vec(15, 0)));
	addChild(Widget::create<MScrewA>(Vec(15, 365)));
	addChild(Widget::create<MScrewC>(Vec(90, 0)));
	addChild(Widget::create<MScrewD>(Vec(90, 365)));
	
	//Params
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(10, 65), module, Fade::CF_A_PARAM, 0.0, 1.0, 0.5));
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(64, 65), module, Fade::CF_B_PARAM, 0.0, 1.0, 0.5));
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(38, 110), module, Fade::CF_AB_PARAM, 0.0, 1.0, 0.5));

	//Inputs
	addInput(Port::create<SilverSixPortA>(Vec(10, 240), Port::INPUT, module, Fade::IN_1_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(49, 240), Port::INPUT, module, Fade::IN_2_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(10, 280), Port::INPUT, module, Fade::IN_3_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(49, 280), Port::INPUT, module, Fade::IN_4_INPUT));
		
	addInput(Port::create<SilverSixPortD>(Vec(10, 320), Port::INPUT, module, Fade::CVA_INPUT));
	addInput(Port::create<SilverSixPortE>(Vec(49, 320), Port::INPUT, module, Fade::CVB_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(49, 200), Port::INPUT, module, Fade::CVAB_INPUT));

	//Outputs
	addOutput(Port::create<SilverSixPortA>(Vec(88, 240), Port::OUTPUT, module, Fade::OUT_A_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(88, 280), Port::OUTPUT, module, Fade::OUT_B_OUTPUT));
	addOutput(Port::create<SilverSixPortC>(Vec(88, 320), Port::OUTPUT, module, Fade::OUT_AB_OUTPUT));	
};

void FadeWidget::step() {
	Fade *fade = dynamic_cast<Fade*>(module);
	assert(fade);
	panelClassic->visible = (fade->Theme == 0);
	panelNightMode->visible = (fade->Theme == 1);
	ModuleWidget::step();
}

struct FadeClassicMenu : MenuItem {
	Fade *fade;
	void onAction(EventAction &e) override {
		fade->Theme = 0;
	}
	void step() override {
		rightText = (fade->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct FadekNightModeMenu : MenuItem {
	Fade *fade;
	void onAction(EventAction &e) override {
		fade->Theme = 1;
	}
	void step() override {
		rightText = (fade->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* FadeWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	Fade *fade = dynamic_cast<Fade*>(module);
	assert(fade);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<FadeClassicMenu>(&FadeClassicMenu::text, "Classic (default)", &FadeClassicMenu::fade, fade));
	menu->addChild(construct<FadekNightModeMenu>(&FadekNightModeMenu::text, "Night Mode", &FadekNightModeMenu::fade, fade));
	return menu;
}

Model *modelFade = Model::create<Fade, FadeWidget>("MSM", "Fade", "Fade", MIXER_TAG);