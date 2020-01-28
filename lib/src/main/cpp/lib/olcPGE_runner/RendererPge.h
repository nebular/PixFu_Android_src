#ifndef RENDERERPGE_H
#define RENDERERPGE_H

/*
 * The PGE Renderer.
 * Author: Rodolfo Lopez Pintor, 2020
 */

#include "PgeRunner.h"
#include <olcPixelGameEngine.h>
#include <EGL/egl.h>
#include "Renderer.h"

class RendererPge : public Renderer {

public:

	virtual ~RendererPge();

	bool resize(uint32_t w, uint32_t h);

	bool init();

	RendererPge(olc::PixelGameEngine *engine);

	void draw(float elapsedTimeNs);

	void OnMotionEvent(MotionEvent_t event);

	static RendererPge *createRender(olc::PixelGameEngine *engine);

protected:

private:

	bool isInited = false;
	const EGLContext mEglContext;
	olc::PixelGameEngine *pEngine = nullptr;

};

#endif //GLES3JNI_RENDEREROLC_H
