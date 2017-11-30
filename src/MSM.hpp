#include "rack.hpp"
#include "app.hpp"
#include "dsp/digital.hpp"

using namespace rack;

extern Plugin *plugin;

////////////////////
// Knob
////////////////////

struct GreyKnob : SVGKnob {
	GreyKnob() {
		minAngle = -0.83*M_PI;
		maxAngle = 0.83*M_PI;
	}
};


struct GreyLargeKnob : GreyKnob {
	GreyLargeKnob() {
		box.size = Vec(46, 46);
		setSVG(SVG::load(assetPlugin(plugin, "res/GreyLargeKnob.svg")));
	}
}; 

struct GreySmallKnob : GreyKnob {
	GreySmallKnob() {
		box.size = Vec(30, 30);
		setSVG(SVG::load(assetPlugin(plugin, "res/GreySmallKnob.svg")));
	}
}; 

/////////////////////////
// Jacks
/////////////////////////

struct SilverSixPort : SVGPort {
	SilverSixPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/SilverSixPort.svg"));
		background->wrap();
		box.size = background->box.size;
	}
	
};

/////////////////////////
// Button
/////////////////////////

struct SwitchVert : SVGSwitch, ToggleSwitch {
	SwitchVert() {
		addFrame(SVG::load(assetPlugin(plugin, "res/SwitchVert0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/SwitchVert1.svg")));
		addFrame(SVG::load(assetPlugin(plugin,"res/SwitchVert2.svg")));
	}
};

////////////////////
// module widgets
////////////////////


struct RandomSourceV1Widget : ModuleWidget {
	RandomSourceV1Widget();
};

struct RandomSourceV2Widget : ModuleWidget {
	RandomSourceV2Widget();
};

struct MultWidget : ModuleWidget {
	MultWidget();
};

struct SwitchedMultWidget : ModuleWidget {
	SwitchedMultWidget();
};

struct BlankPanelWidget : ModuleWidget {
	BlankPanelWidget();
};
