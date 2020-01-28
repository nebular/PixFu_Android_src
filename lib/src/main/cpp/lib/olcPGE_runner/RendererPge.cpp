/*
 *
 * A renderer to interface olcPixelGameEngine
 * Author: Rodolfo Lopez Pintor, 2020
 *
 * This is just-for-fun, open source code, licensed under Creative Commons Attribution.
 *
 * The original OLCPixelGameEngine had an EngineThread, that has now been splitted in its
 * 3 parts: initialization, oop, end. Thanks to that, we call each stage from here when
 * neccessary, because Android will tell us when to paint , when to init and when to die.
 * The EngineThread splitted methods are olca_thread_init, olca_thread_tick, olca_thread_deinit.
 *
 * So when we have the OpenGL Surface setup and the size is available, is then when we
 * construct() the olcPixelGameEngine. From now on, we will get requests from OpenGL
 * to draw the frame, and is there where we give control to olcPixelGameEngine, with a
 * calculated fElapsedTime as it expects.
 *
 */

#include <EGL/egl.h>
#include "RendererPge.h"

RendererPge *RendererPge::createRender(olc::PixelGameEngine *engine) {

	auto *renderer = new RendererPge(engine);
	if (!renderer->init()) {
		delete renderer;
		return nullptr;
	}
	return renderer;
}


RendererPge::RendererPge(olc::PixelGameEngine *engine)
		: mEglContext(eglGetCurrentContext()),
		  pEngine(engine) {
}


RendererPge::~RendererPge() {
	if (isInited)
		pEngine->olca_thread_deinit();
}

/**
 * We are only in business when we know the size of our window. Until then ... just wait.
 * @return  true
 */

bool RendererPge::init() {

	ALOGV("Renderer INITED, waiting for RESIZE");

	return true;

}

bool RendererPge::resize(uint32_t w, uint32_t h) {

	ALOGV("Renderer OLC RESIZE w %d, h %d", w, h);

	if (!isInited) {
		isInited = true;

		Renderer::resize(w, h);

		// construct the PGE class with our window size
		if (pEngine->Construct(w, h, 1, 1, false) == olc::FAIL)
			return false;

		if (pEngine->Start() == olc::FAIL)
			return false;

		// calls former pge::EngineThread() first part (OpenGL init)
		if (!pEngine->olca_thread_init())
			return false;

	}

	return true;

}

void RendererPge::draw(float elapsedTimeNs) {

	// calls the PGE EngineThread loop that now is not on a thread. It is us here the ones
	// in a thread !

	if (isInited)
		pEngine->olca_thread_tick(elapsedTimeNs);
}

void RendererPge::OnMotionEvent(MotionEvent_t event) {

	// MotionEvent supplied here by the Java part, then we send it down to the engine. It will be
	// translated into GetMouse positions.

	if (isInited)
		pEngine->olca_on_motionevent(event);
}

