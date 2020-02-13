//
// Created by rodo on 2020-02-12.
//
#ifdef ANDROID

#ifndef PIXENGINE_ANDROID_ANDROIDLAYER_H
#define PIXENGINE_ANDROID_ANDROIDLAYER_H

#include "OpenGL.h"
#include "plugins/lonescreenkey.h"
#include "Utils.hpp"

namespace rgl {

	typedef struct sMotionEvent {
		int PointersCount, Action, PointerId, RawAction;
		int32_t X0, Y0, X1, Y1;
	} MotionEvent_t;

	class PixFuPlatformAndroid : public PixFuPlatform {

		LoneScreenKey *cLoneKeys;                // screenkeys helper class

		MotionEvent_t tCurrentMotionEvent;

		PixFu *pBootInstance;

		bool init(rgl::PixFu *engine);

		std::pair<bool, bool> events();

		void commit();

		void deinit();

		void onFps(int fps);


	public:

		PixFuPlatformAndroid(PixFu *bootInstance);

		~PixFuPlatformAndroid();

		PixFu *engine();

		void inputMotionEvent(MotionEvent_t event);

	};

	inline PixFu* PixFuPlatformAndroid::engine() { return pBootInstance; }

}
#endif

#endif //PIXENGINE_ANDROID_ANDROIDLAYER_H
