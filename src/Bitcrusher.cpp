#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"

class BitcrusherFX {
	public:
		BitcrusherFX()
		{
			reset();
			quant = powf(2.0f, 32.0f);
			counter = 0.0f;
			_drive = 1.0f;
			input = 0.0f;
		}
		
		void reset() {
			output = 0.0f;
		}
		
		float process(float in, long int Bits, float drive, float sample_rate)	{
				// Overdrive
				_drive = drive;
				input = overdrive(in, _drive);
				//Bit-reduction
				quant = std::pow(2.0, Bits);
				
				counter += sample_rate;
				if (counter >= 1.0f) {
					counter -= 1.0f;
					output = ROUND((input + 1.0) * quant) / quant - 1.0;
				}
		return output;
		}
		
		
	private:
		float quant;
		float _drive;
		float counter;
		float input;		
		float output;
};

struct Bitcrusher : Module {
	
	enum ParamIds {
		BITS_PARAM,
		QD_PARAM,
		SR_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		INPUT,
		CV_BITS,
		NUM_INPUTS
	};
	
	enum OutputIds {
		OUTPUT,
		NUM_OUTPUTS
	};
	
	 enum LightIds {
        NUM_LIGHTS
	};
		
	// Panel Theme
	int Theme = 0;	
	
	float out = 0.0f ? 0.0f : 0.0f;
	
	BitcrusherFX BitC;
		
	Bitcrusher() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
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

	void reset() override {
		BitC.reset();
	}
	
};

void Bitcrusher::step() {
	
	float in = inputs[INPUT].value / 5.0f;
	long int bits = params[BITS_PARAM].value + inputs[CV_BITS].value;
	float qd = params[QD_PARAM].value;
	float sr = params[SR_PARAM].value;
		
	out = 5.0f * BitC.process(in, bits, qd, sr);
	
	outputs[OUTPUT].value = saturate(out);
	
};

struct BitcrusherWidget : ModuleWidget {
	// Panel Themes
	SVGPanel *panelClassic;
	SVGPanel *panelNightMode;
	
	BitcrusherWidget(Bitcrusher *module);
	void step() override;
	
	Menu* createContextMenu() override;
};	
		
BitcrusherWidget::BitcrusherWidget(Bitcrusher *module) : ModuleWidget(module) {
	box.size = Vec(4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);
	panelClassic = new SVGPanel();
	panelClassic->box.size = box.size;
	panelClassic->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Bitcrusher.svg")));
	addChild(panelClassic);
	
	panelNightMode = new SVGPanel();
	panelNightMode->box.size = box.size;
	panelNightMode->setBackground(SVG::load(assetPlugin(plugin, "res/Panels/Bitcrusher-Dark.svg")));
	addChild(panelNightMode);
	
	addChild(Widget::create<MScrewD>(Vec(0, 0)));
	addChild(Widget::create<MScrewA>(Vec(0, 365)));
	addChild(Widget::create<MScrewC>(Vec(45, 0)));
	addChild(Widget::create<MScrewB>(Vec(45, 365)));
	
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 48), module, Bitcrusher::BITS_PARAM, 0.0, 8.0, 8.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 98), module, Bitcrusher::SR_PARAM, 0.01, 1.0, 1.0));
	addParam(ParamWidget::create<GreenSmallKnob>(Vec(15, 148), module, Bitcrusher::QD_PARAM, 0.0, 0.95, 0.0));
	
	addInput(Port::create<SilverSixPortA>(Vec(18, 210), Port::INPUT, module, Bitcrusher::INPUT));
	addInput(Port::create<SilverSixPort>(Vec(18, 250), Port::INPUT, module, Bitcrusher::CV_BITS));
	
	addOutput(Port::create<SilverSixPortB>(Vec(18, 290), Port::OUTPUT, module, Bitcrusher::OUTPUT));

};

void BitcrusherWidget::step() {
	Bitcrusher *bitcrusher = dynamic_cast<Bitcrusher*>(module);
	assert(bitcrusher);
	panelClassic->visible = (bitcrusher->Theme == 0);
	panelNightMode->visible = (bitcrusher->Theme == 1);
	ModuleWidget::step();
}

struct BitCClassicMenu : MenuItem {
	Bitcrusher *bitcrusher;
	void onAction(EventAction &e) override {
		bitcrusher->Theme = 0;
	}
	void step() override {
		rightText = (bitcrusher->Theme == 0) ? "✔" : "";
		MenuItem::step();
	}
};

struct BitCNightModeMenu : MenuItem {
	Bitcrusher *bitcrusher;
	void onAction(EventAction &e) override {
		bitcrusher->Theme = 1;
	}
	void step() override {
		rightText = (bitcrusher->Theme == 1) ? "✔" : "";
		MenuItem::step();
	}
};

Menu* BitcrusherWidget::createContextMenu() {
	Menu* menu = ModuleWidget::createContextMenu();
	Bitcrusher *bitcrusher = dynamic_cast<Bitcrusher*>(module);
	assert(bitcrusher);
	menu->addChild(construct<MenuEntry>());
	menu->addChild(construct<MenuLabel>(&MenuLabel::text, "Theme"));
	menu->addChild(construct<BitCClassicMenu>(&BitCClassicMenu::text, "Classic (default)", &BitCClassicMenu::bitcrusher, bitcrusher));
	menu->addChild(construct<BitCNightModeMenu>(&BitCNightModeMenu::text, "Night Mode", &BitCNightModeMenu::bitcrusher, bitcrusher));
	return menu;
}

Model *modelBitcrusher = Model::create<Bitcrusher, BitcrusherWidget>("MSM", "Bitcrusher", "Bitcrusher", EFFECT_TAG);