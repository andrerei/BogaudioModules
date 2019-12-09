
#include "VCO.hpp"

bool VCO::active() {
	return (
		outputs[SQUARE_OUTPUT].isConnected() ||
		outputs[SAW_OUTPUT].isConnected() ||
		outputs[TRIANGLE_OUTPUT].isConnected() ||
		outputs[SINE_OUTPUT].isConnected()
	);
}

void VCO::modulate() {
	_slowMode = params[SLOW_PARAM].getValue() > 0.5f;
	_linearMode = params[LINEAR_PARAM].getValue() > 0.5f;
	_fmLinearMode = params[FM_TYPE_PARAM].getValue() < 0.5f;
	_fmDepth = params[FM_PARAM].getValue();
}

void VCO::modulateChannel(int c) {
	VCOBase::modulateChannel(c);
	Engine& e = *_engines[c];

	e.squareActive = outputs[SQUARE_OUTPUT].isConnected();
	e.sawActive = outputs[SAW_OUTPUT].isConnected();
	e.triangleActive = outputs[TRIANGLE_OUTPUT].isConnected();
	e.sineActive = outputs[SINE_OUTPUT].isConnected();

	if (e.squareActive) {
		float pw = params[PW_PARAM].getValue();
		if (inputs[PW_INPUT].isConnected()) {
			pw *= clamp(inputs[PW_INPUT].getPolyVoltage(c) / 5.0f, -1.0f, 1.0f);
		}
		pw *= 1.0f - 2.0f * e.square.minPulseWidth;
		pw *= 0.5f;
		pw += 0.5f;
		e.square.setPulseWidth(e.squarePulseWidthSL.next(pw));
	}
}

void VCO::processChannel(const ProcessArgs& args, int c) {
	VCOBase::processChannel(args, c);
	Engine& e = *_engines[c];

	outputs[SQUARE_OUTPUT].setChannels(_channels);
	outputs[SQUARE_OUTPUT].setVoltage(e.squareOut, c);
	outputs[SAW_OUTPUT].setChannels(_channels);
	outputs[SAW_OUTPUT].setVoltage(e.sawOut, c);
	outputs[TRIANGLE_OUTPUT].setChannels(_channels);
	outputs[TRIANGLE_OUTPUT].setVoltage(e.triangleOut, c);
	outputs[SINE_OUTPUT].setChannels(_channels);
	outputs[SINE_OUTPUT].setVoltage(e.sineOut, c);
}

struct VCOWidget : ModuleWidget {
	static constexpr int hp = 10;

	VCOWidget(VCO* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);

		{
			SvgPanel *panel = new SvgPanel();
			panel->box.size = box.size;
			panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/VCO.svg")));
			addChild(panel);
		}

		addChild(createWidget<ScrewSilver>(Vec(0, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 0)));
		addChild(createWidget<ScrewSilver>(Vec(0, 365)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

		// generated by svg_widgets.rb
		auto frequencyParamPosition = Vec(41.0, 45.0);
		auto fineParamPosition = Vec(48.0, 153.0);
		auto slowParamPosition = Vec(114.0, 149.7);
		auto linearParamPosition = Vec(114.0, 162.7);
		auto pwParamPosition = Vec(62.0, 188.0);
		auto fmParamPosition = Vec(62.0, 230.0);
		auto fmTypeParamPosition = Vec(100.5, 231.5);

		auto pwInputPosition = Vec(15.0, 274.0);
		auto fmInputPosition = Vec(47.0, 274.0);
		auto pitchInputPosition = Vec(15.0, 318.0);
		auto syncInputPosition = Vec(47.0, 318.0);

		auto squareOutputPosition = Vec(79.0, 274.0);
		auto sawOutputPosition = Vec(111.0, 274.0);
		auto triangleOutputPosition = Vec(79.0, 318.0);
		auto sineOutputPosition = Vec(111.0, 318.0);
		// end generated by svg_widgets.rb

		addParam(createParam<Knob68>(frequencyParamPosition, module, VCO::FREQUENCY_PARAM));
		addParam(createParam<Knob16>(fineParamPosition, module, VCO::FINE_PARAM));
		addParam(createParam<IndicatorButtonGreen9>(slowParamPosition, module, VCO::SLOW_PARAM));
		addParam(createParam<Knob26>(pwParamPosition, module, VCO::PW_PARAM));
		addParam(createParam<Knob26>(fmParamPosition, module, VCO::FM_PARAM));
		addParam(createParam<SliderSwitch2State14>(fmTypeParamPosition, module, VCO::FM_TYPE_PARAM));
		addParam(createParam<IndicatorButtonGreen9>(linearParamPosition, module, VCO::LINEAR_PARAM));

		addInput(createInput<Port24>(pitchInputPosition, module, VCO::PITCH_INPUT));
		addInput(createInput<Port24>(syncInputPosition, module, VCO::SYNC_INPUT));
		addInput(createInput<Port24>(pwInputPosition, module, VCO::PW_INPUT));
		addInput(createInput<Port24>(fmInputPosition, module, VCO::FM_INPUT));

		addOutput(createOutput<Port24>(squareOutputPosition, module, VCO::SQUARE_OUTPUT));
		addOutput(createOutput<Port24>(sawOutputPosition, module, VCO::SAW_OUTPUT));
		addOutput(createOutput<Port24>(triangleOutputPosition, module, VCO::TRIANGLE_OUTPUT));
		addOutput(createOutput<Port24>(sineOutputPosition, module, VCO::SINE_OUTPUT));
	}

	void appendContextMenu(Menu* menu) override {
		VCO* m = dynamic_cast<VCO*>(module);
		assert(m);
		menu->addChild(new MenuLabel());
		OptionsMenuItem* p = new OptionsMenuItem("Polyphony channels from");
		p->addItem(OptionMenuItem("V/OCT input", [m]() { return m->_polyInputID == VCO::PITCH_INPUT; }, [m]() { m->_polyInputID = VCO::PITCH_INPUT; }));
		p->addItem(OptionMenuItem("FM input", [m]() { return m->_polyInputID == VCO::FM_INPUT; }, [m]() { m->_polyInputID = VCO::FM_INPUT; }));
		OptionsMenuItem::addToMenu(p, menu);
	}
};

Model* modelVCO = bogaudio::createModel<VCO, VCOWidget>("Bogaudio-VCO", "VCO", "Oscillator with 4 waveforms, PWM, FM, hard sync", "Oscillator", "Polyphonic");
