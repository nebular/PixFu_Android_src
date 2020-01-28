//
// Created by rodo on 2020-01-26.
//

#ifndef GLES3JNI_LONESENSOR_H
#define GLES3JNI_LONESENSOR_H

#include "android/sensor.h"
#include <dlfcn.h>
#include <cassert>
#include <arch/android/androidapi.h>


class LoneSensor {

private:
	const char *kPackageName = "com.android.accelerometergraph";

	ALooper *looper;
	ASensorManager *sensorManager;

	const ASensor *rotation;
	ASensorEventQueue *rotationEventQueue;


	ASensorManager *AcquireASensorManagerInstance(void);

public:

	LoneSensor();

	virtual ~LoneSensor();

	bool init();

	void update();

	bool fetch(ASensorEvent &event);

};

#endif //GLES3JNI_LONESENSOR_H
