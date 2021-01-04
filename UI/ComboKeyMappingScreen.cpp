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

#include "TouchControlVisibilityScreen.h"
#include "UI/ComboKeyMappingScreen.h"

void ComboKeyScreen::CreateViews() {
	using namespace UI;
	auto co = GetI18NCategory("Controls");
	auto mc = GetI18NCategory("MappableControls");
	root_ = new LinearLayout(ORIENT_VERTICAL);
	root_->Add(new ItemHeader(co->T("Combo Key Setting")));
	LinearLayout *root__ = new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(1.0));
	root_->Add(root__);
	LinearLayout *leftColumn = new LinearLayout(ORIENT_VERTICAL, new LinearLayoutParams(120, FILL_PARENT));
	auto di = GetI18NCategory("Dialog");

	static const ImageID comboKeyImages[] = {
		ImageID("I_1"), ImageID("I_2"), ImageID("I_3"), ImageID("I_4"), ImageID("I_5"),
		ImageID("I_CIRCLE"), ImageID("I_CROSS"), ImageID("I_SQUARE"), ImageID("I_TRIANGLE"),
		ImageID("I_L"), ImageID("I_R"), ImageID("I_START"), ImageID("I_SELECT"), ImageID("I_ARROW")
	};

	comboselect = new ChoiceStrip(ORIENT_VERTICAL, new AnchorLayoutParams(10, 10, NONE, NONE));
	comboselect->SetSpacing(10);
	comboselect->AddChoice(comboKeyImages[g_Config.iCombokeyImage0]);
	comboselect->AddChoice(comboKeyImages[g_Config.iCombokeyImage1]);
	comboselect->AddChoice(comboKeyImages[g_Config.iCombokeyImage2]);
	comboselect->AddChoice(comboKeyImages[g_Config.iCombokeyImage3]);
	comboselect->AddChoice(comboKeyImages[g_Config.iCombokeyImage4]);

	comboselect->SetSelection(*mode);
	comboselect->OnChoice.Handle(this, &ComboKeyScreen::onCombo);
	leftColumn->Add(comboselect);
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

	bool *toggle = nullptr;
	int *image = nullptr;
	int *shape = nullptr;
	memset(array, 0, sizeof(array));
	switch (*mode) {
	case 0: 
		toggle = &g_Config.bComboToggle0;
		image = &g_Config.iCombokeyImage0;
		shape = &g_Config.iComboShape0;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.iCombokey0 >> i) & 0x01));
		break;
	case 1:
		toggle = &g_Config.bComboToggle1;
		image = &g_Config.iCombokeyImage1;
		shape = &g_Config.iComboShape1;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.iCombokey1 >> i) & 0x01));
		break;
	case 2:
		toggle = &g_Config.bComboToggle2;
		image = &g_Config.iCombokeyImage2;
		shape = &g_Config.iComboShape2;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.iCombokey2 >> i) & 0x01));
		break;
	case 3:
		toggle = &g_Config.bComboToggle3;
		image = &g_Config.iCombokeyImage3;
		shape = &g_Config.iComboShape3;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.iCombokey3 >> i) & 0x01));
		break;
	case 4:
		toggle = &g_Config.bComboToggle4;
		image = &g_Config.iCombokeyImage4;
		shape = &g_Config.iComboShape4;
		for (int i = 0; i < 29; i++)
			array[i] = (0x01 == ((g_Config.iCombokey4 >> i) & 0x01));
		break;
	default:
		// This shouldn't happen, let's just not crash.
		toggle = &g_Config.bComboToggle0;
		image = &g_Config.iCombokeyImage0;
		shape = &g_Config.iComboShape0;
		break;
	}
	
	static const char *imageNames[] = { "1", "2", "3", "4", "5", "Circle", "Cross", "Square", "Triangle", "L", "R", "Start", "Select", "Arrow" };
	PopupMultiChoice *icon = vertLayout->Add(new PopupMultiChoice(image, co->T("Icon"), imageNames, 0, ARRAY_SIZE(imageNames), mc->GetName(), screenManager()));
	icon->OnChoice.Handle(this, &ComboKeyScreen::onCombo);
	static const char *shapeNames[] = { "Circle", "Rectangle" };
	vertLayout->Add(new PopupMultiChoice(shape, co->T("Shape"), shapeNames, 0, ARRAY_SIZE(shapeNames), mc->GetName(), screenManager()));
	vertLayout->Add(new CheckBox(toggle, co->T("Toggle mode")));

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

void ComboKeyScreen::onFinish(DialogResult result) {
	switch (*mode) {
	case 0:
		g_Config.iCombokey0 = arrayToInt(array);
		break;
	case 1:
		g_Config.iCombokey1 = arrayToInt(array);
		break;
	case 2:
		g_Config.iCombokey2 = arrayToInt(array);
		break;
	case 3:
		g_Config.iCombokey3 = arrayToInt(array);
		break;
	case 4:
		g_Config.iCombokey4 = arrayToInt(array);
		break;
	}
	g_Config.Save("ComboKeyScreen::onFInish");
}

UI::EventReturn ComboKeyScreen::ChoiceEventHandler::onChoiceClick(UI::EventParams &e){
	checkbox_->Toggle();
	return UI::EVENT_DONE;
};

UI::EventReturn ComboKeyScreen::onCombo(UI::EventParams &e) {
	switch (*mode){
	case 0:g_Config.iCombokey0 = arrayToInt(array);
		break;
	case 1:g_Config.iCombokey1 = arrayToInt(array);
		break;
	case 2:g_Config.iCombokey2 = arrayToInt(array);
		break;
	case 3:g_Config.iCombokey3 = arrayToInt(array);
		break;
	case 4:g_Config.iCombokey4 = arrayToInt(array);
	}
	*mode = comboselect->GetSelection();
	CreateViews();
	return UI::EVENT_DONE;
}
