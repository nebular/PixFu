//
// Created by rodo on 2020-02-13.
//

#pragma once

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

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

		void poll();

		ASensorEvent raw();

		static void enable(int xlen, int ylen);

		static GyroController *instance();

		static GyroController *pCurrentInstance;

	};

	inline ASensorEvent GyroController::raw() { return tCurrentEvent; }

	inline GyroController *GyroController::instance() { return pCurrentInstance; }

	inline void GyroController::enable(int xlen, int ylen) {
		pCurrentInstance = new GyroController(xlen, ylen);
	}

}

#pragma clang diagnostic pop