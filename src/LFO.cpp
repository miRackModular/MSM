#include "MSM.hpp"

//////////////////////////////////////////////////////////////
// This LFO is based on the LFO from the Fundamental package// 
//////////////////////////////////////////////////////////////

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
		pitch = fminf(pitch, 8.0);
		freq = powf(2.0, pitch);
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
	float sin() {
		if (offset)
			return 1.0 - cosf(2*M_PI * phase) * (invert ? -1.0 : 1.0);
		else
			return sinf(2*M_PI * phase) * (invert ? -1.0 : 1.0);
	}
	float tri(float x) {
		return 4.0 * fabsf(x - roundf(x));
	}
	float tri() {
		if (offset)
			return tri(invert ? phase - 0.5 : phase);
		else
			return -1.0 + tri(invert ? phase - 0.25 : phase - 0.75);
	}
	float saw(float x) {
		return 2.0 * (x - roundf(x));
	}
	float saw() {
		if (offset)
			return invert ? 2.0 * (1.0 - phase) : 2.0 * phase;
		else
			return saw(phase) * (invert ? -1.0 : 1.0);
	}
	float sqr() {
		float sqr = (phase < pw) ^ invert ? 1.0 : -1.0;
		return offset ? sqr + 1.0 : sqr;
	}
	
};

struct LowFrequencyOscillator1 {
	float phase = 0.0;
	float pw = 0.5;
	float freq = 1.0;
	bool offset = false;
	bool invert = false;
	SchmittTrigger resetTrigger;
	LowFrequencyOscillator1() {
		resetTrigger.setThresholds(0.0, 0.01);
	}
	void setPitch(float pitch) {
		pitch = fminf(pitch, 8.0);
		freq = powf(2.0, pitch);
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
	float sin() {
		if (offset)
			return 1.0 - cosf(2*M_PI * phase) * (invert ? -1.0 : 1.0);
		else
			return sinf(2*M_PI * phase) * (invert ? -1.0 : 1.0);
	}
	float tri(float x) {
		return 4.0 * fabsf(x - roundf(x));
	}
	float tri() {
		if (offset)
			return tri(invert ? phase - 0.5 : phase);
		else
			return -1.0 + tri(invert ? phase - 0.25 : phase - 0.75);
	}
	float saw(float x) {
		return 2.0 * (x - roundf(x));
	}
	float saw() {
		if (offset)
			return invert ? 2.0 * (1.0 - phase) : 2.0 * phase;
		else
			return saw(phase) * (invert ? -1.0 : 1.0);
	}
	float sqr() {
		float sqr = (phase < pw) ^ invert ? 1.0 : -1.0;
		return offset ? sqr + 1.0 : sqr;
	}
	
};

struct LFO : Module {
	
	enum ParamIds 
	{
		SYNCED_PARAM,
		FREQ_PARAM,
		FM_PARAM,
		PW_1_PARAM,
		PWM_1_PARAM,
		OFFSET_1_PARAM,
		INVERT_1_PARAM,
		WAVE_A_PARAM,
		FM_2_PARAM,
		PW_2_PARAM,
		PWM_2_PARAM,
		OFFSET_2_PARAM,
		INVERT_2_PARAM,
		WAVE_B_PARAM,
		MIX_PARAM,
		
		NUM_PARAMS
		
	};
	enum InputIds 
	{
		FM_1_INPUT,
		PW_1_INPUT,
		RESET_INPUT,
		WAVE_A_INPUT,
		FM_2_INPUT,
		PW_2_INPUT,
		WAVE_B_INPUT,
		CV_MIX_INPUT,
		NUM_INPUTS
	};
	enum OutputIds 
	{
		SIN_OUTPUT,
		TRI_OUTPUT,
		SAW_OUTPUT,
		SQR_OUTPUT,
		SIN_2_OUTPUT,
		TRI_2_OUTPUT,
		SAW_2_OUTPUT,
		SQR_2_OUTPUT,
		LFO_A_OUTPUT,
		LFO_B_OUTPUT,
		OUT_MIX_OUTPUT,
		NUM_OUTPUTS
	};
	
	LowFrequencyOscillator oscillator;
	LowFrequencyOscillator1	oscillatorB;
	
	LFO() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) 
	{
		
	}
	void step() override;
	
};

void LFO::step() 
{
	//LFO 1
	
	oscillator.setPitch(params[FREQ_PARAM].value + inputs[FM_1_INPUT].value * params[FM_PARAM].value);
	oscillator.setPulseWidth(params[PW_1_PARAM].value + inputs[PW_1_INPUT].value);
	oscillator.offset = (params[OFFSET_1_PARAM].value > 0.0);
	oscillator.invert = (params[INVERT_1_PARAM].value <= 0.0);
	oscillator.step(1.0 / engineGetSampleRate());
	oscillator.setReset(inputs[RESET_INPUT].value);

	outputs[SIN_OUTPUT].value = 5.0 * oscillator.sin();
	outputs[TRI_OUTPUT].value = 5.0 * oscillator.tri();
	outputs[SAW_OUTPUT].value = 5.0 * oscillator.saw();
	outputs[SQR_OUTPUT].value = 5.0 * oscillator.sqr();
	
	float WAVE_A = params[WAVE_A_PARAM].value + inputs[WAVE_A_INPUT].value;
	WAVE_A = clampf(WAVE_A, 0.0, 3.0);
	float WAVE_1_OUT;
	if (WAVE_A < 1.0)
		WAVE_1_OUT = crossf(oscillator.sin(), oscillator.tri(), WAVE_A);
	else if (WAVE_A < 2.0)
		WAVE_1_OUT = crossf(oscillator.tri(), oscillator.saw(), WAVE_A - 1.0);
	else
		WAVE_1_OUT = crossf(oscillator.saw(), oscillator.sqr(), WAVE_A - 2.0);
	outputs[LFO_A_OUTPUT].value = 5.0 * WAVE_1_OUT;
	
			
	//LFO 2
	
	oscillatorB.setPitch(params[FREQ_PARAM].value + inputs[FM_2_INPUT].value * params[FM_2_PARAM].value);
	oscillatorB.setPulseWidth(params[PW_2_PARAM].value + inputs[PW_2_INPUT].value);
	oscillatorB.offset = (params[OFFSET_2_PARAM].value > 0.0);
	oscillatorB.invert = (params[INVERT_2_PARAM].value <= 0.0);
	oscillatorB.step(1.0 / engineGetSampleRate());
	oscillatorB.setReset(inputs[RESET_INPUT].value);

	outputs[SIN_2_OUTPUT].value = 5.0 * oscillatorB.sin();
	outputs[TRI_2_OUTPUT].value = 5.0 * oscillatorB.tri();
	outputs[SAW_2_OUTPUT].value = 5.0 * oscillatorB.saw();
	outputs[SQR_2_OUTPUT].value = 5.0 * oscillatorB.sqr();
	
	float WAVE_B = params[WAVE_B_PARAM].value + inputs[WAVE_B_INPUT].value;
	WAVE_B = clampf(WAVE_B, 0.0, 3.0);
	float WAVE_2_OUT;
	if (WAVE_B < 1.0)
		WAVE_2_OUT = crossf(oscillatorB.sin(), oscillatorB.tri(), WAVE_B);
	else if (WAVE_B < 2.0)
		WAVE_2_OUT = crossf(oscillatorB.tri(), oscillatorB.saw(), WAVE_B - 1.0);
	else
		WAVE_2_OUT = crossf(oscillatorB.saw(), oscillatorB.sqr(), WAVE_B - 2.0);
	outputs[LFO_B_OUTPUT].value = 5.0 * WAVE_2_OUT;
	
	
	//MIX LFO 1 & LFO 2
	float CrossfadeMix = clampf(params[MIX_PARAM].value + inputs[CV_MIX_INPUT].value, 0.0, 1.0);
	float MIX_IN_1 = outputs[LFO_A_OUTPUT].value;
	float MIX_IN_2 = outputs[LFO_B_OUTPUT].value;
	float OutMix;
	
	if(CrossfadeMix < 0.5) {
		OutMix = crossf(MIX_IN_1, MIX_IN_1, CrossfadeMix);
	}
	else(CrossfadeMix > 1.0); 
		OutMix = crossf(MIX_IN_1, MIX_IN_2, CrossfadeMix);
	outputs[OUT_MIX_OUTPUT].value = OutMix;
};

LFOWidget::LFOWidget() {
	
	LFO *module = new LFO();
	setModule(module);
	box.size = Vec(240, 380);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/LFO.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 365)));
	
	//LFO 1
	addParam(createParam<CKSS>(Vec(70, 150), module, LFO::OFFSET_1_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(70, 215), module, LFO::INVERT_1_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<GreyLargeKnob>(Vec(100, 40), module, LFO::FREQ_PARAM, -8.0, 6.0, -1.0));
	addParam(createParam<GreySmallKnob>(Vec(11, 142), module, LFO::FM_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreySmallKnob>(Vec(11, 207), module, LFO::PW_1_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<SilverSixPort>(Vec(40, 256), module, LFO::FM_1_INPUT));
	addInput(createInput<SilverSixPort>(Vec(80, 256), module, LFO::RESET_INPUT));
	addInput(createInput<SilverSixPort>(Vec(11, 298), module, LFO::WAVE_A_INPUT));
	addOutput(createOutput<SilverSixPort>(Vec(54, 298), module, LFO::LFO_A_OUTPUT));
	
	addOutput(createOutput<SilverSixPort>(Vec(35, 338), module, LFO::SIN_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(69, 338), module, LFO::TRI_OUTPUT));


	
	//LFO 2 
	addParam(createParam<CKSS>(Vec(162, 150), module, LFO::OFFSET_2_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<CKSS>(Vec(162, 215), module, LFO::INVERT_2_PARAM, 0.0, 1.0, 1.0));
	addParam(createParam<GreySmallKnob>(Vec(199, 142), module, LFO::FM_2_PARAM, 0.0, 1.0, 0.0));
	addParam(createParam<GreySmallKnob>(Vec(199, 207), module, LFO::PW_2_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<SilverSixPort>(Vec(180, 256), module, LFO::FM_2_INPUT));
	addInput(createInput<SilverSixPort>(Vec(206, 298), module, LFO::WAVE_B_INPUT));
	addOutput(createOutput<SilverSixPort>(Vec(162, 298), module, LFO::LFO_B_OUTPUT));
	
	addOutput(createOutput<SilverSixPort>(Vec(151, 338), module, LFO::SAW_2_OUTPUT));
	addOutput(createOutput<SilverSixPort>(Vec(185, 338), module, LFO::SQR_2_OUTPUT));
	
	//Wave Selectors
	addParam(createParam<GreySmallKnob>(Vec(40, 50), module, LFO::WAVE_A_PARAM, 0.0, 3.0, 0.0));
	addParam(createParam<GreySmallKnob>(Vec(175, 50), module, LFO::WAVE_B_PARAM, 0.0, 3.0, 0.0));
	
	//LFO MIX
	addParam(createParam<GreySmallKnob>(Vec(108, 105), module, LFO::MIX_PARAM, 0.0, 1.0, 0.5));
	addInput(createInput<SilverSixPort>(Vec(140, 256), module, LFO::CV_MIX_INPUT));
	addOutput(createOutput<SilverSixPort>(Vec(110, 298), module, LFO::OUT_MIX_OUTPUT));
};