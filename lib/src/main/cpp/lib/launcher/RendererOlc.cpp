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
#include "RendererOlc.h"

RendererOlc *RendererOlc::createRender(olc::PixelGameEngine *engine) {

	RendererOlc *renderer = new RendererOlc(engine);
	if (!renderer->init()) {
		delete renderer;
		return NULL;
	}
	return renderer;
}


RendererOlc::RendererOlc(olc::PixelGameEngine *engine)
		: mEglContext(eglGetCurrentContext()),
		  pEngine(engine) {
}



RendererOlc::~RendererOlc() {
	/* The destructor may be called after the context has already been
	 * destroyed, in which case our objects have already been destroyed.
	 *
	 * If the context exists, it must be current. This only happens when we're
	 * cleaning up after a failed init().
	*/
	if (isInited)
		pEngine->olca_thread_deinit();
}

bool RendererOlc::init() {

	ALOGV("Renderer INITED, waiting for RESIZE");
	return true;

}

bool RendererOlc::resize(uint32_t w, uint32_t h) {

	ALOGV("Renderer OLC RESIZE w %d, h %d", w, h);

	if (!isInited) {
		isInited = true;

		Renderer::resize(w, h);

		if (pEngine->Construct(w, h, 1, 1, false) == olc::FAIL)
			return false;

		if (pEngine->Start() == olc::FAIL)
			return false;

		if (!pEngine->olca_thread_init())
			return false;

	}

	return true;

}

void RendererOlc::draw(float elapsedTimeNs) {

	if (isInited)
		pEngine->olca_thread_tick( elapsedTimeNs  );
}

void RendererOlc::OnMotionEvent(MotionEvent_t event) {
	if (isInited)
		pEngine->olca_on_motionevent(event);
}

