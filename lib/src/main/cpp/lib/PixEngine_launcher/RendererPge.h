#ifndef RENDERERPGE_H
#define RENDERERPGE_H

/*
 *
 * A renderer to interface olcPixelGameEngine
 * Author: Rodolfo Lopez Pintor, 2020
 *
 * This is just-for-fun, open source code, licensed under Creative Commons CC-BY 4.0.
 *
 */

#include "PgeRunner.h"
#include "PixEngine.hpp"
#include "arch/android/androidlayer.hpp"
#include <EGL/egl.h>
#include "Renderer.h"

namespace rgl {

	class RendererPge : public Renderer {

	public:

		RendererPge(rgl::PixEngine *engine);

		virtual ~RendererPge();

		bool resize(uint32_t w, uint32_t h);

		void onMotionEvent(MotionEvent_t event);

		static RendererPge *createRender(rgl::PixEngine *engine);

	protected:

		void draw(float elapsedTimeNs);

	private:

		bool isInited = false;
		EGLContext mEglContext;

		rgl::PixEngine *pEngine = nullptr;

		void onLifeCycle(LCycle_t status);
	};
}
#endif //GLES3JNI_RENDEREROLC_H
