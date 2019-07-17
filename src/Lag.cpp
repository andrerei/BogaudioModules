
#include "Lag.hpp"

void Lag::onReset() {
	_modulationStep = modulationSteps;
}

void Lag::process(const ProcessArgs& args) {
	if (!(inputs[IN_INPUT].isConnected() && outputs[OUT_OUTPUT].isConnected())) {
		return;
	}

	++_modulationStep;
	if (_modulationStep >= modulationSteps) {
		_modulationStep = 0;

		float time = params[TIME_PARAM].getValue();
		if (inputs[TIME_INPUT].isConnected()) {
			time *= clamp(inputs[TIME_INPUT].getVoltage() / 10.0f, 0.0f, 1.0f);
		}
		switch ((int)roundf(params[TIME_SCALE_PARAM].getValue())) {
			case 0: {
				time /= 10.f;
				break;
			}
			case 2: {
				time *= 10.f;
				break;
			}
		}
		time *= 1000.0f;

		float shape = params[SHAPE_PARAM].getValue();
		if (inputs[SHAPE_INPUT].isConnected()) {
			shape *= clamp(inputs[SHAPE_INPUT].getVoltage() / 5.0f, -1.0f, 1.0f);
		}
		if (shape < 0.0) {
			shape = 1.0f + shape;
			shape = _slew.minShape + shape * (1.0f - _slew.minShape);
		}
		else {
			shape *= _slew.maxShape - 1.0f;
			shape += 1.0f;
		}

		_slew.setParams(APP->engine->getSampleRate(), time, shape);
	}

	outputs[OUT_OUTPUT].setVoltage(_slew.next(inputs[IN_INPUT].getVoltage()));
}

struct LagWidget : ModuleWidget {
	static constexpr int hp = 3;

	LagWidget(Lag* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);

		{
			SvgPanel *panel = new SvgPanel();
			panel->box.size = box.size;
			panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Lag.svg")));
			addChild(panel);
		}

		addChild(createWidget<ScrewSilver>(Vec(0, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 15, 365)));

		// generated by svg_widgets.rb
		auto timeParamPosition = Vec(8.0, 36.0);
		auto timeScaleParamPosition = Vec(14.5, 84.0);
		auto shapeParamPosition = Vec(8.0, 176.0);

		auto timeInputPosition = Vec(10.5, 107.0);
		auto shapeInputPosition = Vec(10.5, 217.0);
		auto inInputPosition = Vec(10.5, 267.0);

		auto outOutputPosition = Vec(10.5, 305.0);
		// end generated by svg_widgets.rb

		addParam(createParam<Knob29>(timeParamPosition, module, Lag::TIME_PARAM));
		{
			auto w = createParam<Knob16>(timeScaleParamPosition, module, Lag::TIME_SCALE_PARAM);
			auto k = dynamic_cast<SvgKnob*>(w);
			k->snap = true;
			k->minAngle = -M_PI / 4.0f;
			k->maxAngle = M_PI / 4.0f;
			k->speed = 3.0;
			addParam(w);
		}
		addParam(createParam<Knob29>(shapeParamPosition, module, Lag::SHAPE_PARAM));

		addInput(createInput<Port24>(timeInputPosition, module, Lag::TIME_INPUT));
		addInput(createInput<Port24>(shapeInputPosition, module, Lag::SHAPE_INPUT));
		addInput(createInput<Port24>(inInputPosition, module, Lag::IN_INPUT));

		addOutput(createOutput<Port24>(outOutputPosition, module, Lag::OUT_OUTPUT));
	}
};

Model* modelLag = bogaudio::createModel<Lag, LagWidget>("Bogaudio-Lag", "Lag",  "slew limiter / lag processor", "Slew limiter");
