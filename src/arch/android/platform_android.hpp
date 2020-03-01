//
// Created by rodo on 2020-02-12.
//
#ifdef ANDROID

#ifndef PIXENGINE_ANDROID_ANDROIDLAYER_H
#define PIXENGINE_ANDROID_ANDROIDLAYER_H

#include "OpenGL.h"
#include "plugins/lonescreenkey.h"
#include "Utils.hpp"

namespace Pix {

	typedef struct sMotionEvent {
		int PointersCount, Action, PointerId, RawAction;
		int32_t X0, Y0, X1, Y1;
	} MotionEvent_t;

	class PixFuPlatformAndroid : public FuPlatform {

		LoneScreenKey *cLoneKeys;                // screenkeys helper class

		MotionEvent_t tCurrentMotionEvent;

		Fu *pBootInstance;

		bool init(Fu *engine) override;

		std::pair<bool, bool> events() override;

		void commit() override;

		void deinit() override;

		void onFps(Fu *engine, int fps) override;


	public:

		PixFuPlatformAndroid(Fu *bootInstance);

		~PixFuPlatformAndroid();

		Fu *engine();

		void inputMotionEvent(MotionEvent_t event);

	};

	inline Fu *PixFuPlatformAndroid::engine() { return pBootInstance; }

}
#endif

#endif //PIXENGINE_ANDROID_ANDROIDLAYER_H
