/**
 *
 * A renderer to interface olcPixelGameEngine
 * @author Rodolfo Lopez Pintor 2020.
 * @license Creative Commons CC-BY 4.0
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

RendererPge::RendererPge(olc::PixelGameEngine *engine)
		: mEglContext(eglGetCurrentContext()),
		  pEngine(engine) {
}

RendererPge::~RendererPge() {
	ALOGV("RendererPge: Destruct");
	if (isInited) {
		pEngine->olca_thread_deinit();
		isInited = false;
	}
}

RendererPge *RendererPge::createRender(olc::PixelGameEngine *engine) {
	return new RendererPge(engine);
}


void RendererPge::onLifeCycle(LCycle_t status) {

	// ALOGV("RendererPge: OLC PAUSE %d", status);
	// android system pause

	switch (status) {

		case ONSURFACECREATED:
			// context has changed
			mEglContext = eglGetCurrentContext();
			break;

		case ONPAUSE:
			// clean up resources on the PGE side
			pEngine->olca_thread_deinit();
			break;

		case ONRESUME:
			// nothing as init() will be called again
			break;
	}
}

// "much more than a resize" as this function is called when the GL surface is created
// it is really the only place where we have guaranteed access to the GL context just created
// and with a valid size

bool RendererPge::resize(uint32_t w, uint32_t h) {

	//	ALOGV("RendererPge: OLC RESIZE w %d, h %d", w, h);

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

	} else {
		glViewport(0, 0, w, h);
		pEngine->olca_thread_reinit(w, h);
		Renderer::resize(w, h);
	}

	return true;

}

void RendererPge::draw(float elapsedTimeNs) {

	// calls the PGE EngineThread loop that now is not on a thread. It is us here the ones
	// in a thread !

	if (isInited)
		pEngine->olca_thread_tick(elapsedTimeNs);
}

void RendererPge::onMotionEvent(MotionEvent_t event) {

	// MotionEvent supplied here by the Java part, then we send it down to the engine. It will be
	// translated into GetMouse positions.

	if (isInited)
		pEngine->olca_on_motionevent(event);
}

