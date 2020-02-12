/**
 *
 * A generic render. Mind that up to know, all the stack doesn't know the PGE, so it is abstraact to be used
 * as a genric android native application launcher. Is by extending this class how we will tie the renderer
 * to a PGE instance.
 *
 * @author Rodolfo Lopez Pintor 2020.
 * @license Creative Commons CC-BY 4.0
 *
 */


#include "Renderer.h"
#include "PgeRunner.h"

namespace rgl {


	Renderer::Renderer()
			: mLastFrameNs(0) {
	}

	Renderer::~Renderer() {}

/**
 * This function is called by Android when the size of the "surface" (our window) is finally known.
 * @param w Window width in Pixels
 * @param h Window height in Pixels
 * @return Whether the resize request succeeded (aborts the program if it doesnt)
 */

	bool Renderer::resize(uint32_t w, uint32_t h) {

		mLastFrameNs = 0;
		ALOGV("Renderer Base RESIZE w %d, h %d", w, h);

		//	glViewport(0, 0, w, h);
		// test again, 99% sure this is not needed here.

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

};