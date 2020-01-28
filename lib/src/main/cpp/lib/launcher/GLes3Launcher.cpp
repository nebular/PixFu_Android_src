/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "olcPixelGameEngine.h"
#include "GLes3Launcher.h"
#include "RendererOlc.h"

static void printGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

static Renderer* g_renderer = NULL;


extern "C" {
    JNIEXPORT void JNICALL Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jclass obj, jstring path);
    JNIEXPORT void JNICALL Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jclass obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_onTouch(JNIEnv* env, jclass obj,jobject motionEvent);
};

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jclass obj, jstring internalFilesPath) {

    if (g_renderer) {
        delete g_renderer;
        g_renderer = NULL;
    }

    const char *cstr = env->GetStringUTFChars(internalFilesPath, NULL);

    olc::PixelGameEngine::ROOTPATH = std::string(cstr) + "/";

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if ((strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) || strstr(versionStr, "OpenGL ES 2.")) {
        g_renderer = RendererOlc::createRender(olc::PixelGameEngine::BOOTINSTANCE);
    } else {
        ALOGE("Unsupported OpenGL ES version");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jclass obj, jint width, jint height) {
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jclass obj) {
    if (g_renderer) {
        g_renderer->render();
    }
}


extern "C" JNIEXPORT void JNICALL
Java_tv_nebular_olcpxe_android_gles3jni_GLES3JNILib_onTouch(JNIEnv *jenv, jclass obj,jobject motionEvent) {
	jclass motionEventClass=jenv->GetObjectClass(motionEvent);

	jmethodID pointersCountMethodId = jenv->GetMethodID(motionEventClass,"getPointerCount", "()I");
	int pointersCount = jenv->CallIntMethod(motionEvent, pointersCountMethodId);
	jmethodID getActionMethodId = jenv->GetMethodID(motionEventClass, "getAction", "()I");
	int32_t action = jenv->CallIntMethod(motionEvent, getActionMethodId);

	jmethodID getXMethodId = jenv->GetMethodID(motionEventClass, "getX", "(I)F");
	float x0 = jenv->CallFloatMethod(motionEvent, getXMethodId,0);

	jmethodID getYMethodId = jenv->GetMethodID(motionEventClass, "getY", "(I)F");
	float y0 = jenv->CallFloatMethod(motionEvent, getYMethodId,0);

	float x1=-1;
	float y1=-1;
	if(pointersCount>1){
		x1 = jenv->CallFloatMethod(motionEvent, getXMethodId,1);
		y1 = jenv->CallFloatMethod(motionEvent, getYMethodId,1);
	}

	MotionEvent_t inputEvent;
	inputEvent.PointersCount=pointersCount;
	inputEvent.Action=action;
	inputEvent.X0=x0;
	inputEvent.Y0=y0;
	inputEvent.X1=x1;
	inputEvent.Y1=y1;
	ALOGE("mouse x0 %d y0 %d x1 %d, y1 %d, action %d",x0,y0,x1,y1,action);
	g_renderer->OnMotionEvent(inputEvent);
}
