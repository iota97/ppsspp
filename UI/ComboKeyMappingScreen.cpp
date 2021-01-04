// Copyright (c) 2013- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#include "Common/System/Display.h"
#include "Common/Render/DrawBuffer.h"
#include "Common/Render/TextureAtlas.h"
#include "Common/UI/Context.h"
#include "Common/UI/View.h"
#include "Common/UI/ViewGroup.h"

#include "Common/Data/Text/I18n.h"
#include "Common/Data/Color/RGBAUtil.h"
#include "Common/File/PathBrowser.h"
#include "Common/Math/curves.h"
#include "Common/TimeUtil.h"
#include "Common/StringUtils.h"
#include "Core/Config.h"
#include "UI/GamepadEmu.h"

#include "TouchControlVisibilityScreen.h"
#include "UI/ComboKeyMappingScreen.h"

class ButtonPreview : public MultiTouchButton {
public:
	ButtonPreview(ImageID bgImg, ImageID img, float rotation, bool flip, int x, int y)
	: MultiTouchButton(bgImg, bgImg, img, 1.0f, new UI::AnchorLayoutParams(x, y, UI::NONE, UI::NONE, true)),
		x_(x), y_(y), rot_(rotation), bgImg_(bgImg), img_(img), flip_(flip) {
	}
	bool IsDown() override {
		return false;
	}

	void Draw(UIContext &dc) override {
		float opacity = g_Config.iTouchButtonOpacity / 100.0f;

		uint32_t colorBg = colorAlpha(g_Config.iTouchButtonStyle != 0 ? 0xFFFFFF : 0xc0b080, opacity);
		uint32_t color = colorAlpha(0xFFFFFF, opacity);

		dc.Draw()->DrawImageRotated(bgImg_, x_, y_, 1.0f, 0, colorBg, flip_);
		dc.Draw()->DrawImageRotated(img_, x_, y_, 1.0f, rot_*PI/180, color, false);
	}
private:
	int x_;
	int y_;
	float rot_;
	bool flip_;
	ImageID bgImg_;
	ImageID img_;
};

void ComboKeyScreen::CreateViews() {
	using namespace UI;
	auto co = GetI18NCategory("Controls");
	auto mc = GetI18NCategory("MappableControls");
	root_ = new LinearLayout(ORIENT_VERTICAL);
	root_->Add(new ItemHeader(co->T("Custom Key Setting")));
	LinearLayout *root__ = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(1.0));
	root_->Add(root__);
	LinearLayout *leftColumn = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(120, FILL_PARENT));
	auto di = GetI18NCategory("Dialog");

	static const ImageID comboKeyImages[] = {
		ImageID("I_1"), ImageID("I_2"), ImageID("I_3"), ImageID("I_4"), ImageID("I_5"),
		ImageID("I_CIRCLE"), ImageID("I_CROSS"), ImageID("I_SQUARE"), ImageID("I_TRIANGLE"),
		ImageID("I_L"), ImageID("I_R"), ImageID("I_START"), ImageID("I_SELECT"), ImageID("I_ARROW")
	};

	static const ImageID comboKeyShape[][2] = {
		{ ImageID("I_ROUND"), ImageID("I_ROUND_LINE") },
		{ ImageID("I_RECT"), ImageID("I_RECT_LINE") },
		{ ImageID("I_SHOULDER"), ImageID("I_SHOULDER_LINE") }
	};

	ConfigCustomButton* cfg = nullptr;
	bool* show = nullptr;
	memset(array, 0, sizeof(array));
	switch (id_) {
	case 0: 
		cfg = &g_Config.CustomKey0;
		show = &g_Config.touchCombo0.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey0.key >> i) & 0x01));
		break;
	case 1:
		cfg = &g_Config.CustomKey1;
		show = &g_Config.touchCombo1.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey1.key >> i) & 0x01));
		break;
	case 2:
		cfg = &g_Config.CustomKey2;
		show = &g_Config.touchCombo2.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey2.key >> i) & 0x01));
		break;
	case 3:
		cfg = &g_Config.CustomKey3;
		show = &g_Config.touchCombo3.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey3.key >> i) & 0x01));
		break;
	case 4:
		cfg = &g_Config.CustomKey4;
		show = &g_Config.touchCombo4.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey4.key >> i) & 0x01));
		break;
	case 5: 
		cfg = &g_Config.CustomKey5;
		show = &g_Config.touchCombo5.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey5.key >> i) & 0x01));
		break;
	case 6:
		cfg = &g_Config.CustomKey6;
		show = &g_Config.touchCombo6.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey6.key >> i) & 0x01));
		break;
	case 7:
		cfg = &g_Config.CustomKey7;
		show = &g_Config.touchCombo7.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey7.key >> i) & 0x01));
		break;
	case 8:
		cfg = &g_Config.CustomKey8;
		show = &g_Config.touchCombo8.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey8.key >> i) & 0x01));
		break;
	case 9:
		cfg = &g_Config.CustomKey9;
		show = &g_Config.touchCombo9.show;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.CustomKey9.key >> i) & 0x01));
		break;
	default:
		// This shouldn't happen, let's just not crash.
		cfg = &g_Config.CustomKey0;
		show = &g_Config.touchCombo0.show;
		break;
	}

	leftColumn->Add(new ButtonPreview(comboKeyShape[cfg->shape][g_Config.iTouchButtonStyle != 0], comboKeyImages[cfg->image], cfg->rotation, cfg->flip, 62, 82));

	root__->Add(leftColumn);
	rightScroll_ = new ScrollView(ORIENT_VERTICAL, new LinearLayoutParams(WRAP_CONTENT, WRAP_CONTENT, 1.0f));
	leftColumn->Add(new Spacer(new LinearLayoutParams(1.0f)));
	leftColumn->Add(new Choice(di->T("Back")))->OnClick.Handle<UIScreen>(this, &UIScreen::OnBack);
	root__->Add(rightScroll_);

	const int cellSize = 400;
	UI::GridLayoutSettings gridsettings(cellSize, 64, 5);
	gridsettings.fillCells = true;
	LinearLayout *vertLayout = new LinearLayout(ORIENT_VERTICAL);
	rightScroll_->Add(vertLayout);
	
	vertLayout->Add(new ItemHeader(co->T("Button Style")));
	vertLayout->Add(new CheckBox(show, co->T("Visible")));

	static const char *imageNames[] = { "1", "2", "3", "4", "5", "Circle", "Cross", "Square", "Triangle", "L", "R", "Start", "Select", "Arrow" };
	PopupMultiChoice *icon = vertLayout->Add(new PopupMultiChoice(&(cfg->image), co->T("Icon"), imageNames, 0, ARRAY_SIZE(imageNames), mc->GetName(), screenManager()));
	icon->OnChoice.Handle(this, &ComboKeyScreen::onCombo);

	vertLayout->Add(new PopupSliderChoiceFloat(&(cfg->rotation), -180.0f, 180.0f, co->T("Icon rotation"), 5.0f, screenManager()))->OnChange.Handle(this, &ComboKeyScreen::onCombo);
	
	static const char *shapeNames[] = { "Circle", "Rectangle", "Shoulder button" };
	vertLayout->Add(new PopupMultiChoice(&(cfg->shape), co->T("Shape"), shapeNames, 0, ARRAY_SIZE(shapeNames), mc->GetName(), screenManager()))->OnChoice.Handle(this, &ComboKeyScreen::onCombo);

	vertLayout->Add(new CheckBox(&(cfg->flip), co->T("Flip shape")))->OnClick.Handle(this, &ComboKeyScreen::onCombo);;

	vertLayout->Add(new ItemHeader(co->T("Button Binding")));
	vertLayout->Add(new CheckBox(&(cfg->toggle), co->T("Toggle mode")));
	GridLayout *grid = vertLayout->Add(new GridLayout(gridsettings, new LayoutParams(FILL_PARENT, WRAP_CONTENT)));

	std::map<std::string, ImageID> keyImages;
	keyImages["Circle"] = ImageID("I_CIRCLE");
	keyImages["Cross"] = ImageID("I_CROSS");
	keyImages["Square"] = ImageID("I_SQUARE");
	keyImages["Triangle"] = ImageID("I_TRIANGLE");
	keyImages["L"] = ImageID("I_L");
	keyImages["R"] = ImageID("I_R");
	keyImages["Start"] = ImageID("I_START");
	keyImages["Select"] = ImageID("I_SELECT");

	std::vector<std::string> keyToggles;
	keyToggles.push_back("Square");
	keyToggles.push_back("Triangle");
	keyToggles.push_back("Circle");
	keyToggles.push_back("Cross");
	keyToggles.push_back("Up");
	keyToggles.push_back("Down");
	keyToggles.push_back("Left");
	keyToggles.push_back("Right");
	keyToggles.push_back("Start");
	keyToggles.push_back("Select");
	keyToggles.push_back("L");
	keyToggles.push_back("R");
	keyToggles.push_back("Rapid Fire");
	keyToggles.push_back("Unthrottle");
	keyToggles.push_back("Speed Toggle");
	keyToggles.push_back("Rewind");
	keyToggles.push_back("Save State");
	keyToggles.push_back("Load State");
	keyToggles.push_back("Next Slot");
	keyToggles.push_back("Toggle Fullscreen");
	keyToggles.push_back("Speed Custom 1");
	keyToggles.push_back("Speed Custom 2");
	keyToggles.push_back("Texture Dump");
	keyToggles.push_back("Texture Replace");
	keyToggles.push_back("Screenshot");
	keyToggles.push_back("Toggle Mute");
	keyToggles.push_back("Open Chat");
	keyToggles.push_back("Analog CW");
	keyToggles.push_back("Analog CCW");

	std::map<std::string, ImageID>::iterator imageFinder;

	for (int i = 0; i < keyToggles.size(); ++i) {
		LinearLayout *row = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
		row->SetSpacing(0);

		CheckBox *checkbox = new CheckBox(&array[i], "", "", new LinearLayoutParams(50, WRAP_CONTENT));
		row->Add(checkbox);

		imageFinder = keyImages.find(keyToggles[i]);
		Choice *choice;
		if (imageFinder != keyImages.end()) {
			choice = new Choice(keyImages[imageFinder->first], new LinearLayoutParams(1.0f));
		}
		else {
			choice = new Choice(mc->T(keyToggles[i].c_str()), new LinearLayoutParams(1.0f));
		}

		ChoiceEventHandler *choiceEventHandler = new ChoiceEventHandler(checkbox);
		choice->OnClick.Handle(choiceEventHandler, &ChoiceEventHandler::onChoiceClick);

		choice->SetCentered(true);

		row->Add(choice);
		grid->Add(row);
	}
}

static int arrayToInt(bool ary[29]) {
	int value = 0;
	for (int i = 28; i >= 0; i--) {
		value |= ary[i] ? 1 : 0;
		value = value << 1;
	}
	return value >> 1;
}

void ComboKeyScreen::saveArray() {
	switch (id_) {
	case 0:
		g_Config.CustomKey0.key = arrayToInt(array);
		break;
	case 1:
		g_Config.CustomKey1.key = arrayToInt(array);
		break;
	case 2:
		g_Config.CustomKey2.key = arrayToInt(array);
		break;
	case 3:
		g_Config.CustomKey3.key = arrayToInt(array);
		break;
	case 4:
		g_Config.CustomKey4.key = arrayToInt(array);
		break;
	case 5:
		g_Config.CustomKey5.key = arrayToInt(array);
		break;
	case 6:
		g_Config.CustomKey6.key = arrayToInt(array);
		break;
	case 7:
		g_Config.CustomKey7.key = arrayToInt(array);
		break;
	case 8:
		g_Config.CustomKey8.key = arrayToInt(array);
		break;
	case 9:
		g_Config.CustomKey9.key = arrayToInt(array);
		break;
	}
}

void ComboKeyScreen::onFinish(DialogResult result) {
	saveArray();
	g_Config.Save("ComboKeyScreen::onFinish");
}

UI::EventReturn ComboKeyScreen::ChoiceEventHandler::onChoiceClick(UI::EventParams &e){
	checkbox_->Toggle();
	return UI::EVENT_DONE;
};

UI::EventReturn ComboKeyScreen::onCombo(UI::EventParams &e) {
	saveArray();
	CreateViews();
	return UI::EVENT_DONE;
}
