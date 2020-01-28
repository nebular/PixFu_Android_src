//
// Created by rodo on 2020-01-24.
//

#ifndef GLES3JNI_ANDROID_H
#define GLES3JNI_ANDROID_H

#ifdef ANDROID

#include "png.h"
#include "arch/android/androidapi.h"
#include "plugins/lonesensor.h"
#include "plugins/lonescreenkey.h"
typedef struct sMotionEvent {
	int PointersCount, Action, X0, Y0, X1, Y1;
} MotionEvent_t;

void close_window();
void close_application();
void refresh_window();

#endif
#endif //GLES3JNI_ANDROID_H
