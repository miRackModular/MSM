#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct BlankPanel : Module {
	
	enum ParamIds {
		NUM_PARAMS
	};
	
	enum InputIds {
		NUM_INPUTS
	};
	
	enum OutputIds {
		NUM_OUTPUTS
	};
	
	 enum LightIds {
        NUM_LIGHTS
	};

	// Panel Theme
	int Theme = 0;	
	
	BlankPanel() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
	{
		
	}
	
	void step() override {}
	
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

	void onReset() override {}
	
};

struct BlankPanelWidget : ModuleWidget {
	BlankPanelWidget(BlankPanel *module);
};	
		
BlankPanelWidget::BlankPanelWidget(BlankPanel *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/BlankPanelLight.svg")));

	addChild(Widget::create<MScrewD>(Vec(15, 0)));
	addChild(Widget::create<MScrewA>(Vec(15, 365)));
	addChild(Widget::create<MScrewC>(Vec(90, 0)));
	addChild(Widget::create<MScrewB>(Vec(90, 365)));
	
};

Model *modelBlankPanel = Model::create<BlankPanel, BlankPanelWidget>("MSM", "Blank Panel", "Blank Panel", BLANK_TAG);