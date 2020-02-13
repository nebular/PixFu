//
// Created by rodo on 2020-02-13.
//

#include "GyroController.hpp"


namespace rgl {

	const int LOOPER_ID_USER = 3;

	GyroController *GyroController::pCurrentInstance = nullptr;

	GyroController::GyroController(int xlen, int ylen, bool autoStart) : AxisController(xlen, ylen) {

		sensorManager = AcquireASensorManagerInstance();
		if (sensorManager == nullptr) throw 1;
		rotation = ASensorManager_getDefaultSensor(sensorManager,
												   ASENSOR_TYPE_GAME_ROTATION_VECTOR);
		if (rotation == nullptr) throw 2;
		looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
		if (looper == nullptr) throw 3;

		rotationEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER,
															 nullptr, nullptr);
		if (rotationEventQueue == nullptr) throw 4;

		if (autoStart) enable(true);

	}

	GyroController::~GyroController() {
		enable(false);
	}

	bool GyroController::enable(bool stat) {
		if (stat) {
			auto status = ASensorEventQueue_enableSensor(rotationEventQueue, rotation);
			return status > 0;
		} else {
			ASensorEventQueue_disableSensor(rotationEventQueue, rotation);
			return true;
		}
	}

	ASensorManager *GyroController::AcquireASensorManagerInstance() {
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

	void GyroController::poll() {
		ASensorEventQueue_getEvents(rotationEventQueue, &tCurrentEvent, 1) ;
		inputGyroscope(tCurrentEvent.vector.azimuth, tCurrentEvent.vector.pitch);
	}

}