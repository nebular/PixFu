//
// Created by rodo on 2020-02-13.
//
#ifndef PIXENGINE_ANDROID_GYROCONTROLLER_HPP
#define PIXENGINE_ANDROID_GYROCONTROLLER_HPP

#include "AxisController.hpp"
#include "android/sensor.h"
#include "dlfcn.h"
#include <cassert>
#include "arch/android/androidapi.h"

namespace rgl {

	class GyroController : public AxisController {


		const char *kPackageName = "com.android.accelerometergraph";

		ALooper *looper;
		ASensorManager *sensorManager;
		const ASensor *rotation;
		ASensorEventQueue *rotationEventQueue;

		ASensorEvent tCurrentEvent;

		ASensorManager *AcquireASensorManagerInstance(void);

		GyroController(int xlen, int ylen, bool autoStart = true);

		~GyroController();


	public:

		bool enable(bool enable);

		void update();

		static void enable(int xlen, int ylen);

		static GyroController *currentInstance();

		static GyroController *pCurrentInstance;

	};


	inline GyroController *GyroController::currentInstance() { return pCurrentInstance; }

	inline void GyroController::enable(int xlen, int ylen) {
		pCurrentInstance = new GyroController(xlen, ylen);
	}

}
#endif //PIXENGINE_ANDROID_GYROCONTROLLER_HPP
