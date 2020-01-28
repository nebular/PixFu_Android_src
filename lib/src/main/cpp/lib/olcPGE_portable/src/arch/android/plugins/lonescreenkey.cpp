//
// Created by rodo on 2020-01-26.
//

#include <lib/launcher/GLes3Launcher.h>
#include "lonescreenkey.h"

LoneScreenKey *LoneScreenKey::currentInstance = nullptr;

LoneScreenKey::LoneScreenKey(int resolution):fResolution(resolution) {}

int LoneScreenKey::input(int x, int y) {
	x/=fResolution;
	y/=fResolution;
	ALOGV("INPUT %d,%d", x, y);
	for (ScreenKey_t key : vKeys) {
		if (x > key.x0 && x < key.x1 && y>key.y0 && y<key.y1) return key.code;
	}
	return NO_KEYZ;
}


void LoneScreenKey::add(ScreenKey_t key) {
	vKeys.push_back(key);
}

void LoneScreenKey::DrawSelf(olc::PixelGameEngine *engine, olc::Pixel color) {
	for (ScreenKey_t key : vKeys) {
		if (engine->GetKey(static_cast<olc::Key>(key.code)).bHeld)
			engine->FillRect(key.x0, key.y0, key.x1-key.x0, key.y1-key.y0, color);
		else
			engine->DrawRect(key.x0, key.y0, key.x1-key.x0, key.y1-key.y0, color);
	}

}

int LoneScreenKey::sync(int x0, int y0, int x1, int y1, bool keystats[], bool isUp) {
	x0/=fResolution;
	y0/=fResolution;
	x1/=fResolution;
	y1/=fResolution;

	for (ScreenKey_t key : vKeys) {
		if (x1 > key.x0 && x1 < key.x1 && y1>key.y0 && y1<key.y1) {
			keystats[key.code]=!isUp;
		} else if (x0 > key.x0 && x0 < key.x1 && y0>key.y0 && y0<key.y1) {
			keystats[key.code]=!isUp;
		} else keystats[key.code] = 0;
	}
	return 0;
}
