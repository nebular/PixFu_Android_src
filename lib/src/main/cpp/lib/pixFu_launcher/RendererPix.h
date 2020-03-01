#ifndef RENDERERPGE_H
#define RENDERERPGE_H

/*
 *
 * A renderer for PixFu Game Engine
 * Author: Rodolfo Lopez Pintor, 2020
 *
 * This is just-for-fun, open source code, licensed under Creative Commons CC-BY 4.0.
 *
 */

#include "Launcher.h"
#include "Fu.hpp"
#include "arch/android/platform_android.hpp"
#include <EGL/egl.h>
#include "Renderer.h"

namespace Pix {

	class RendererPix : public Renderer {

		static const std::string TAG;

	public:

		RendererPix(Pix::Fu *engine);

		virtual ~RendererPix();

		bool resize(uint32_t w, uint32_t h);

		void onMotionEvent(MotionEvent_t event);

		static RendererPix *createRender(Pix::Fu *engine);

	protected:

		void draw(float elapsedTimeNs);

	private:

		bool isInited = false;
		float mCounter = 0;
		EGLContext mEglContext;

		Pix::Fu *pEngine = nullptr;

		void onLifeCycle(LCycle_t status);
	};
}
#endif //GLES3JNI_RENDEREROLC_H
