#include "MSM.hpp"

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
		
	BlankPanel() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
	{
		
	}
	
	void step() override {}

	void reset() override {}
	
};

BlankPanelWidget::BlankPanelWidget() 
{
	BlankPanel *module = new BlankPanel();
	setModule(module);
	box.size = Vec(120, 380);

    panel = new SVGPanel();
    panel->box.size = box.size;
    panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanel.svg")));
    addChild(panel);
	
	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(90, 0)));
	addChild(createScrew<ScrewSilver>(Vec(90, 365)));
		
};

