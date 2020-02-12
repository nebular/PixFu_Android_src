/*
 *
 * Generic Android OpenGL Native Application Launcher
 * Author: Rodolfo Lopez Pintor 2020.
 *
 * License: Creative Commons CC-BY 4.0
 *
 * This is the native part of the launcher application. Will lbe called by Android to initialize, render,
 * deinit and on mouse events.
 *
 */

#include <jni.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "PixEngine.hpp"
#include "PgeRunner.h"
#include "RendererPge.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

static rgl::Renderer *g_renderer = nullptr;


// these functions are called from Java, hence the exotic decorations
extern "C" {

JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_init(JNIEnv *env, jclass obj, jstring path);

JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_resize(JNIEnv *env, jclass obj, jint width,
															 jint height);
JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_onTouch(JNIEnv *env, jclass obj,
															  jobject motionEvent,  jint decodedAction, jint pointerId, jfloat screenDensity);
};

#if !defined(DYNAMIC_ES3)

static GLboolean gl3stubInit() {
	return GL_TRUE;
}

#endif


/**
 * Initialization. This is really onSurfaceCreated and the place to init OpenGL
 * It may be called several times if the user goes back to home, etc.
 * Called from JAVA
 */

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_init(JNIEnv *env, jclass obj,
														   jstring internalFilesPath) {

	if (g_renderer == nullptr) {

		const char *cstr = env->GetStringUTFChars(internalFilesPath, nullptr);

		/*
		 * Initialize the Base Path for the PGE. Android is very restrictive and will only allow to write
		 * in very few places. This path is the internal application storage directory, and all assets are
		 * copied here by the Java part.
		 */

		rgl::PixEnginePlatform::setPath(std::string(cstr) + "/");

		/*
		 * Creates the Pge Renderer. It is there where we will instantiate the PGE, and
		 * where the PGE object lives.
		 */

		const char *versionStr = (const char *) glGetString(GL_VERSION);
		if ((strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) ||
			strstr(versionStr, "OpenGL ES 2.")) {
			g_renderer = rgl::RendererPge::createRender(rgl::PixEngineAndroid::BOOTINSTANCE);
		} else {
			ALOGE("Unsupported OpenGL ES version");
		}
	} else {
		g_renderer->onLifeCycle(rgl::Renderer::ONSURFACECREATED);
	}
}


/**
 * "Resize" is really our main function, as it is the first place where we know about the window
 * size that has been assigned to us (we request the window size and it is later confirmed here
 * by Android's openGL stack)
 *
 * Called from JAVA when the size is known.
 *
 */

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_resize(JNIEnv *env, jclass obj, jint width,
															 jint height) {
	if (g_renderer) {
		g_renderer->resize((unsigned) width, (unsigned) height);
	}
}

/**
 * This is called by the OpenGL stack th request a frame.
 */

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_step(JNIEnv *env, jclass obj) {
	if (g_renderer) {
		g_renderer->render();
	}
}

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_onPause(JNIEnv *env, jclass obj, jboolean status) {
	if (g_renderer) {
		g_renderer->onLifeCycle(status ? rgl::Renderer::ONPAUSE : rgl::Renderer::ONRESUME);
	}
}


/**
 * This is called by the Java Activity to send MotionEvents (touch events)
 */

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpge_android_pgerunner_PgeNativeLib_onTouch(JNIEnv *jenv, jclass obj,
															  jobject motionEvent,  jint decodedAction, jint pointerId, jfloat screenDensity) {

	jclass motionEventClass = jenv->GetObjectClass(motionEvent);

	jmethodID pointersCountMethodId = jenv->GetMethodID(motionEventClass, "getPointerCount", "()I");
	int pointersCount = jenv->CallIntMethod(motionEvent, pointersCountMethodId);

	jmethodID getActionMethodId = jenv->GetMethodID(motionEventClass, "getAction", "()I");
	int32_t action = jenv->CallIntMethod(motionEvent, getActionMethodId);

	jmethodID getXMethodId = jenv->GetMethodID(motionEventClass, "getX", "(I)F");
	float x0 = jenv->CallFloatMethod(motionEvent, getXMethodId, 0);

	jmethodID getYMethodId = jenv->GetMethodID(motionEventClass, "getY", "(I)F");
	float y0 = jenv->CallFloatMethod(motionEvent, getYMethodId, 0);

	float x1 = -1;
	float y1 = -1;
	if (pointersCount > 1) {
		x1 = jenv->CallFloatMethod(motionEvent, getXMethodId, 1);
		y1 = jenv->CallFloatMethod(motionEvent, getYMethodId, 1);
	}

	rgl::MotionEvent_t inputEvent;
	inputEvent.PointersCount = pointersCount;
	inputEvent.Action = action;
	inputEvent.RawAction = decodedAction;
	inputEvent.PointerId = pointerId;
	inputEvent.X0 = x0 / screenDensity;
	inputEvent.Y0 = y0 / screenDensity;
	inputEvent.X1 = x1 / screenDensity;
	inputEvent.Y1 = y1 / screenDensity;
	g_renderer->onMotionEvent(inputEvent);
}

#pragma clang diagnostic pop