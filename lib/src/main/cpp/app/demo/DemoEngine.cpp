//
// Created by rodo on 2020-01-24.
//

#include <olcPixelGameEngine.h>
#include <jni.h>

#include "DemoEngine.h"


bool DemoEngine::OnUserCreate() {
	Clear(olc::YELLOW);
	return true;
}

bool DemoEngine::OnUserUpdate(float fElapsedTime) {
	DrawString(100,100,"HELLO ANDROID", olc::RED, 6.0);
	return true;
}
