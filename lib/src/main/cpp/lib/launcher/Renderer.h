//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_RENDERER_H
#define GLES3JNI_RENDERER_H

#include <cstdint>

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "olcPixelGameEngine.h"

// ----------------------------------------------------------------------------
// Interface to the ES2 and ES3 renderers, used by JNI code.

class Renderer {

public:

	Renderer();
	virtual ~Renderer();
	void render();

	virtual bool resize(uint32_t w,uint32_t h);
	virtual void draw(float fElapsedNs) = 0;
	virtual void OnMotionEvent(MotionEvent_t event) = 0;

protected:
	uint64_t mLastFrameNs;

};


#endif //GLES3JNI_RENDERER_H
