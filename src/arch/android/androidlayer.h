//
// Created by rodo on 2020-02-12.
//
#ifdef ANDROID

#ifndef PIXENGINE_ANDROID_ANDROIDLAYER_H
#define PIXENGINE_ANDROID_ANDROIDLAYER_H

#include "OpenGL.h"
#include "plugins/lonescreenkey.h"
#include "plugins/lonesensor.h"

namespace rgl {

	typedef struct sMotionEvent {
		int PointersCount, Action, PointerId, RawAction;
		int32_t X0, Y0, X1, Y1;
	} MotionEvent_t;

	class PixEngineAndroid : public PixEnginePlatform {

		ASensorEvent tCurrentEvent;            	// current sensor event

		LoneScreenKey *cLoneKeys;            	// screenkeys helper class
		LoneSensor *pLoneSensor;            	// gyroscope joystick helper class

		MotionEvent_t tCurrentMotionEvent;

		bool init();

		std::pair<bool, bool> events();

		void commit();

		void deinit();

		void onFps(int fps);

	public:

		static PixEngine *BOOTINSTANCE;    // OLC engine to run

		~PixEngineAndroid();

		void inputMotionEvent(MotionEvent_t event);

	};
}
#endif

#endif //PIXENGINE_ANDROID_ANDROIDLAYER_H
