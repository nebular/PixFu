//
// Created by rodo on 2020-02-13.
//

#include "GyroController.hpp"


namespace Pix {

	const int LOOPER_ID_USER = 3;

	GyroController *GyroController::pCurrentInstance = nullptr;

	GyroController::GyroController(bool autoStart)
	: AxisController({-1,1,-1,1}) {

		sensorManager = AcquireASensorManagerInstance();
		if (sensorManager == nullptr) throw std::runtime_error("No sensor manager");
		rotation = ASensorManager_getDefaultSensor(sensorManager,
												   ASENSOR_TYPE_GAME_ROTATION_VECTOR);
		if (rotation == nullptr) throw std::runtime_error("No rotation sensor");
		looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
		if (looper == nullptr) throw std::runtime_error("Cannot get a looper");

		rotationEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER,
															 nullptr, nullptr);
		if (rotationEventQueue == nullptr)
			throw std::runtime_error("Cannot get sensor event queue");

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
		auto getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
				dlsym(androidHandle, "ASensorManager_getInstanceForPackage");

		if (getInstanceForPackageFunc) {
			return getInstanceForPackageFunc(kPackageName);
		}

		typedef ASensorManager *(*PF_GETINSTANCE)();
		auto getInstanceFunc = (PF_GETINSTANCE)
				dlsym(androidHandle, "ASensorManager_getInstance");

		// by all means at this point, ASensorManager_getInstance should be available
		assert(getInstanceFunc);
		return getInstanceFunc();
	}

	void GyroController::poll() {
		ASensorEventQueue_getEvents(rotationEventQueue, &tCurrentEvent, 1);
		inputGyroscope(tCurrentEvent.vector.azimuth, tCurrentEvent.vector.pitch);
	}

}