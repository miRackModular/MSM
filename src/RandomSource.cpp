#include "MS-Modules.hpp"
#include "dsp/digital.hpp"

struct LowFrequencyOscillator {
	float phase = 0.0;
	float pw = 0.5;
	float freq = 1.0;
	bool offset = false;
	bool invert = false;
	SchmittTrigger resetTrigger;
	LowFrequencyOscillator() {
		resetTrigger.setThresholds(0.0, 0.01);
	}
	void setPitch(float pitch) {
		pitch = fminf(pitch, 12.0);
		freq = powf(24.0, pitch);
	}
	void setPulseWidth(float pw_) {
		const float pwMin = 0.01;
		pw = clampf(pw_, pwMin, 1.0 - pwMin);
	}
	void setReset(float reset) {
		if (resetTrigger.process(reset)) {
			phase = 0.0;
		}
	}
	void step(float dt) {
		float deltaPhase = fminf(freq * dt, 0.5);
		phase += deltaPhase;
		if (phase >= 1.0)
			phase -= 1.0;
	}
	
	float sqr() {
		float sqr = (phase < pw) ^ invert ? 1.0 : -1.0;
		return offset ? sqr + 1.0 : sqr;
	}
	float light() {
		return sinf(2*M_PI * phase);
	}
};

struct RandomSourceV1 : Module {
		enum ParamIds {
		RANGE_PARAM,
		NUM_PARAMS
	};
		
		enum InputIds {
		TRIG_INPUT,
		SH_INPUT,
		NUM_INPUTS
	};
		
		enum OutputIds {
		SH_OUTPUT,
		NUM_OUTPUTS
	};
		
		SchmittTrigger trigger;
		float sample = 0.0;
		float SH_Light = {};
				
		RandomSourceV1() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {
			trigger.setThresholds(0.0,0.7);
		}
		void step() override;
};

void RandomSourceV1::step() {
	
	//sample and hold
	float range = params[RANGE_PARAM].value;
 		
	if (trigger.process(inputs[TRIG_INPUT].value)) {
		sample = inputs[SH_INPUT].normalize(range);
	}
	
	float SHOut = sample * params[RANGE_PARAM].value;
	
	//Output
	outputs[SH_OUTPUT].value = SHOut;

};

RandomSourceV1Widget::RandomSourceV1Widget() {
		RandomSourceV1 *module = new RandomSourceV1();
		setModule(module);
		box.size = Vec(60, 380);
				
		{
			SVGPanel *panel = new SVGPanel();
			panel->box.size = box.size;
			panel->setBackground(SVG::load(assetPlugin(plugin, "/res/Random SourceV1.svg")));
			addChild(panel);
		}
		
		//Screw
		addChild(createScrew<ScrewSilver>(Vec(15, 0)));
		addChild(createScrew<ScrewSilver>(Vec(15, 365)));
		
		//Param
		addParam(createParam<GreyLargeKnob>(Vec(7, 95), module, RandomSourceV1::RANGE_PARAM, 0.0, 1.0, 0.0));
				
		//Inputs
		addInput(createInput<SilverSixPort>(Vec(29, 239), module, RandomSourceV1::SH_INPUT));
		addInput(createInput<SilverSixPort>(Vec(29, 285), module, RandomSourceV1::TRIG_INPUT));
		
		//Outputs
		addOutput(createOutput<SilverSixPort>(Vec(29, 330), module, RandomSourceV1::SH_OUTPUT));
				
};

struct RandomSourceV2 : Module {
		enum ParamIds {
		FREQ_PARAM,
		FM_PARAM,
		OFFSET_PARAM,
		INVERT_PARAM,
		RANGE_PARAM,
		NUM_PARAMS
	};
		
		enum InputIds {
		FM_INPUT,
		SH_INPUT,
		CLK_INPUT,
		NUM_INPUTS
	};
		
		enum OutputIds {
		SH_OUTPUT,
		SQR_OUTPUT,
		NUM_OUTPUTS
	};
		
		enum LightIds {
		NUM_LIGHTS
	};
	
	LowFrequencyOscillator oscillator;
	
		SchmittTrigger trigger;
		float sample = 0.0;
				
		RandomSourceV2() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
			trigger.setThresholds(0.0,0.7);
		}
		void step() override;		
};

void RandomSourceV2::step()	{
	
	//LFO
	oscillator.setPitch(params[FREQ_PARAM].value + params[FM_PARAM].value * inputs[FM_INPUT].value);
	oscillator.offset = (params[OFFSET_PARAM].value > 0.0);
	oscillator.step(0.3 / engineGetSampleRate());

	outputs[SQR_OUTPUT].value = 5.0 * oscillator.sqr();
	
	float LFOout;
	
	if (inputs[CLK_INPUT].active) {
		LFOout = inputs[CLK_INPUT].value;
	}
	else {
		LFOout = outputs[SQR_OUTPUT].value;
	}
	
	//Noise
	float noise2 = 2.0 * randomNormal();
	
	//sample and hold
	float range = noise2 + params[RANGE_PARAM].value;
 		
	if (trigger.process(LFOout)) {
		sample = inputs[SH_INPUT].normalize(range);
	}
	
	float SHOut = sample * params[RANGE_PARAM].value;
	
	//Output
	outputs[SH_OUTPUT].value = SHOut;

};

RandomSourceV2Widget::RandomSourceV2Widget() {
		RandomSourceV2 *module = new RandomSourceV2();
		setModule(module);
		box.size = Vec(120, 380);
				
		{
			SVGPanel *panel = new SVGPanel();
			panel->box.size = box.size;
			panel->setBackground(SVG::load(assetPlugin(plugin, "/res/Random SourceV2.svg")));
			addChild(panel);
		}
		
		//Screw
		addChild(createScrew<ScrewSilver>(Vec(15, 0)));
		addChild(createScrew<ScrewSilver>(Vec(15, 365)));
		addChild(createScrew<ScrewSilver>(Vec(105, 0)));
		addChild(createScrew<ScrewSilver>(Vec(105, 365)));
		
		//Param
		addParam(createParam<GreyLargeKnob>(Vec(9, 95), module, RandomSourceV2::RANGE_PARAM, 0.0, 1.0, 1.0));
		addParam(createParam<GreyLargeKnob>(Vec(9, 150), module, RandomSourceV2::FREQ_PARAM, 0.0, 1.0, 0.5));
		addParam(createParam<GreyLargeKnob>(Vec(62, 123), module, RandomSourceV2::FM_PARAM, 0.0, 1.0, 0.0));
		addParam(createParam<CKSS>(Vec(20, 32), module, RandomSourceV2::OFFSET_PARAM, 0.0, 1.0, 0.0));
						
		//Inputs
		addInput(createInput<SilverSixPort>(Vec(10, 239), module, RandomSourceV2::SH_INPUT));
		addInput(createInput<SilverSixPort>(Vec(10, 285), module, RandomSourceV2::FM_INPUT));
		addInput(createInput<SilverSixPort>(Vec(10, 330), module, RandomSourceV2::CLK_INPUT));
		
		//Outputs
		addOutput(createOutput<SilverSixPort>(Vec(85, 285), module, RandomSourceV2::SH_OUTPUT));
		addOutput(createOutput<SilverSixPort>(Vec(85, 330), module, RandomSourceV2::SQR_OUTPUT));
				
}