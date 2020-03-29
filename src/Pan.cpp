
#include "Pan.hpp"
#include "mixer.hpp"

void Pan::sampleRateChange() {
	float sr = APP->engine->getSampleRate();
	for (int c = 0; c < maxChannels; ++c) {
		_slew1[c].setParams(sr, MIXER_PAN_SLEW_MS, 2.0f);
		_slew2[c].setParams(sr, MIXER_PAN_SLEW_MS, 2.0f);
	}
}

bool Pan::active() {
	return (inputs[IN1_INPUT].isConnected() || inputs[IN2_INPUT].isConnected()) && (outputs[L_OUTPUT].isConnected() || outputs[R_OUTPUT].isConnected());
}

int Pan::channels() {
	return inputs[IN1_INPUT].getChannels();
}

void Pan::processChannel(const ProcessArgs& args, int c) {
	float pan = params[PAN1_PARAM].getValue();
	if (inputs[CV1_INPUT].isConnected()) {
		pan *= clamp(inputs[CV1_INPUT].getPolyVoltage(c) / 5.0f, -1.0f, 1.0f);
	}
	_panner1[c].setPan(_slew1[c].next(pan));

	pan = params[PAN2_PARAM].getValue();
	if (inputs[CV2_INPUT].isConnected()) {
		pan *= clamp(inputs[CV2_INPUT].getPolyVoltage(c) / 5.0f, -1.0f, 1.0f);
	}
	_panner2[c].setPan(_slew2[c].next(pan));

	float l1 = 0.0f, r1 = 0.0f;
	_panner1[c].next(inputs[IN1_INPUT].getPolyVoltage(c), l1, r1);
	float l2 = 0.0f, r2 = 0.0f;
	_panner2[c].next(inputs[IN2_INPUT].getPolyVoltage(c), l2, r2);
	outputs[L_OUTPUT].setChannels(_channels);
	outputs[L_OUTPUT].setVoltage(_saturatorLeft[c].next(l1 + l2), c);
	outputs[R_OUTPUT].setChannels(_channels);
	outputs[R_OUTPUT].setVoltage(_saturatorRight[c].next(r1 + r2), c);
}

struct PanWidget : ModuleWidget {
	static constexpr int hp = 3;

	PanWidget(Pan* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);

		{
			SvgPanel *panel = new SvgPanel();
			panel->box.size = box.size;
			panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Pan.svg")));
			addChild(panel);
		}

		addChild(createWidget<ScrewSilver>(Vec(0, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

		// generated by svg_widgets.rb
		auto pan1ParamPosition = Vec(9.5, 24.5);
		auto pan2ParamPosition = Vec(9.5, 147.5);

		auto cv1InputPosition = Vec(10.5, 62.0);
		auto in1InputPosition = Vec(10.5, 97.0);
		auto cv2InputPosition = Vec(10.5, 185.0);
		auto in2InputPosition = Vec(10.5, 220.0);

		auto lOutputPosition = Vec(10.5, 268.0);
		auto rOutputPosition = Vec(10.5, 303.0);
		// end generated by svg_widgets.rb

		addParam(createParam<Knob26>(pan1ParamPosition, module, Pan::PAN1_PARAM));
		addParam(createParam<Knob26>(pan2ParamPosition, module, Pan::PAN2_PARAM));

		addInput(createInput<Port24>(cv1InputPosition, module, Pan::CV1_INPUT));
		addInput(createInput<Port24>(in1InputPosition, module, Pan::IN1_INPUT));
		addInput(createInput<Port24>(cv2InputPosition, module, Pan::CV2_INPUT));
		addInput(createInput<Port24>(in2InputPosition, module, Pan::IN2_INPUT));

		addOutput(createOutput<Port24>(lOutputPosition, module, Pan::L_OUTPUT));
		addOutput(createOutput<Port24>(rOutputPosition, module, Pan::R_OUTPUT));
	}
};

Model* modelPan = bogaudio::createModel<Pan, PanWidget>("Bogaudio-Pan", "PAN", "Dual stereo panner", "Panning", "Dual", "Polyphonic");
