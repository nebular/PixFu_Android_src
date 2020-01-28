//
// Created by rodo on 2020-01-24.
//

#include "Renderer.h"
#include "GLes3Launcher.h"


Renderer::Renderer()
		: mLastFrameNs(0) {
}

Renderer::~Renderer() {
}

bool Renderer::resize(uint32_t w, uint32_t h) {
	mLastFrameNs = 0;
	ALOGV("Renderer Base RESIZE w %d, h %d", w, h);
//	glViewport(0, 0, w, h);
	return true;
}


void Renderer::render() {

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;

	if (mLastFrameNs > 0) {
		float dt = float(nowNs - mLastFrameNs) * 0.000000001f;
		draw(dt);
	}

	mLastFrameNs = nowNs;
}

