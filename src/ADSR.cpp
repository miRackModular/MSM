#include "MSM.hpp"
#include "MSMComponentLibrary.hpp"
#include "Additional/EnvelopeGenerator.hpp"

struct ADSR : Module {
	enum ParamIds {
		ATTACK_PARAM,
		DECAY_PARAM,
		SUSTAIN_PARAM,
		RELEASE_PARAM,
		ATT_SHAPE,
		DEC_SHAPE,
		REL_SHAPE,
		NUM_PARAMS
	};
	enum InputIds {
		ATTACK_INPUT,
		DECAY_INPUT,
		SUSTAIN_INPUT,
		RELEASE_INPUT,
		GATE_INPUT,
		TRIG_INPUT,
		DEC_SHAPE_CV,
		NUM_INPUTS
	};
	enum OutputIds {
		ENVELOPE_OUTPUT,
		INV_OUTPUT,
		EOA,
		EOD,
		EOS,
		EOR,
		NUM_OUTPUTS
	};
	enum LightIds {
		ATTACK_LIGHT,
		DECAY_LIGHT,
		SUSTAIN_LIGHT,
		RELEASE_LIGHT,
		NUM_LIGHTS
	};
	
	const int modulationSteps = 100;
	int _modulationStep = 0;
	//char _linearMode = 0;
	SchmittTrigger _gateTrigger;
	SchmittTrigger _retrigTrigger;
	EnvelopeGen _envelope;
	
	PulseGenerator _triggerAttack;
	PulseGenerator _triggerDecay;
	PulseGenerator _triggerSustain;
	PulseGenerator _triggerRelease;
	
	float attackshape = 0.0f;
	float decayshape = 0.0f;
	float releaseshape = 0.0f;
	
	// Panel Theme
	int Theme = 0;
	
	ADSR() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {
		onReset();
		onSampleRateChange();
	}

	void onReset() override;
	void onSampleRateChange() override;
	void step() override;
	
	//Json for Panel Theme
	json_t *toJson() override	{
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "Theme", json_integer(Theme));
		//json_object_set_new(rootJ, "Linear", json_integer(_linearMode));
		return rootJ;
	}
	void fromJson(json_t *rootJ) override	{
		json_t *ThemeJ = json_object_get(rootJ, "Theme");
		if (ThemeJ)
			Theme = json_integer_value(ThemeJ);
	
		//json_t *LinJ = json_object_get(rootJ, "Linear");
		//if (LinJ)
		//	_linearMode = json_integer_value(LinJ);
	}
	
};

void ADSR::onReset() {
	_gateTrigger.reset();
	_envelope.reset();
	//_linearMode = 0;
	_modulationStep = modulationSteps;
};

void ADSR::onSampleRateChange() {
	_envelope.setSampleRate(engineGetSampleRate());
	_modulationStep = modulationSteps;
};

void ADSR::step() {

	++_modulationStep;
	if (_modulationStep >= modulationSteps) {
		_modulationStep = 0;

		_envelope.setAttack(params[ATTACK_PARAM].value + clamp(inputs[ATTACK_INPUT].value / 10.0f, 0.0f, 1.0f));
		_envelope.setDecay(params[DECAY_PARAM].value + clamp(inputs[DECAY_INPUT].value / 10.0f, 0.0f, 1.0f));
		_envelope.setSustain(params[SUSTAIN_PARAM].value, inputs[SUSTAIN_INPUT].value);
		_envelope.setRelease(params[RELEASE_PARAM].value + clamp(inputs[RELEASE_INPUT].value / 10.0f, 0.0f, 1.0f));
		/*
		_envelope.setAttack(powf(params[ATTACK_PARAM].value + clamp(inputs[ATTACK_INPUT].value / 10.0f, 0.0f, 1.0f), 2.0f) * 10.f);
		_envelope.setDecay(powf(params[DECAY_PARAM].value + clamp(inputs[DECAY_INPUT].value / 10.0f, 0.0f, 1.0f), 2.0f) * 10.f);
		_envelope.setSustain(params[SUSTAIN_PARAM].value + clamp(inputs[SUSTAIN_INPUT].value / 10.0f, 0.0f, 1.0f));
		_envelope.setRelease(powf(params[RELEASE_PARAM].value + clamp(inputs[RELEASE_INPUT].value / 10.0f, 0.0f, 1.0f), 2.0f) * 10.f);
		*/
		
		attackshape = clamp(params[ATT_SHAPE].value, 0.1f, 4.0f);
		decayshape = clamp(params[DEC_SHAPE].value + inputs[DEC_SHAPE_CV].value / 2.5f, 0.1f, 4.0f);
		releaseshape = clamp(params[REL_SHAPE].value, 0.1f, 4.0f);
		_envelope.setShapes(attackshape, decayshape, releaseshape);
	}

	_gateTrigger.process(inputs[GATE_INPUT].value);
	_retrigTrigger.process(inputs[TRIG_INPUT].value);
	_envelope.setGate(_gateTrigger.isHigh(), _retrigTrigger.isHigh());


	outputs[ENVELOPE_OUTPUT].value = _envelope.next() * 10.0f;
	outputs[INV_OUTPUT].value = 1.0f - (_envelope.next() * 10.0f) + 9.0f;
	

	lights[ATTACK_LIGHT].value = _envelope.isStage(EnvelopeGen::ATTACK_STAGE);
	if(!_envelope.isStage(EnvelopeGen::ATTACK_STAGE)) {
		_triggerAttack.trigger(0.01f);
	}
	outputs[EOA].value = _triggerAttack.process(1.0f / engineGetSampleRate()) ? 10.0 : 0.0;

	lights[DECAY_LIGHT].value = _envelope.isStage(EnvelopeGen::DECAY_STAGE);
	if(!_envelope.isStage(EnvelopeGen::DECAY_STAGE)) {
		_triggerDecay.trigger(0.01f);
	}
	outputs[EOD].value = _triggerDecay.process(1.0f / engineGetSampleRate()) ? 10.0 : 0.0;
	
	lights[SUSTAIN_LIGHT].value = _envelope.isStage(EnvelopeGen::SUSTAIN_STAGE);
	if(!_envelope.isStage(EnvelopeGen::SUSTAIN_STAGE)) {
		_triggerSustain.trigger(0.01f);
	}
	outputs[EOS].value = _triggerSustain.process(1.0f / engineGetSampleRate()) ? 10.0 : 0.0;

	lights[RELEASE_LIGHT].value = _envelope.isStage(EnvelopeGen::RELEASE_STAGE);
	if(!_envelope.isStage(EnvelopeGen::RELEASE_STAGE)) {
		_triggerRelease.trigger(0.01f);
	}
	outputs[EOR].value = _triggerRelease.process(1.0f / engineGetSampleRate()) ? 10.0 : 0.0;


};


struct ADSRWidget : ModuleWidget {
	ADSRWidget(ADSR *module);
};

ADSRWidget::ADSRWidget(ADSR *module) : ModuleWidget(module) {
	setPanel(SVG::load(assetPlugin(plugin, "res/Panels/ADSR.svg")));
	
	addChild(Widget::create<MScrewA>(Vec(15, 0)));
	addChild(Widget::create<MScrewD>(Vec(box.size.x-30, 0)));
	addChild(Widget::create<MScrewC>(Vec(15, 365)));
	addChild(Widget::create<MScrewB>(Vec(box.size.x-30, 365)));
	
	addParam(ParamWidget::create<MSMSlidePot>(Vec(8, 55), module, ADSR::ATTACK_PARAM, 0.0f, 1.0f, 0.1f));
	addParam(ParamWidget::create<MSMSlidePot>(Vec(38, 55), module, ADSR::DECAY_PARAM, 0.0f, 1.0f, 0.2f));
	addParam(ParamWidget::create<MSMSlidePot>(Vec(71, 55), module, ADSR::SUSTAIN_PARAM, 0.0f, 1.0f, 0.5f));
	addParam(ParamWidget::create<MSMSlidePot>(Vec(101, 55), module, ADSR::RELEASE_PARAM, 0.0f, 1.0f, 0.15f));
	
	
	addParam(ParamWidget::create<GreenTinyKnob>(Vec(9, 205), module, ADSR::ATT_SHAPE, 0.1f, 4.0f, 1.0f));
	addParam(ParamWidget::create<GreenTinyKnob>(Vec(40, 205), module, ADSR::DEC_SHAPE, 0.1f, 4.0f, 1.0f));
	addInput(Port::create<SilverSixPortA>(Vec(71, 205), Port::INPUT, module, ADSR::DEC_SHAPE_CV));
	addParam(ParamWidget::create<GreenTinyKnob>(Vec(102, 205), module, ADSR::REL_SHAPE, 0.1f, 4.0f, 1.0f));
	
	addInput(Port::create<SilverSixPortA>(Vec(9, 251.5), Port::INPUT, module, ADSR::ATTACK_INPUT));
	addInput(Port::create<SilverSixPortD>(Vec(40, 251.5), Port::INPUT, module, ADSR::DECAY_INPUT));
	addInput(Port::create<SilverSixPort>(Vec(72, 251.5), Port::INPUT, module, ADSR::SUSTAIN_INPUT));
	addInput(Port::create<SilverSixPortB>(Vec(102, 251.5), Port::INPUT, module, ADSR::RELEASE_INPUT));
	
	addOutput(Port::create<SilverSixPortC>(Vec(9, 289.5), Port::OUTPUT, module, ADSR::EOA));
	addOutput(Port::create<SilverSixPort>(Vec(40, 289.5), Port::OUTPUT, module, ADSR::EOD));
	addOutput(Port::create<SilverSixPortA>(Vec(72, 289.5), Port::OUTPUT, module, ADSR::EOS));
	addOutput(Port::create<SilverSixPortB>(Vec(102, 289.5), Port::OUTPUT, module, ADSR::EOR));
	
	addInput(Port::create<SilverSixPortA>(Vec(9, 327.5), Port::INPUT, module, ADSR::GATE_INPUT));
	addInput(Port::create<SilverSixPortC>(Vec(40, 327.5), Port::INPUT, module, ADSR::TRIG_INPUT));
	addOutput(Port::create<SilverSixPort>(Vec(102, 327.5), Port::OUTPUT, module, ADSR::ENVELOPE_OUTPUT));
	addOutput(Port::create<SilverSixPortD>(Vec(72, 327.5), Port::OUTPUT, module, ADSR::INV_OUTPUT));
	
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(19, 46), module, ADSR::ATTACK_LIGHT));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(49, 46), module, ADSR::DECAY_LIGHT));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(82, 46), module, ADSR::SUSTAIN_LIGHT));
	addChild(ModuleLightWidget::create<SmallLight<BlueLight>>(Vec(112, 46), module, ADSR::RELEASE_LIGHT));
}

Model *modelADSR = Model::create<ADSR, ADSRWidget>("MSM", "ADSR", "ADSR", ENVELOPE_GENERATOR_TAG);