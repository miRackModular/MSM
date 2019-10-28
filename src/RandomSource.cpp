#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

struct RandomSource : Module {
		enum ParamIds {
		RANGE_PARAM,
		RANGE_CV_PARAM,
		SWITCH_PARAM,
		SLEW_PARAM,
		SHAPE_PARAM,
		NUM_PARAMS
	};
		
		enum InputIds {
		TRIG_INPUT,
		SH_INPUT,
		SLEW_CV,
		RANGE_CV_INPUT,
		NUM_INPUTS
	};
		
		enum OutputIds {
		SH_OUTPUT,
		SLEWED_OUT,
		NUM_OUTPUTS
	};
		
		SchmittTrigger trigger;
		double sample = 0.0;
		double out = 0.0;
		
		int Theme = 0;
		
		RandomSource() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
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

void RandomSource::step() {
	
	//sample and hold
	double range = params[RANGE_PARAM].value + params[RANGE_CV_PARAM].value * (inputs[RANGE_CV_INPUT].value / 5.0f);
 	double Switch = params[SWITCH_PARAM].value;
	
	if (trigger.process(inputs[TRIG_INPUT].value)) {
		if(Switch > 0.0) {
		sample = inputs[SH_INPUT].normalize(range) + 5.0;
		}
		else {
			sample = inputs[SH_INPUT].normalize(range);
		}
	}
	
	double SHOut = (sample ? sample : 0.0f) * range;
	
	// S&H Output
	outputs[SH_OUTPUT].value = saturate(SHOut);	
	
	//slew limiter
	double in = outputs[SH_OUTPUT].value;
	double shape = params[SHAPE_PARAM].value;

	const double slewMin = 0.1;
	const double slewMax = 10000.0;
	const double shapeScale = 1/10.0;

	// Rise
	if (in > out) {
		double rise = inputs[SLEW_CV].value / 10.0 + params[SLEW_PARAM].value;
		double slew = slewMax * powf(slewMin / slewMax, rise);
		out += slew * crossfade(1.0f, shapeScale * (in - out), shape) * engineGetSampleTime();
		if (out > in)
			out = in;
	}
	// Fall
	else if (in < out) {
		double fall = inputs[SLEW_CV].value / 10.0 + params[SLEW_PARAM].value;
		double slew = slewMax * powf(slewMin / slewMax, fall);
		out -= slew * crossfade(1.0f, shapeScale * (out - in), shape) * engineGetSampleTime();
		if (out < in)
			out = in;
	}

	outputs[SLEWED_OUT].value = saturate(out);
};

struct RandomSourceWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	RandomSourceWidget(RandomSource *module);
	void step() override;
	
	Menu* createContextMenu() override;
};

RandomSourceWidget::RandomSourceWidget(RandomSource *module) : ModuleWidget(module) {
	box.size = Vec(8 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Random Source.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Random Source-Dark.svg")));
	addChild(panelNightMode);
	
	//Screw
	addChild(Widget::create<MScrewB>(Vec(15, 0)));
	addChild(Widget::create<MScrewC>(Vec(15, 365)));
	addChild(Widget::create<MScrewD>(Vec(box.size.x - 30, 0)));
	addChild(Widget::create<MScrewA>(Vec(box.size.x - 30, 365)));
		
	//Param
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(7, 75), module, RandomSource::RANGE_PARAM, 0.0, 1.0, 0.0));
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(65, 115), module, RandomSource::SLEW_PARAM, 0.0, 1.0, 0.5));
	addParam(ParamWidget::create<GreenLargeKnob>(Vec(7, 165), module, RandomSource::SHAPE_PARAM, 0.0, 1.0, 0.5));
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(73, 189), module, RandomSource::RANGE_CV_PARAM, 0.0, 1.0, 0.0));
		
	addParam(ParamWidget::create<VioMSwitch>(Vec(82.5, 65), module, RandomSource::SWITCH_PARAM, 0.0, 1.0, 0.0));
				
	//Inputs
	addInput(Port::create<SilverSixPortA>(Vec(18, 289.5), Port::INPUT, module, RandomSource::SH_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(18, 327.5), Port::INPUT, module, RandomSource::TRIG_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(18, 251.5), Port::INPUT, module, RandomSource::SLEW_CV));
	addInput(Port::create<SilverSixPortC>(Vec(80, 251.5), Port::INPUT, module, RandomSource::RANGE_CV_INPUT));
	
	//Outputs
	addOutput(Port::create<SilverSixPortB>(Vec(80, 289.5), Port::OUTPUT, module, RandomSource::SH_OUTPUT));
	addOutput(Port::create<SilverSixPort>(Vec(80, 327.5), Port::OUTPUT, module, RandomSource::SLEWED_OUT));
	
};

void RandomSourceWidget::step() {
	RandomSource *randomsource = dynamic_cast<RandomSource*>(module);
	assert(randomsource);
	panelClassic->visible = (randomsource->Theme == 0);
	panelNightMode->visible = (randomsource->Theme == 1);
	ModuleWidget::step();
}

struct RandomSClassicMenu : MenuItem {
	RandomSource *randomsource;
	void onAction(EventAction &e) override {
		randomsource->Theme = 0;
	}
	void step() override {
		rightText = (randomsource->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct RandomSNightModeMenu : MenuItem {
	RandomSource *randomsource;
	void onAction(EventAction &e) override {
		randomsource->Theme = 1;
	}
	void step() override {
		rightText = (randomsource->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* RandomSourceWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	RandomSource *randomsource = dynamic_cast<RandomSource*>(module);
	assert(randomsource);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<RandomSClassicMenu>(&RandomSClassicMenu::text, "Classic (default)", &RandomSClassicMenu::randomsource, randomsource));
	menu->addChild(construct<RandomSNightModeMenu>(&RandomSNightModeMenu::text, "Night Mode", &RandomSNightModeMenu::randomsource, randomsource));
	return menu;
}

Model *modelRandomSource = Model::create<RandomSource, RandomSourceWidget>("MSM", "Random Source", "Random Source", SAMPLE_AND_HOLD_TAG, RANDOM_TAG);