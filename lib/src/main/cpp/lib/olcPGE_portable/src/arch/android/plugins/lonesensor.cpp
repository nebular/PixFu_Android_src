//
// Created by rodo on 2020-01-26.
//


#include "lonesensor.h"

const int LOOPER_ID_USER = 3;

ASensorManager *LoneSensor::AcquireASensorManagerInstance(void) {
	typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE)(const char *name);
	void *androidHandle = dlopen("libandroid.so", RTLD_NOW);
	PF_GETINSTANCEFORPACKAGE getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
			dlsym(androidHandle, "ASensorManager_getInstanceForPackage");
	if (getInstanceForPackageFunc) {
		return getInstanceForPackageFunc(kPackageName);
	}

	typedef ASensorManager *(*PF_GETINSTANCE)();
	PF_GETINSTANCE getInstanceFunc = (PF_GETINSTANCE)
			dlsym(androidHandle, "ASensorManager_getInstance");
	// by all means at this point, ASensorManager_getInstance should be available
	assert(getInstanceFunc);
	return getInstanceFunc();
}

LoneSensor::LoneSensor() {
}

LoneSensor::~LoneSensor() {
	ASensorEventQueue_disableSensor(rotationEventQueue, rotation);

}

bool LoneSensor::init() {
	sensorManager = AcquireASensorManagerInstance();
	if (sensorManager == NULL) return false;
	rotation = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GAME_ROTATION_VECTOR);
	if (rotation == NULL) return false;
	looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	if (looper == NULL) return false;
	rotationEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER,
														 NULL, NULL);
	if (rotationEventQueue == NULL) return false;
	auto status = ASensorEventQueue_enableSensor(rotationEventQueue, rotation);
	return status > 0;
}

void LoneSensor::update() {
	ASensorEvent event;
	while (fetch(event)) {
		ALOGE("AZIMUTH %f", event.vector.azimuth);
	}

}

bool LoneSensor::fetch(ASensorEvent &event) {
	return ASensorEventQueue_getEvents(rotationEventQueue, &event, 1) > 0;
}

