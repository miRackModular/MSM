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

	void reset() override {}
	
};

struct BlankPanelWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	BlankPanelWidget(BlankPanel *module);
	void step() override;
	
	Menu* createContextMenu() override;
};	
		
BlankPanelWidget::BlankPanelWidget(BlankPanel *module) : ModuleWidget(module) {
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/BlankPanelLight.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/BlankPanelDark.svg")));
	addChild(panelNightMode);

	addChild(Widget::create<MScrewD>(Vec(15, 0)));
	addChild(Widget::create<MScrewA>(Vec(15, 365)));
	addChild(Widget::create<MScrewC>(Vec(90, 0)));
	addChild(Widget::create<MScrewB>(Vec(90, 365)));
	
};

void BlankPanelWidget::step() {
	BlankPanel *blankpanel = dynamic_cast<BlankPanel*>(module);
	assert(blankpanel);
	panelClassic->visible = (blankpanel->Theme == 0);
	panelNightMode->visible = (blankpanel->Theme == 1);
	ModuleWidget::step();
}

struct BlankClassicMenu : MenuItem {
	BlankPanel *blankpanel;
	void onAction(EventAction &e) override {
		blankpanel->Theme = 0;
	}
	void step() override {
		rightText = (blankpanel->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct BlankNightModeMenu : MenuItem {
	BlankPanel *blankpanel;
	void onAction(EventAction &e) override {
		blankpanel->Theme = 1;
	}
	void step() override {
		rightText = (blankpanel->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* BlankPanelWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	BlankPanel *blankpanel = dynamic_cast<BlankPanel*>(module);
	assert(blankpanel);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<BlankClassicMenu>(&BlankClassicMenu::text, "Classic (default)", &BlankClassicMenu::blankpanel, blankpanel));
	menu->addChild(construct<BlankNightModeMenu>(&BlankNightModeMenu::text, "Night Mode", &BlankNightModeMenu::blankpanel, blankpanel));
	return menu;
}

Model *modelBlankPanel = Model::create<BlankPanel, BlankPanelWidget>("MSM", "Blank Panel", "Blank Panel", BLANK_TAG);