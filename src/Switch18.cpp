
#include "Switch18.hpp"

struct Switch18Widget : SwitchMatrixModuleWidget {
	static constexpr int hp = 6;

	Switch18Widget(Switch18* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
		setPanel(box.size, "Switch18");
		createScrews();

		// generated by svg_widgets.rb
		auto mix1ParamPosition = Vec(15.0, 33.0);
		auto mix2ParamPosition = Vec(15.0, 70.0);
		auto mix3ParamPosition = Vec(15.0, 107.0);
		auto mix4ParamPosition = Vec(15.0, 144.0);
		auto mix5ParamPosition = Vec(15.0, 181.0);
		auto mix6ParamPosition = Vec(15.0, 218.0);
		auto mix7ParamPosition = Vec(15.0, 255.0);
		auto mix8ParamPosition = Vec(15.0, 292.0);

		auto inInputPosition = Vec(53.5, 321.0);

		auto out1OutputPosition = Vec(53.5, 30.0);
		auto out2OutputPosition = Vec(53.5, 67.0);
		auto out3OutputPosition = Vec(53.5, 104.0);
		auto out4OutputPosition = Vec(53.5, 141.0);
		auto out5OutputPosition = Vec(53.5, 178.0);
		auto out6OutputPosition = Vec(53.5, 215.0);
		auto out7OutputPosition = Vec(53.5, 252.0);
		auto out8OutputPosition = Vec(53.5, 289.0);
		// end generated by svg_widgets.rb

		createSwitch<InvertingIndicatorButton18>(mix1ParamPosition, module, Switch18::MIX1_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix2ParamPosition, module, Switch18::MIX2_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix3ParamPosition, module, Switch18::MIX3_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix4ParamPosition, module, Switch18::MIX4_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix5ParamPosition, module, Switch18::MIX5_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix6ParamPosition, module, Switch18::MIX6_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix7ParamPosition, module, Switch18::MIX7_PARAM);
		createSwitch<InvertingIndicatorButton18>(mix8ParamPosition, module, Switch18::MIX8_PARAM);

		addInput(createInput<Port24>(inInputPosition, module, Switch18::IN_INPUT));

		addOutput(createOutput<Port24>(out1OutputPosition, module, Switch18::OUT1_OUTPUT));
		addOutput(createOutput<Port24>(out2OutputPosition, module, Switch18::OUT2_OUTPUT));
		addOutput(createOutput<Port24>(out3OutputPosition, module, Switch18::OUT3_OUTPUT));
		addOutput(createOutput<Port24>(out4OutputPosition, module, Switch18::OUT4_OUTPUT));
		addOutput(createOutput<Port24>(out5OutputPosition, module, Switch18::OUT5_OUTPUT));
		addOutput(createOutput<Port24>(out6OutputPosition, module, Switch18::OUT6_OUTPUT));
		addOutput(createOutput<Port24>(out7OutputPosition, module, Switch18::OUT7_OUTPUT));
		addOutput(createOutput<Port24>(out8OutputPosition, module, Switch18::OUT8_OUTPUT));
	}
};

Model* modelSwitch18 = createModel<Switch18, Switch18Widget>("Bogaudio-Switch18", "SWITCH18", "8-way signal router", "Switch", "Polyphonic");
