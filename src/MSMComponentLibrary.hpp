#pragma once

#include "rack.hpp"
#include "asset.hpp"
#include "widgets.hpp"

/////////////////////////////////////////////////////////////////////////////////////////
//
// COMPONENTS LIBRARY
//
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////
// Knobs
////////////////////

struct MSMLargeKnob : SVGKnob {
	MSMLargeKnob() {
		minAngle = -0.78*M_PI;
		maxAngle = 0.78*M_PI;
		shadow->blurRadius = 4.0;
		shadow->opacity = 0.8;
	}
};


struct GreyLargeKnob : MSMLargeKnob {
	GreyLargeKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreyLargeKnob.svg")));
	}
};

struct BlueLargeKnob : MSMLargeKnob {
	BlueLargeKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/BlueLargeKnob.svg")));
	}
};

struct GreenLargeKnob : MSMLargeKnob {
	GreenLargeKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreenLargeKnob.svg")));
	}
};

struct RedLargeKnob : MSMLargeKnob {
	RedLargeKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/RedLargeKnob.svg")));
	}
};

struct VioLargeKnob : MSMLargeKnob {
	VioLargeKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/VioLargeKnob.svg")));
	}
};

struct MSMSmallKnob : SVGKnob {
	MSMSmallKnob() {
		minAngle = -0.78*M_PI;
		maxAngle = 0.78*M_PI;
		shadow->blurRadius = 2.5;
		shadow->opacity = 1.0;
	}
};

struct GreySmallKnob : MSMSmallKnob {
	GreySmallKnob() {
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreySmallKnob.svg")));
	}
}; 

struct BlueSmallKnob : MSMSmallKnob {
	BlueSmallKnob() {
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/BlueSmallKnob.svg")));
	}
}; 

struct GreenSmallKnob : MSMSmallKnob {
	GreenSmallKnob() {
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreenSmallKnob.svg")));
	}
}; 

struct RedSmallKnob : MSMSmallKnob {
	RedSmallKnob() {
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/RedSmallKnob.svg")));
	}
}; 

struct YellowSmallKnob : MSMSmallKnob {
	YellowSmallKnob() {
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/YellowSmallKnob.svg")));
	}
};

struct GreenTinyKnob : MSMSmallKnob {
	GreenTinyKnob() {
		box.size = Vec(25, 25);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreenTinyKnob.svg")));
	}
}; 

struct MSMToggleKnob : SVGKnob {
	MSMToggleKnob() {
		minAngle = -0.78*M_PI;
		maxAngle = 0.78*M_PI;
		shadow->blurRadius = 2.5;
		shadow->opacity = 1.0;
	}
	
	void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }
};

struct GreenToggleKnobSmall : MSMToggleKnob {
	GreenToggleKnobSmall() {			
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/GreenSmallKnob.svg")));
		snap = true;
		smooth = false;
	}
};

struct RedSmallToggleKnob : MSMToggleKnob {
	RedSmallToggleKnob() {			
		box.size = Vec(32, 32);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/RedSmallKnob.svg")));
		snap = true;
		smooth = false;
	}
};

struct MSMToggle2Knob : SVGKnob {
	MSMToggle2Knob() {
		minAngle = -0.78*M_PI;
		maxAngle = 0.78*M_PI;
		shadow->blurRadius = 4.0;
		shadow->opacity = 0.8;
		snap = true;
		smooth = false;
	}

	void onChange(EventChange &e) override {
        value = round(value);
        SVGKnob::onChange(e);
    }
};


struct RedLargeToggleKnob : MSMToggle2Knob {
	RedLargeToggleKnob() {
		box.size = Vec(47, 47);
		setSVG(SVG::load(assetPlugin(plugin, "res/Knobs/RedLargeKnob.svg")));
		shadow->blurRadius = 4.0;
		shadow->opacity = 0.8;
	}
};

/////////////////////////
// Slider
/////////////////////////

struct MSMSlider : SVGSlider {
	MSMSlider() {
		Vec margin = Vec(3.6, 3.6);
		maxHandlePos = Vec(11, 0.0).plus(margin);
		minHandlePos = Vec(11, 140).plus(margin);
		background->svg = SVG::load(assetPlugin(plugin,"res/Slider/slider.svg"));
		background->wrap();
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
		handle->svg = SVG::load(assetPlugin(plugin, "res/Slider/sliderhandle.svg"));
		handle->wrap();
	}
};

struct MSMSlidePot : SVGSlider {
	MSMSlidePot() {
		Vec margin = Vec(4, 4);
		maxHandlePos = Vec(0.0, -5.0).plus(margin);
		minHandlePos = Vec(0.0, 90.0).plus(margin);
		background->svg = SVG::load(assetPlugin(plugin,"res/Slider/SlidePot.svg"));
		background->wrap();
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
		handle->svg = SVG::load(assetPlugin(plugin, "res/Slider/SlidePotHandle.svg"));
		handle->wrap();
	}
};

/////////////////////////
// Jacks
/////////////////////////

struct MSMPort : SVGPort {
	MSMPort() {
	}
};

struct SilverSixPort : MSMPort {
	SilverSixPort() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPort.svg"));
		background->wrap();
		box.size = background->box.size;
		
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

struct SilverSixPortA : MSMPort {
	SilverSixPortA() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPortA.svg"));
		background->wrap();
		box.size = background->box.size;
		
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

struct SilverSixPortB : MSMPort {
	SilverSixPortB() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPortB.svg"));
		background->wrap();
		box.size = background->box.size;
		
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

struct SilverSixPortC : MSMPort {
	SilverSixPortC() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPortC.svg"));
		background->wrap();
		box.size = background->box.size;
		
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

struct SilverSixPortD : MSMPort {
	SilverSixPortD() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPortD.svg"));
		background->wrap();
		box.size = background->box.size;
	
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

struct SilverSixPortE : MSMPort {
	SilverSixPortE() {
		background->svg = SVG::load(assetPlugin(plugin, "res/Port/SilverSixPortE.svg"));
		background->wrap();
		box.size = background->box.size;
		
		shadow->box.size = background->box.size;
		shadow->box.pos = Vec(0, background->box.size.y * 0.1);
		shadow->blurRadius = 5.0;
		shadow->opacity = 0.75;
	}
	
};

/////////////////////////
// Switch 
/////////////////////////

struct ToggleMe : SVGSwitch, ToggleSwitch {
	ToggleMe() 
	{
		
	}
};

struct ToggleMe2 : SVGSwitch, ToggleSwitch {
	ToggleMe2()
	{
	
	}
	
	void onChange(EventChange &e) override {
		randomizable = false; 
		SVGSwitch::onChange(e);
	}
};

struct MThree : ToggleMe {
	MThree() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_2.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_0.svg")));
	}
};

struct MThree2 : ToggleMe2 {
	MThree2() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_2.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/CKSSThree_0.svg")));
	}
};

struct VioMSwitch : ToggleMe {
	VioMSwitch() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitch_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitch_1.svg")));
	}
};

struct VioMVertSwitch : ToggleMe2 {
	VioMVertSwitch() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitchVert_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitchVert_1.svg")));
	}
};

struct VioM2Switch : ToggleMe2 {
	VioM2Switch() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitch_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/VioMSwitch_1.svg")));
	}
};

struct FMSM : ToggleMe {
	FMSM() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/FMSM_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/FMSM_1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/FMSM_2.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Switch/FMSM_3.svg")));
	}
};

/////////////////////////
// Button
/////////////////////////

struct MSwitchGrey : SVGSwitch, MomentarySwitch {
	MSwitchGrey() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundButton_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundButton_1.svg")));
	}
};

struct MSwitchGrey2 : SVGSwitch, MomentarySwitch {
	MSwitchGrey2() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundButtonBlue_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundButtonBlue_1.svg")));
	}
};

struct MToggleGrey : SVGSwitch, ToggleSwitch {
	MToggleGrey() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundToggle_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/GreyRoundToggle_1.svg")));
	}
};

struct OSCiXEGG : SVGSwitch, ToggleSwitch {
	OSCiXEGG() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/Easteregg_0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/Easteregg_1.svg")));
	}
};

struct MButton : SVGSwitch, ToggleSwitch {
	MButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ButtonGreen0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ButtonGreen1.svg")));
	}
};

struct ThreeColorButton : SVGSwitch, ToggleSwitch {
	ThreeColorButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton2.svg")));
	}
};

struct FiveColorButton : SVGSwitch, ToggleSwitch {
	FiveColorButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton2.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton3.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton4.svg")));
	}
};

struct SixColorButton : SVGSwitch, ToggleSwitch {
	SixColorButton() {
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton0.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton1.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton2.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton3.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton4.svg")));
		addFrame(SVG::load(assetPlugin(plugin, "res/Button/ColorButton5.svg")));
	}
};

////////////////////////
// Screws
////////////////////////

struct MScrewA : SVGScrew {
		MScrewA() {
		sw->setSVG(SVG::load(assetPlugin(plugin, "res/Screws/MScrewA.svg")));
		box.size = sw->box.size;
	}
};

struct MScrewB : SVGScrew {
		MScrewB() {
		sw->setSVG(SVG::load(assetPlugin(plugin, "res/Screws/MScrewB.svg")));
		box.size = sw->box.size;
	}
};

struct MScrewC : SVGScrew {
		MScrewC() {
		sw->setSVG(SVG::load(assetPlugin(plugin, "res/Screws/MScrewC.svg")));
		box.size = sw->box.size;
	}
};

struct MScrewD : SVGScrew {
		MScrewD() {
		sw->setSVG(SVG::load(assetPlugin(plugin, "res/Screws/MScrewD.svg")));
		box.size = sw->box.size;
	}
};

////////////////////////
// Light
////////////////////////

static const NVGcolor MCOLOR_BLUE = nvgRGB(0x16, 0xBC, 0xEB);

struct MGrayModuleLightWidget : ModuleLightWidget {
	MGrayModuleLightWidget() {
		bgColor = nvgRGB(0x5a, 0x5a, 0x5a);
		borderColor = nvgRGBA(0, 0, 0, 0x60);
	}
};

template <typename BASE>
struct MLargeLight : BASE {
	MLargeLight() {
		this->box.size = mm2px(Vec(6.0, 6.0));
	}
};

struct MBlueLight : MGrayModuleLightWidget {
	MBlueLight() {
		addBaseColor(MCOLOR_BLUE);
	}
};

/////////////////////////////////////////////////////////////////////////////////////