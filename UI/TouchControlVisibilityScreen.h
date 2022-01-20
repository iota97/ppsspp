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

#pragma once

#include "Common/Render/TextureAtlas.h"
#include "MiscScreens.h"

namespace UI {
	class CheckBox;
}

struct TouchButtonToggle {
	const char *key;
	bool *show;
	ImageID img;
	std::function<UI::EventReturn(UI::EventParams&)> handle;
};

class TouchControlVisibilityScreen : public UIDialogScreenWithBackground {
public:
	void CreateViews() override;
	void onFinish(DialogResult result) override;

protected:
	UI::EventReturn OnToggleAll(UI::EventParams &e);

private:
	std::vector<TouchButtonToggle> toggles_;
	bool nextToggleAll_ = true;
};

class CustomAnalogMappingScreen : public UIDialogScreenWithBackground {
public:
	CustomAnalogMappingScreen(bool *show, int *up, int *down, int *left, int *right, int *press, bool *diag) : 
		show_(show), up_(up), down_(down), left_(left), right_(right), press_(press), diag_(diag) {}
	void CreateViews() override;
private:
	bool *show_;
	int *up_;
	int *down_;
	int *left_;
	int *right_;
	int *press_;
	bool *diag_;
};
